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
    disableWarpMode();
    
    // allocate fbo to draw result of warping
    // size is the scaled projector resolution
    fboWarpedBlobs.allocate(projectorWidth, projectorHeight); //< ...
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
        
        fboWarpedBlobs.begin();
        {
            // clear with red to see actual size of fbo
            ofClear(255,0,0, 255);
            
            // draw warped (downscaled)
            // ...
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
    if (warpModeOn)
    {
        drawWarpMode();
    }
    else
    {
        // draw downscaled blobs
        fboWarpedBlobs.draw(0, 0);
        
        // draw info
        string fboWidth = ofToString(fboWarpedBlobs.getWidth());
        string fboHeight = ofToString(fboWarpedBlobs.getHeight());
        string outWidth = ofToString(projectorWidth);
        string outHeight = ofToString(projectorHeight);
        string scale = ofToString(scaling * 100, 0);
        ofDrawBitmapString("fbo: " + fboWidth + "x" + fboHeight + " (scale " + scale + "%)", 20, fboWarpedBlobs.getHeight() + 30);
        ofDrawBitmapString("output: " + outWidth + "x" + outHeight, 20, fboWarpedBlobs.getHeight() + 50);
    }
}


void ofApp::drawWarpMode()
{
    // do warping on full screen
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
            if (warpModeOn)
                disableWarpMode();
            else
                enableWarpMode();
            break;
            
        default:
            break;
    }
}

