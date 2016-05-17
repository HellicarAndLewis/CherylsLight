#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxSerial.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber camera;
	
	ofxCv::FlowFarneback fb;
	ofxCv::FlowPyrLK lk;
	
	ofxCv::Flow* curFlow;
		
    ofxPanel gui;
    ofParameter<float> fbPyrScale, lkQualityLevel, fbPolySigma;
    ofParameter<int> fbLevels, lkWinSize, fbIterations, fbPolyN, fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
    ofParameter<bool> fbUseGaussian, usefb;
    
    ofx::IO::SerialDevice device;
    
};

