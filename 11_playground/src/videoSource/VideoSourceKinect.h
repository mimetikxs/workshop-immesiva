#include "VideoSource.h"
#include "ofMain.h"
#include "ofxKinect.h"


class VideoSourceKinect : public VideoSource
{
public:
    
    ~VideoSourceKinect()
    {
        kinect.close();
    }
    
    
    void setup() override
    {
        kinect.setUseTexture(false); // disable kinect textures
        kinect.init(false, false);   // use depth only, disable video image (faster fps)
        kinect.open();               // open kinect
        
        // allocate thresholded pixels
        pixThresholded.allocate(kinect.getWidth(), kinect.getHeight(), OF_PIXELS_GRAY);
    }
    
    
    void update() override
    {
        kinect.update();
        
        if(kinect.isFrameNew())
        {
            // get references to kinect raw depth pixels
            // this pixels are 16bit with values from 0 to 65,535
            // closest pixels are darker (as they represent distance from sensor)
            ofShortPixels& pixDepthRaw = kinect.getRawDepthPixels();
            
            // update thresholded pixels
            int w = pixDepthRaw.getWidth();
            int h = pixDepthRaw.getHeight();
            
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int i = (y * w) + x;
                    int value = pixDepthRaw[i];
                    
                    if (value > minDistance && value <= maxDistance)
                    {
                        pixThresholded[i] = 255;
                    }
                    else
                    {
                        pixThresholded[i] = 0;
                    }
                }
            }
        }
    }
    
    
    bool isFrameNew() override
    {
        return kinect.isFrameNew();
    }
    
    
    ofPixels& getPixels() override
    {
        return pixThresholded;
    }
    
    
    virtual float getWidth() override
    {
        return kinect.getWidth();
    }
    
    
    virtual float getHeight() override
    {
        return kinect.getHeight();
    }
    
    
    void addParameters(ofxPanel& gui) override
    {
        ofParameterGroup parameters;
        parameters.setName("kinect threshold");
        parameters.add(minDistance.set("min dist", 0, 0, 6000));
        parameters.add(maxDistance.set("max dist", 6000, 0, 6000));
        
        gui.add(parameters);
    }
    
    
private:
    
    ofxKinect kinect;
    
    ofPixels pixThresholded;
    
    ofParameter<int> minDistance;
    ofParameter<int> maxDistance;
    
};
