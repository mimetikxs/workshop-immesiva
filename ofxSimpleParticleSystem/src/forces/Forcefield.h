#pragma once
#include "ofMain.h"
#include "Force.h"

using namespace glm;

class Forcefield : public Force
{
public:

    ofParameter<float> forceScale;
    
    // inherited functionality
    void setup(string name) override;
    void applyForce(Particle& p) override;
    void draw() override;

    // specific functionality
    void setupGrid(float width, float height, float cellSize);

protected:

    int cols = 0;
    int rows = 0;
    float cellSize = 0;

    // our grid is stored in a 1 dimensional array
    vector<vec2> forces;

    vec2 getForceAtCell(int col, int row);
    vec2 getForceAtPos(vec2& pos);
};
