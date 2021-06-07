#pragma once
#include "ofMain.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp {
public:
	
    void exit();
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofxKinect kinect;
    
    // previews data from kinect
    ofTexture texVideo;         // can be rgb or infrared video
    ofTexture texDepth;         // 2^8 (or 256) values
    ofTexture texDepthRaw;      // 2^16 (or 65,536) values
};
