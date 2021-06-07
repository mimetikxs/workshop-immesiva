#pragma once
#include "ofMain.h"
//#include "ofxKinect.h" // here we use a pre recorded video
#include "ofxGLWarper.h"

class ofApp : public ofBaseApp {
public:
	
    void exit();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    // projector res
    const int projectorWidth = 1024;
    const int projectorHeight = 768;
	
    // blobs (input)
	ofVideoPlayer videoBlobs;
    ofTexture texBlobs;
    
    // warping
    ofxGLWarper warper;
    
    // an fbo is like a texture that we can draw into
    // we draw the warped blobs here
    ofFbo fboWarpedBlobs;
    
};
