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
    
    const float scaling = 0.5;
    
    const int projectorWidth = 1024;
    const int projectorHeight = 768;
    
    // ---------------------
    // video
    // ---------------------
    
    ofVideoPlayer videoBlobs;
    ofTexture texBlobs;
    
    // ---------------------
    // warping
    // ---------------------
    
    ofxGLWarper warper;
    
    ofFbo    fboWarpedBlobs; // gpu memory
    ofPixels pixWarpedBlobs; // cpu memory
    
    bool warpModeOn;
    
    void enableWarpMode();
    void disableWarpMode();
    void drawWarpMode();

    // ---------------------
    // opencv processing
    // ---------------------
    
    // grayscaling
    ofImage grey;
    
    // contours
    ofxCv::ContourFinder contours;

    // ---------------------
    // gui
    // ---------------------

    ofParameter<int> blurSize;

    ofxPanel gui;

    bool bDrawGui;
    
};
