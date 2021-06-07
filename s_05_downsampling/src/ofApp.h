#pragma once
#include "ofMain.h"
#include "ofxGLWarper.h"

class ofApp : public ofBaseApp {
public:
	
    void exit();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    // constants
    const int projectorWidth = 1024;
    const int projectorHeight = 768;
    
    const float scaling = 0.5; //< scale down factor
    
    // blobs (input)
    ofVideoPlayer videoBlobs;
    ofTexture texBlobs;
    
    // warping
    ofxGLWarper warper;
    ofFbo fboWarpedBlobs;
    
    bool warpModeOn; //< edit the warping full screen
    
    void enableWarpMode();
    void disableWarpMode();
    void drawWarpMode();
};
