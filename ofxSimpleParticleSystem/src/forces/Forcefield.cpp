#include "Forcefield.h"


void Forcefield::setup(string name)
{
    parameters.setName(name);
    parameters.add( forceScale.set( "force scale", 1, 0.001, 10));
}


void Forcefield::setupGrid(float width, float height, float _cellSize)
{
    cellSize = _cellSize;

    cols = width / cellSize;
    rows = height / cellSize;

    // init the grid of vectors (all will be 0)
    forces.resize(cols * rows); 
}


void Forcefield::applyForce(Particle& p)
{
    // find column and row
    int col = ceil(p.pos.x / cellSize) - 1;
    int row = ceil(p.pos.y / cellSize) - 1;

    // only apply force to particles inside the grid
    if (col >= 0 && col < cols && row >= 0 && row < rows)
    {
        p.applyForce( getForceAtCell(col, row) * forceScale.get() );
    }
}


vec2 Forcefield::getForceAtCell(int col, int row)
{
    int i = col + row * cols;
    return forces[i];
}


void Forcefield::draw()
{
    ofPushStyle();
    ofNoFill();

    float halfCellSize = cellSize * 0.5;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            vec2 cellCenter = vec2(x, y) * cellSize + halfCellSize;
            
            // draw cells
            //ofDrawRectangle(cellCenter - vec2(halfCellSize), cellSize, cellSize);
            
            // draw force vectors
            vec2 vec = getForceAtCell(x, y) * 20;
            ofDrawLine(cellCenter, cellCenter + vec);
        }
    }

    ofPopStyle();
}
