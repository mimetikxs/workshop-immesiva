#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxGLWarper.h"
#include "ofxSimpleParticleSystem.h"
#include "ofxSimplePresets.h"
#include "VideoSourceKinect.h"
#include "VideoSourceRecording.h"

//
// uncomment this to use a live kinect instead of a recording
//
//#define USE_LIVE_KINECT


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
    
    const float scaling = 0.2;
    
    // ---------------------
    // video source
    // ---------------------
    
#ifdef USE_LIVE_KINECT
    VideoSourceKinect videoBlobs;
#else
    VideoSourceRecording videoBlobs;
#endif
    
    ofTexture texBlobs;
    
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
    
    ofxCv::ContourFinder contours;
    
    ofxCv::FlowFarneback flow;    
    ofFloatPixels pixFlow;
    ofTexture     texFlow;
    
    void updateFlowParams();
    
    // ---------------------
    // particle system
    // ---------------------
    
    ParticleSystem particleSystem;
    EmitterRect    emitter;
    ForceWind      forceWind;
    ForcefieldFlow forcefield;
    
    // renderer
    RenderLines renderLines;
    ofFbo       fboParticles;
    
    void initParticleSystem();

    // ---------------------
    // gui
    // ---------------------

    // blur
    ofParameter<int>   blurPre;
    ofParameter<int>   blurPost;
    // optical flow
    ofParameter<float> pyramidScale;
    ofParameter<int>   numLevels;
    ofParameter<int>   windowSize;
    ofParameter<int>   numIterations;
    ofParameter<bool>  resetFlow;
    // rendering
    ofParameter<float> trailsOpacity;
    ofParameter<bool>  drawForcefield;
    ofParameter<bool>  drawContours;
    ofParameter<bool>  drawEmitters;
    
    ofxSimplePresets presets;

    ofxPanel gui;

    bool bDrawGui;
    
    void initGui();
    
};
