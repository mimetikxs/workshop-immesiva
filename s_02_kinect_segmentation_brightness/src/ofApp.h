#pragma once
#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	
    void exit();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxKinect kinect;
    
    ofTexture texDepth;  // 2^8 (or 256) values
    
    // thresholded
    // ...
    
    ofxPanel gui;
    // ...
};
