#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	cout<<"Starting up..."<<endl;
	ofSetFrameRate(15);
	lightChangeSpeed = 0.1;
	maxOutputValue = 100.f;
	int camWidth = 320;
	int camHeight = 240;

	// GPIO setup
	// numberOfPins = 21;
	registeredPins = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,21,22,23,24,25,26,27};
	cout<<"GPIO setup"<<endl;
	wiringPiSetup();
	setupSoftPwmPins(registeredPins); // Also accept a vector of pin IDs

	// Video feed
	cout<<"Initializing video feed"<<endl;
	ofSetVerticalSync(true);
    cam.setDesiredFrameRate(15);
    cam.initGrabber(camWidth, camHeight);

	imitate(previous, cam);
	imitate(diff, cam);

	cout<<"Up and running!"<<endl;
}

//--------------------------------------------------------------
void ofApp::update() {
    cam.update();
    
    if(cam.isFrameNew()) {
    	// take the absolute difference of prev and cam and save it inside diff
		absdiff(cam, previous, diff);
		diff.update();
		
		// like ofSetPixels, but more concise and cross-toolkit
		copy(cam, previous);
		
		// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
		diffMean = mean(toCv(diff));
		
		// you can only do math between Scalars,
		// but it's easy to make a Scalar from an int (shown here)
		diffMean *= Scalar(50);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	// ofBackground(0);
	// ofSetColor(255);
    // cam.draw(0, 0);

 //    ofPushMatrix();
 //    ofTranslate(320, 0);
 //    diff.draw(10, 10);
 //    ofPopMatrix();
	
	// use the [] operator to get elements from a Scalar
	// float diffRed = diffMean[0];
	// float diffGreen = diffMean[1];
	// float diffBlue = diffMean[2];
	
	// ofSetColor(255, 0, 0);
	// ofDrawRectangle(0, 0, diffRed, 10);
	// ofSetColor(0, 255, 0);
	// ofDrawRectangle(0, 15, diffGreen, 10);
	// ofSetColor(0, 0, 255);
	// ofDrawRectangle(0, 30, diffBlue, 10);

    // float oldPinValue = pinValue;
	// pinValue = ofMap(getAverage(diffMean[0], diffMean[1], diffMean[2]), 6, 600, 0.0f, maxOutputValue, true);
	// pinValue = ofLerp(oldPinValue, pinValue, lightChangeSpeed);

	for (int i=0; i<registeredValues.size(); i++){
		float oldPinValue = registeredValues[i];
		registeredValues[i] = ofMap(getAverage(diffMean[0], diffMean[1], diffMean[2]), 6, 600, 0.0f, maxOutputValue, true);
		registeredValues[i] = ofLerp(oldPinValue, registeredValues[i], lightChangeSpeed);
	}

	// writeAllSoftPwmPins(pinValue);
	writeAllSoftPwmPins(registeredValues);
	// cout<<pinValue<<endl;
	// cout<<registeredValues[23]<<endl;
}

//--------------------------------------------------------------
// Pins setup functions
void ofApp::setupSoftPwmPins(){
	cout<<"Initializing "<<numberOfPins<<" pins..."<<endl;
	for (int i=0; i<numberOfPins; i++){
		softPwmCreate(i, 0, int(maxOutputValue));
		registeredPins.push_back(i);
	}
	registeredValues.resize(registeredPins.size());
}

void ofApp::setupSoftPwmPins(vector<int> pinNo){
	cout<<"Initializing "<<pinNo.size()<<" pins..."<<endl;
	for (int i=0; i<pinNo.size(); i++){
		softPwmCreate(pinNo[i], 0, 100);
		registeredPins.push_back(pinNo[i]);
	}
	registeredValues.resize(registeredPins.size());
}

//--------------------------------------------------------------
// Pins write functions
void ofApp::writeAllSoftPwmPins(float pinValue){
	for (int i=0; i<registeredPins.size(); i++){
		softPwmWrite(registeredPins[i], pinValue);
	}
}

void ofApp::writeAllSoftPwmPins(vector<float> pinValues){
	if(registeredPins.size() != pinValues.size()){
		cout<<"The output vector is of the wrong size!"<<endl;
		exit();
	}else{
		for (int i=0; i<registeredPins.size(); i++){
			softPwmWrite(registeredPins[i], pinValues[i]);
		}
	}
}

//--------------------------------------------------------------
// Utility functions
float ofApp::getAverage(float a, float b, float c){
	float result;
	if (a == 0){
		if (b == 0){
			result = c;
		}else if (c == 0){
			result = b;
		}
		result = (b+c)/2;
	}else if (b == 0){
		if (c == 0){
			result = a;
		}
		result = (a+c)/2;
	}else if (c == 0){
		result = (a+b)/2;
	}

	result = (a + b + c)/3;
	return result;
}



//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}