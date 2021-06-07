#include "ofApp.h"


void ofApp::exit()
{
    kinect.close();
}


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(100);
    
    kinect.setUseTexture(false); // disable kinect textures
	kinect.init(false, false);   // use depth only, disable video image (faster fps)
	kinect.open();               // open kinect
    
    // allocate thresholded pixels
    // ...
    
    // gui
    gui.setup("brightness");
    // ...
    gui.setPosition(20, 500);
}


void ofApp::update()
{
	kinect.update();
	
	if(kinect.isFrameNew())
    {
        // get references to kinect depth
        // this pixels are 8bit with values from 0 to 255
        // closest pixels are brighter
        ofPixels& pixDepth = kinect.getDepthPixels();
        
        // upload pixels to gpu
        texDepth.loadData(pixDepth);
        
        // update thresholded pixels
        // ...
        
        // upload pixel data to gpu for drawing
        // ...
	}
}


void ofApp::draw()
{
    float w = kinect.getWidth();
    float h = kinect.getHeight();
    
    // draw kinect images
    texDepth.draw(0, 0);
    
    // draw thresholded image
    // ...
    
    // labels
    ofDrawBitmapStringHighlight("DEPTH (8bits)", 20, 20);
    ofDrawBitmapStringHighlight("THRESHOLDED", w+20, 20);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),0), ofGetWidth()-20, ofGetHeight()-20);
    
    gui.draw();
}


void ofApp::keyPressed (int key)
{
}

