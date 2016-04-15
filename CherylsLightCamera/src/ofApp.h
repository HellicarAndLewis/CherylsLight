#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "RPiVideoGrabber.h"
#include "ofxCv.h"

#include "ofxWiringPi.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);

		float getAverage(float a, float b, float c);

		void setupSoftPwmPins();
		void setupSoftPwmPins(vector<int> pinNo);

		void writeAllSoftPwmPins(float pinValue);
		void writeAllSoftPwmPins(vector<float> pinValues);

		// Video grabber
		RPiVideoGrabber cam;
		
		// CV stuff
	    ofPixels previous;
		ofImage diff;
		cv::Scalar diffMean;

	private:
		float lightChangeSpeed;
		float maxOutputValue;
		int numberOfPins;
		float pinValue = 0;
		vector<int> registeredPins;
		vector<float> registeredValues;
};
