#pragma once

//
// A wrapper that allows to easily switch between a live kinect or a recording.
//

#include "ofxGui.h"

class VideoSource {
    
public:
    
    virtual void setup() = 0;
    
    virtual void update() = 0;
    
    virtual ofPixels& getPixels() = 0;
    
    virtual bool isFrameNew() = 0;
    
    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    
    virtual void addParameters(ofxPanel& gui) = 0;
    
    virtual ~VideoSource() {}
    
};
