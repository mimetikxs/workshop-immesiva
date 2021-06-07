#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxGLWarper.h"
#include "ofxSimpleParticleSystem.h" //< forcefields are part of the particle system


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
    
    const float scaling = 0.1;
    
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
    
    ofxCv::FlowFarneback flow;
    
    ofFloatPixels pixFlow; // 32bits, more than 4 million values
    ofTexture     texFlow;
    
    void initFlow(int w, int h);
    
    // ---------------------
    // particle system
    // ---------------------
    
    ForcefieldFlow forcefield; //<
    
    ofParameter<bool> drawForcefield;

    // ---------------------
    // gui
    // ---------------------

    ofParameter<int> blurSize;

    ofxPanel gui;

    bool bDrawGui;
    
    void initGui();
    
};
