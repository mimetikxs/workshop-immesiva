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
    
    // ...

    // gui
    gui.setup();
    // ...
    gui.setPosition(10, scaledHeight + 20);

    bDrawGui = true;
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
        
        // ... //< download from gpu

        // ---------------------
        // open cv
        // ---------------------
        
        // color to grey
        // ...
        
        // find contours
        // ...
        
        // blur grey image
        // ...

        // ... //< upload to gpu
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
        
        // warped blobs
        fboWarpedBlobs.draw(0, 0);
        ofDrawBitmapStringHighlight("warped (scaled " + ofToString(scaling * 100) + "%)", 5, 15);
        
        // blur
        // ...
        ofDrawBitmapStringHighlight("grey + blur", w+5, 15);
        
        // contours
        // ...
        ofDrawBitmapStringHighlight("contours", w+5, h+15);
        
        // fps
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 70, ofGetHeight() - 20);
        
        // borders
        ofPushStyle();
        ofNoFill();
        ofSetColor(120);
        ofDrawRectangle(0, 0, w, h);
        ofDrawRectangle(w, 0, w, h);
        ofDrawRectangle(w, h, w, h);
        ofPopStyle();
        
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
