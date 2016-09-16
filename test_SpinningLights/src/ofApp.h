#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Crystal.h"
#include "Light.h"
#include "ofxCv.h"
#ifdef __linux
#include "ofxCvPiCam.h"
#endif
#define NUM_CRYSTALS 7
#define NUM_LIGHTS NUM_CRYSTALS

//#define PROP_WEIGHTED_LERP
//#define PROP_DELAY
#define PROP_LOCATION

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void drawMask();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void propagateImpulse(float flow);
    
    Crystal crystals[NUM_CRYSTALS];
    Light lights[NUM_LIGHTS];
    
    ofxPanel gui;
    ofParameter<float> flow;
    ofParameter<float> flowDownSpeed;
    ofParameter<float> flowUpSpeed;
    ofParameter<float> flowEffect;
    ofParameter<float> propagationFactor;
    ofParameter<float> dimFac;

    ofxCv::FlowFarneback flowFb;
    int w, h;
#ifdef __linux
    ofxCvPiCam camera;
    cv::Mat frame;
    cv::Mat smallFrame;
#else
    ofVideoGrabber camera;
#endif
    
#ifdef PROP_DELAY
    deque<float> flowHistory;
#endif
#ifdef PROP_LOCATION
    float flowsFromLeftToRight[NUM_CRYSTALS];
#endif
    
    ofShader circleMask;
    
    ofFbo buffer;
    
    ofVec2f circleCenter = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
    float circleRadius = 300;
};
