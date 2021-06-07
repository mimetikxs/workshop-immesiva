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
    gui.setup("distance (mm)");
    gui.add(minDistance.set("min", 0, 0, 6000));
    gui.add(maxDistance.set("max", 6000, 0, 6000));
    gui.setPosition(20, 500);
}


void ofApp::update()
{
	kinect.update();
	
	if(kinect.isFrameNew())
    {
        // get references to kinect raw depth pixels
        // this pixels are 16bit with values from 0 to 65,535
        // closest pixels are darker (as they represent distance from sensor)
        ofShortPixels& pixDepthRaw = kinect.getRawDepthPixels();
        
        // upload pixels to gpu
        texDepthRaw.loadData(pixDepthRaw);
        
        // update thresholded pixels
        int w = texDepthRaw.getWidth();
        int h = texDepthRaw.getHeight();
        
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                int i = (y * w) + x;
                int value = pixDepthRaw[i];
                
                if (value > minDistance && value <= maxDistance)
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
    texDepthRaw.draw(0, 0);
    
    // draw thresholded image
    texThresholded.draw(w, 0);
    
    // labels
    ofDrawBitmapStringHighlight("DEPTH (16bits)", 20, 20);
    ofDrawBitmapStringHighlight("THRESHOLDED", w+20, 20);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),0), ofGetWidth()-20, ofGetHeight()-20);
    
    gui.draw();
}


void ofApp::keyPressed (int key)
{
}

