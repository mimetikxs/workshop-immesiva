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
    pixThresholded.allocate(kinect.getWidth(), kinect.getHeight(), OF_PIXELS_GRAY);
    
    // gui
    gui.setup("brightness");
    gui.add(minBrightness.set("min", 0, 0, 255));
    gui.add(maxBrightness.set("max", 255, 0, 255));
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
        int w = pixDepth.getWidth();
        int h = pixDepth.getHeight();
        
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                int i = (y * w) + x;
                int value = pixDepth[i];
                
                if (value > minBrightness && value <= maxBrightness)
                {
                    pixThresholded[i] = 255;
                }
                else
                {
                    pixThresholded[i] = 0;
                }
            }
        }
        
        // upload pixel data to gpu for drawing
        texThresholded.loadData(pixThresholded);
	}
}


void ofApp::draw()
{
    float w = kinect.getWidth();
    float h = kinect.getHeight();
    
    // draw kinect images
    texDepth.draw(0, 0);
    
    // draw thresholded image
    texThresholded.draw(w, 0);
    
    // labels
    ofDrawBitmapStringHighlight("DEPTH (8bits)", 20, 20);
    ofDrawBitmapStringHighlight("THRESHOLDED", w+20, 20);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),0), ofGetWidth()-20, ofGetHeight()-20);
    
    gui.draw();
}


void ofApp::keyPressed (int key)
{
}

