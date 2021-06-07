#include "ofApp.h"


void ofApp::exit()
{}


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(100);
    
    // init video
    videoBlobs.load("../../../common_data/silhouette.mp4");
    // ...
    
    // init warper (same resolution as video)
    // ...
    
    // allocate fbo to draw result of warping (same resolution as projector)
    // ...
}


void ofApp::update()
{
    videoBlobs.update();
    
    if (videoBlobs.isFrameNew())
    {
        // ...
    }
}


void ofApp::draw()
{
    // draw warped blobs
    // ...
    
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
            // save warping
            // ...
            break;
            
        case 'l':
            // load warping
            // ...
            break;
            
        case ' ':
            // toggle warping
            // ...
            break;
            
        default:
            break;
    }
}

