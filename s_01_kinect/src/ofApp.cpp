#include "ofApp.h"


void ofApp::exit()
{
    // ...
}


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(100);
	
	// ...
}


void ofApp::update()
{
	// ...
}


void ofApp::draw()
{
    float w = kinect.getWidth();
    float h = kinect.getHeight();
    
    // draw kinect maps
    // ...
    
    // labels
    ofDrawBitmapStringHighlight("VIDEO", 20, 20);
    ofDrawBitmapStringHighlight("DEPTH", w+20, 20);
    ofDrawBitmapStringHighlight("RAW DEPTH", w+20, h+20);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),0), 20, ofGetHeight()-30);
}


void ofApp::keyPressed (int key)
{
	
}

