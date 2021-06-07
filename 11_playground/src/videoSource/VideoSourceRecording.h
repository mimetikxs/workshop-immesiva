#include "VideoSource.h"
#include "ofMain.h"


class VideoSourceRecording : public VideoSource
{
public:
    
    void setup() override
    {
        player.load("../../../common_data/silhouette.mp4");
        player.setUseTexture(false);
        player.setLoopState(OF_LOOP_NORMAL);
        player.play();
    }
    
    
    void update() override
    {
        player.update();
    }
    
    
    bool isFrameNew() override
    {
        return player.isFrameNew();
    }
    
    
    ofPixels& getPixels() override
    {
        return player.getPixels();
    }
    
    
    virtual float getWidth() override
    {
        return player.getWidth();
    }
    
    
    virtual float getHeight() override
    {
        return player.getHeight();
    }
    
    
    void addParameters(ofxPanel& gui) override
    {
        // do nothing...
    }
    
    
private:
    
    ofVideoPlayer player;
    
};
