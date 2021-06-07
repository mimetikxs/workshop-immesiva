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
    
    ofTexture texDepthRaw;  // 2^16 (or 65,536) values
    
    ofPixels pixThresholded;
    ofTexture texThresholded;
    
    ofxPanel gui;
    ofParameter<int> minDistance;
    ofParameter<int> maxDistance;
};
