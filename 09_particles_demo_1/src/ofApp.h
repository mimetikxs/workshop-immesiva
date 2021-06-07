#pragma once
#include "ofMain.h"
#include "ofxSimpleParticleSystem.h"
#include "ofxSimplePresets.h"
#include "ofxGui.h"

using namespace glm;

class ofApp : public ofBaseApp
{
public:

	void setup();
	void update();
    void draw();
    void keyPressed(int key);

    // particle system
    ParticleSystem particleSystem;
    EmitterRect emitter;
    ForceWind forceWind;
    ForceAttraction forceAttraction;

    // render
    RenderLines renderLines;
    RenderPoints renderPoints;

    ofFbo fboLines;
    ofFbo fboPoints;
    ofParameter<float> trailsOpacity;

    // presets
    ofxSimplePresets presets;
    
    // gui
    ofxPanel gui;
    bool bDrawGui = true;
    bool bDrawDebug = false;

    void drawDebug();
};
