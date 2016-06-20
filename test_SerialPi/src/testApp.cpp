//RPI VERSION

#define NUM_CRYSTALS 7

#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    w = 160;h = 120;
    cam.setup(w, h,false);

    panel.setPosition(250, 800);
	
    panel.setup("Optical Flow");
	
    panel.add(levels.set("levels", 4, 1, 8));
    panel.add(winsize.set("winsize", 8, 4, 64));
    panel.add(iterations.set("iterations", 2, 1, 8));
    panel.add(polyN.set("polyN", 7, 5, 10));
    panel.add(polySigma.set("polySigma", 1.5, 1.1, 2));
    panel.add(OPTFLOW_FARNEBACK_GAUSSIAN.set("OPTFLOW_FARNEBACK_GAUSSIAN", false));
	
    panel.add(winSize.set("winSize", 32, 4, 64));
    panel.add(maxLevel.set("maxLevel", 3, 0, 8));
	
    panel.add(maxFeatures.set("maxFeatures", 200, 1, 1000));
    panel.add(qualityLevel.set("qualityLevel", 0.01, 0.001, .02));
    panel.add(minDistance.set("minDistance", 4, 1, 16));

    farneback.setNumLevels( levels.get() );
    farneback.setWindowSize( winsize.get() );
    farneback.setNumIterations( iterations.get() );
    farneback.setPolyN( polyN.get() );
    farneback.setPolySigma( polySigma.get() );
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN.get());
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    int baud = 9600;
    serial.setup(0, baud);

    crystals.resize(NUM_CRYSTALS);

    for(int i = 0; i < NUM_CRYSTALS; i++) {
    	crystals[i].setId(i);
    	crystals[i].setRotation(ofRandom(0, 360));
    	crystals[i].setSerial(&serial);
    }
}

void testApp::update(){
    frame = cam.grab();
    if(!frame.empty()) {

        farneback.calcOpticalFlow(frame);
        
        ofVec2f averageFlow = farneback.getAverageFlow();
        for(int i = 0; i < crystals.size(); i++) {
        	crystals[i].update(averageFlow.x);
        }
        // if(abs(averageFlow.x) > 1) {
        //     if(averageFlow.x > 0) {
        //         serial.writeByte('c');
        //         serial.writeByte('1');
        //     } else {
        //         serial.writeByte('c');
        //         serial.writeByte('0');
        //     }
        // }
	}
}

void testApp::draw(){
//	ofBackground(0);
//		
//	ofSetColor(255);
//    if(!frame.empty()) drawMat(frame,220,0,w*4,h*4);
//    curFlow->draw(220,0,w*4,h*4);
//    panel.draw();
}
