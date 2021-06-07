#pragma once
#include "ofMain.h"
#include "Forcefield.h"

class ForcefieldFlow : public Forcefield
{
public:
    
    ofParameter<float> decay;
    
    
    void setup(string name) override
    {
        Forcefield::setup(name);
        parameters.add( decay.set( "decay", 1, 0, 1));
    }
    

    void setFlow(ofFloatPixels& flow)
    {        
        // is it valid?
        if (flow.getWidth() != cols || flow.getHeight() != rows || flow.getNumChannels() != 2)
        {
            ofLogError("ForcefieldFlow::setFlow()") << "flow pixels must be 2-channel and have the same dimensions as the grid.";
            return;
        }
        
        // update forces
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                int i = (x + y * cols);
                
                // no-decay
                //forces[i].x = flow[i * 2];
                //forces[i].y = flow[i * 2 + 1];
                
                // decay
                forces[i].x += (flow[i * 2]     - forces[i].x) * decay;
                forces[i].y += (flow[i * 2 + 1] - forces[i].y) * decay;
            }
        }
    }
};
