#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(80);

    // init video source
    videoBlobs.setup();
    
    // init warper
    warper.setup(videoBlobs.getWidth(), videoBlobs.getHeight());
    warper.load("warp_settings.xml");
    disableWarpMode();
    
    // downscaled size
    int scaledWidth = projectorWidth * scaling;
    int scaledHeight = projectorHeight * scaling;

    // allocate for results of warping
    fboWarpedBlobs.allocate(scaledWidth, scaledHeight);
    pixWarpedBlobs.allocate(scaledWidth, scaledHeight, OF_PIXELS_GRAY);
    
    // optical flow
    pixFlow.allocate(scaledWidth, scaledHeight, 2);       // two channel pixels
    texFlow.allocate(scaledWidth, scaledHeight, GL_RG8);  // two channel texture
    
    // particle system
    initParticleSystem();
    
    // gui
    initGui();
}


void ofApp::initParticleSystem()
{
    // emitter
    emitter.setup("emitter");
    
    // forces
    forceWind.setup("wind");
    
    forcefield.setup("force field");
    forcefield.setupGrid(projectorWidth, projectorHeight, 1.0 / scaling);
    
    // particle system
    particleSystem.setup(projectorWidth, projectorHeight, 10000);
    particleSystem.addEmitter(&emitter);
    particleSystem.addForce(&forceWind);
    particleSystem.addForce(&forcefield);
    
    // rendering
    renderLines.setup("renderer lines");
    
    fboParticles.allocate(projectorWidth, projectorHeight, GL_RGB32F);
}


void ofApp::initGui()
{
    // gui:
    
    gui.setup();
    
    // background removal
    // (might be empty if using a recording)
    videoBlobs.addParameters(gui);
    
    // optical flow
    gui.setDefaultHeaderBackgroundColor(ofColor(0, 0, 155));
    gui.setDefaultBorderColor(ofColor(0, 0, 155));
    
    ofParameterGroup groupFlow;
    groupFlow.setName("optcial flow");
    groupFlow.add(blurPre.set("blur pre", 2, 0, 15));
    groupFlow.add(pyramidScale.set("pyramid scale", 0.5, 0.1, 0.9));
    groupFlow.add(numLevels.set("num levels", 4, 1, 10));
    groupFlow.add(windowSize.set("window size", 15, 5, 25));
    groupFlow.add(numIterations.set("iterations", 1, 1, 10));
    groupFlow.add(blurPost.set("blur post", 0, 0, 15));
    groupFlow.add(resetFlow.set("reset flow", false));
    gui.add(groupFlow);
    
    // particles
    gui.setDefaultHeaderBackgroundColor(ofColor(0, 155, 0));
    gui.setDefaultBorderColor(ofColor(0, 155, 0));
    
    gui.add(particleSystem.parameters);
    gui.add(emitter.parameters);
    gui.add(forceWind.parameters);
    gui.add(forcefield.parameters);
    
    // rendering
    gui.setDefaultHeaderBackgroundColor(ofColor(155, 0, 0));
    gui.setDefaultBorderColor(ofColor(155, 0, 0));
    
    renderLines.parameters.add(trailsOpacity.set("trails opacity", 0, 0, 1));
    gui.add(renderLines.parameters);
    gui.add(drawContours.set("draw contours", true));
    gui.add(drawForcefield.set("draw forcefield", false));
    gui.add(drawEmitters.set("draw emitters", false));
    
    gui.setPosition(ofGetWidth() - 220, 120);
    gui.minimizeAll();
    
    bDrawGui = true;
    
    // presets:
    
    presets.setup(&gui, 10);
    presets.applyPreset(0);
}


void ofApp::updateFlowParams()
{
    flow.setPyramidScale(pyramidScale);
    flow.setNumLevels(numLevels);
    flow.setWindowSize(windowSize);
    flow.setNumIterations(numIterations);
    flow.setPolyN(7);
    flow.setPolySigma(1.5);
}


