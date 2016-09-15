#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Crystal.hpp"
#include "Light.hpp"
#include "ofxCv.h"

#define NUM_CRYSTALS 7
#define NUM_LIGHTS NUM_CRYSTALS

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
    
    Crystal crystals[NUM_CRYSTALS];
    Light lights[NUM_LIGHTS];
    
    ofxPanel gui;
    ofParameter<float> flow;
    ofParameter<float> flowResetSpeed;
    ofParameter<float> flowEffect;
    
    ofShader circleMask;
    
    ofFbo buffer;
    
    ofVec2f circleCenter = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
    float circleRadius = 300;
};
