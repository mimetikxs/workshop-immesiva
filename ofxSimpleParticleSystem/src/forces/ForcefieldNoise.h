#pragma once
#include "ofMain.h"
#include "Forcefield.h"

class ForcefieldNoise : public Forcefield
{
public:
    
    ofParameter<float> noiseScale;
    ofParameter<float> noiseSpeed;
    
    
    void setup(string name) override
    {
        Forcefield::setup(name);
        parameters.add( noiseScale.set( "noise scale", 0.009, 0.001, 0.15));
        parameters.add( noiseSpeed.set( "noise speed", 0, 0, 0.5));
    }
    
    
    void updateNoise()
    {
        float offset = ofGetElapsedTimef() * noiseSpeed;
        
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                // get noise value between 0..1
                //float noise = ofRandom(0, 1);
                float noise = ofNoise(
                                      x * noiseScale + offset,
                                      y * noiseScale + offset
                                      );
                // remap noise value to an angle in radians
                noise = ofMap(noise, 0.0, 1.0, 0.0, TWO_PI);
                
                // find the index
                int i = (x + y * cols);
                
                // angle to unit vector
                forces[i].x = cos(noise);
                forces[i].y = sin(noise);
            }
        }
    }
};
