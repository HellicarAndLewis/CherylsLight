//RPI VERSION

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "Crystal.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	vector<Crystal> crystals;
	
    int w,h;
    ofxCvPiCam cam;
    cv::Mat frame;

    ofxCv::FlowFarneback farneback;
	
    ofxCv::Flow* curFlow;
		
    ofxPanel panel;
    ofParameter<int> levels;
    ofParameter<int> winsize;
    ofParameter<int> iterations;
    ofParameter<int> polyN;
    ofParameter<float> polySigma;
    ofParameter<bool> OPTFLOW_FARNEBACK_GAUSSIAN;

    ofParameter<int> winSize;
    ofParameter<int> maxLevel;

    ofParameter<int> maxFeatures;
    ofParameter<float> qualityLevel;
    ofParameter<int> minDistance;
    
    ofSerial serial;
};

