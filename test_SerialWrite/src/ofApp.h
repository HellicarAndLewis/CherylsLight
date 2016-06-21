#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxSerial.h"
#include "ofxIntegrator.h"

#define NUM_MOTORS 7



class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
	
	ofVideoGrabber camera;
	
	ofxCv::FlowFarneback fb;
	ofxCv::FlowPyrLK lk;
	
	ofxCv::Flow* curFlow;
    
    const char startOfNumberDelimiter = '<';
    const char endOfNumberDelimiter   = '>';
		
    ofxPanel gui;
    ofParameter<float> fbPyrScale, lkQualityLevel, fbPolySigma;
    ofParameter<int> fbLevels, lkWinSize, fbIterations, fbPolyN, fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
    ofParameter<bool> fbUseGaussian, usefb;
    ofParameter<int> sleepTime;
    float timeOfLastMessage;
    
    ofx::IO::SerialDevice device;
    
    Byte motors[NUM_MOTORS];

};

