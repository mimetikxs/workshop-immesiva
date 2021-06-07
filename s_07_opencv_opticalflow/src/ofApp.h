#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxGLWarper.h"

class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    // ---------------------
    // constants
    // ---------------------
    
    const int projectorWidth = 1024;
    const int projectorHeight = 768;
    
    const float scaling = 0.2; //0.5;
    
    // ---------------------
    // video
    // ---------------------
    
    ofVideoPlayer videoBlobs;
    ofTexture     texBlobs;
    
    // ---------------------
    // warping
    // ---------------------
    
    ofxGLWarper warper;
    
    ofFbo    fboWarpedBlobs;
    ofPixels pixWarpedBlobs;
    
    bool warpModeOn;
    
    void enableWarpMode();
    void disableWarpMode();
    void drawWarpMode();

    // ---------------------
    // opencv processing
    // ---------------------
    
    ofImage grey;
    
    ofxCv::FlowFarneback flow; //< optical flow

    ofFloatPixels pixFlow;   // 32bits, more than 4 million values
    ofTexture     texFlow;
    
    void initFlow(int w, int h);

    // ---------------------
    // gui
    // ---------------------

    ofParameter<int> blurSize;

    ofxPanel gui;

    bool bDrawGui;
    
};