void ofApp::update()
{
    videoBlobs.update();

    if (videoBlobs.isFrameNew())
    {
        // ---------------------
        // warp and downscale video
        // ---------------------

        texBlobs.loadData(videoBlobs.getPixels());
        fboWarpedBlobs.begin();
        {
            ofClear(0,255);
            ofScale(scaling);
            warper.begin();
            {
                texBlobs.draw(0, 0);
            }
            warper.end();
        }
        fboWarpedBlobs.end();
        
        fboWarpedBlobs.readToPixels(pixWarpedBlobs); //< download from gpu

        // ---------------------
        // opencv processing
        // ---------------------

        ofxCv::convertColor(pixWarpedBlobs, grey, CV_RGB2GRAY);
        
        contours.findContours(grey);

        if (blurPre > 0)
        {
            ofxCv::blur(grey, blurPre);
        }

        grey.update(); //< upload to gpu
        
        // ---------------------
        // optical flow
        // ---------------------
        
        if (resetFlow)
        {
            flow.resetFlow();
            resetFlow = false;
        }
        updateFlowParams();
        flow.calcOpticalFlow(grey);
        
        // get flow image and store it in pixFlow
        ofxCv::toOf(flow.getFlow(), pixFlow);
        
        if (blurPost > 0)
        {
            ofxCv::blur(pixFlow, blurPost);
        }
        
        texFlow.loadData(pixFlow); //< upload to gpu
        
        // ---------------------
        // particle system
        // ---------------------
        
        // update forcefield
        forcefield.setFlow(pixFlow);
        
        // update particles
        particleSystem.update();
        
        // update renderer
        renderLines.update( particleSystem.getParticleData() );
    }
}


void ofApp::draw()
{
    if (warpModeOn)
    {
        drawWarpMode();
    }
    else
    {
        // particles
        fboParticles.begin();
        {
            ofSetColor(0,0,0, (1 - trailsOpacity) * 255);
            ofDrawRectangle(0, 0, fboParticles.getWidth(), fboParticles.getHeight());
            renderLines.draw();
        }
        fboParticles.end();
        fboParticles.draw(0, 0);
        
        // debug
        if (drawForcefield)
        {
            forcefield.draw();
        }
        if (drawEmitters)
        {
            emitter.draw();
        }
        if (drawContours)
        {
            ofPushMatrix();
            ofScale(1.0 / scaling);
            contours.draw();
            ofPopMatrix();
        }
        
        // gui
        if (bDrawGui)
        {
            float w = projectorWidth * scaling;
            float h = projectorHeight * scaling;
            
            fboWarpedBlobs.draw(0, 0);
            grey.draw(w, 0);
            texFlow.draw(w*2, 0);
            
            ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 70, ofGetHeight() - 20);
            
            gui.draw();
            presets.draw();
        }
    }
}


void ofApp::drawWarpMode()
{
    warper.begin();
    {
        texBlobs.draw(0, 0);
    }
    warper.end();
    
    // draw instructions
    stringstream ss;
    ss << "Drag the corners of the image to warp it." << endl;
    ss << "Press and hold shift key, then clic over the image and drag to move it." << endl;
    ss << "Use arrow keys to move selected corner." << endl;
    ss << "Press space key to activate/deactivate warper.";
    ofDrawBitmapString(ss.str(), 20, 20);
}


void ofApp::enableWarpMode()
{
    warper.activate();
    warpModeOn = true;
}


void ofApp::disableWarpMode()
{
    warper.deactivate();
    warpModeOn = false;
}


void ofApp::keyPressed(int key)
{
    switch (key)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            presets.applyPreset(key-48);
            break;
            
        case 's':
            warper.save("warp_settings.xml");
            break;
            
        case 'l':
            warper.load("warp_settings.xml");
            break;
            
        case ' ':
            if (warpModeOn)
                disableWarpMode();
            else
                enableWarpMode();
            break;
            
        case 'g':
            bDrawGui = !bDrawGui;
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        default:
            break;
    }
}
