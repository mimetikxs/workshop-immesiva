#include "ofApp.h"


void ofApp::exit()
{
    kinect.close();
}


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(100);
	
	// enable depth->video calibration
	//kinect.setRegistration(true);
    
    // disable kinect textures (we'll handle it ourselves)
    kinect.setUseTexture(false);
    
	kinect.init();
	//kinect.init(true);         // use infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
    // opens first available kinect
	kinect.open();
}


void ofApp::update()
{
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew())
    {
        // get references to kinect data
        ofPixels&      pixVideo    = kinect.getPixels();
        ofPixels&      pixDepth    = kinect.getDepthPixels();
        ofShortPixels& pixDepthRaw = kinect.getRawDepthPixels();
        
        // upload pixels to gpu
        texVideo.loadData(pixVideo);
        texDepth.loadData(pixDepth);
        texDepthRaw.loadData(pixDepthRaw);
	}
}


void ofApp::draw()
{
    float w = kinect.getWidth();
    float h = kinect.getHeight();
    
    // draw kinect maps
    texVideo.draw(0,0);
    texDepth.draw(w, 0);
    texDepthRaw.draw(w, h);
    
    // labels
    ofDrawBitmapStringHighlight("VIDEO", 20, 20);
    ofDrawBitmapStringHighlight("DEPTH", w+20, 20);
    ofDrawBitmapStringHighlight("RAW DEPTH", w+20, h+20);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),0), 20, ofGetHeight()-30);
}


void ofApp::keyPressed (int key)
{
	switch (key)
    {
		case OF_KEY_UP:
        {
			float angle = kinect.getTargetCameraTiltAngle();
            angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
        }
		case OF_KEY_DOWN:
        {
            float angle = kinect.getTargetCameraTiltAngle();
            angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
        }
	}
}

