#include "ofApp.h"


void ofApp::exit()
{}


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(100);
    
    // init video
    videoBlobs.load("../../../common_data/silhouette.mp4");
    videoBlobs.setUseTexture(false);
    videoBlobs.setLoopState(OF_LOOP_NORMAL);
    videoBlobs.play();
    
    // init warper (same size as video)
    warper.setup(videoBlobs.getWidth(), videoBlobs.getHeight());
    warper.activate();  // activate mouse interaction
    
    // allocate fbo to draw result of warping (same resolution as projector)
    fboWarpedBlobs.allocate(projectorWidth, projectorHeight);
}


void ofApp::update()
{
    videoBlobs.update();
    
    if (videoBlobs.isFrameNew())
    {
        // get a reference to pixels
        ofPixels& pixBlobs = videoBlobs.getPixels();
        
        // upload to gpu
        texBlobs.loadData(pixBlobs);
        
        // draw output
        fboWarpedBlobs.begin();
        {
            //comment out to see what happens :)
            ofClear(0);
            
            // draw stretching
            //texBlobs.draw(0, 0, fboWarpedBlobs.getWidth(), fboWarpedBlobs.getHeight());
            
            // draw warping
            warper.begin();
            {
                texBlobs.draw(0, 0);
            }
            warper.end();
        }
        fboWarpedBlobs.end();
    }
}


void ofApp::draw()
{
    fboWarpedBlobs.draw(0, 0);
    
    // draw info
    string inputWidth = ofToString(videoBlobs.getWidth());
    string inputHeight = ofToString(videoBlobs.getHeight());
    string outputWidth = ofToString(projectorWidth);
    string outputHeight = ofToString(projectorHeight);
    ofDrawBitmapStringHighlight("input: " + inputWidth + "x" + inputHeight, ofGetWidth()-140, 20);
    ofDrawBitmapStringHighlight("output: " + outputWidth + "x" + outputHeight, ofGetWidth()-140, 40);
    
    // draw instructions
    stringstream ss;
    ss << "Drag the corners of the image to warp it." << endl;
    ss << "Press and hold shift key, then clic over the image and drag to move it." << endl;
    ss << "Use arrow keys to move selected corner." << endl;
    ss << "Press space key to activate/deactivate warper.";
    ofDrawBitmapStringHighlight(ss.str(), 20, ofGetHeight()-60);
}


void ofApp::keyPressed (int key)
{
    switch (key) {
        case 's':
            warper.save("warp_settings.xml");
            break;
            
        case 'l':
            warper.load("warp_settings.xml");
            break;
            
        case ' ':
            if (warper.isActive())
                warper.deactivate();
            else
                warper.activate();
            break;
            
        default:
            break;
    }
}

