#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(80);

    // init video
    videoBlobs.load("../../../common_data/silhouette.mp4");
    videoBlobs.setUseTexture(false);
    videoBlobs.setLoopState(OF_LOOP_NORMAL);
    videoBlobs.play();
    
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
    initFlow(scaledWidth, scaledHeight);
    
    // forcefield
    // our field will fill the projector, cell size is based on the scaling
    // ...

    // gui
    initGui();
}


void ofApp::initGui()
{
    // gui
    gui.setup();
    gui.add(blurSize.set("blur size", 2, 0, 15));
    // ...
    gui.add(drawForcefield.set("draw forcefield", true));
    
    gui.setPosition(ofGetWidth() - 220, 20);
    
    bDrawGui = true;
}


void ofApp::initFlow(int w, int h)
{
    flow.setPyramidScale(.5);
    flow.setNumLevels(4);
    flow.setWindowSize(15);
    flow.setNumIterations(1);
    flow.setPolyN(7);
    flow.setPolySigma(1.5);
    
    pixFlow.allocate(w, h, 2);       // two channel pixels
    texFlow.allocate(w, h, GL_RG8);  // two channel texture
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
        // convert to grey and blur
        // ---------------------

        ofxCv::convertColor(pixWarpedBlobs, grey, CV_RGB2GRAY);

        if (blurSize > 0)
        {
            ofxCv::blur(grey, blurSize);
        }

        grey.update(); //< upload to gpu
        
        // ---------------------
        // optical flow
        // ---------------------
        
        flow.calcOpticalFlow(grey);
        
        // get flow image and store it in pixFlow
        ofxCv::toOf(flow.getFlow(), pixFlow);
        
        //ofxCv::blur(pixFlow, 10);
        
        texFlow.loadData(pixFlow); //< upload to gpu
        
        // ---------------------
        // force field
        // ---------------------
        
        // ...
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
        float w = projectorWidth * scaling;
        float h = projectorHeight * scaling;
        
        fboWarpedBlobs.draw(0, 0);
        
        grey.draw(w, 0);
        
        texFlow.draw(w*2, 0);
        
        if (drawForcefield)
        {
            // ...
        }
        
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 70, ofGetHeight() - 20);
        
        if (bDrawGui)
        {
            gui.draw();
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
    switch (key) {
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
            
        default:
            break;
    }
}
