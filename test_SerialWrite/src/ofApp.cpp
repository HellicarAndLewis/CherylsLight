#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(0);
    
	camera.setup(320, 240);
	
    gui.setup();
    
    gui.add(lkMaxLevel.set("lkMaxLevel", 3, 0, 8));
    gui.add(lkMaxFeatures.set("lkMaxFeatures", 200, 1, 1000));
    gui.add(lkQualityLevel.set("lkQualityLevel", 0.01, 0.001, .02));
    gui.add(lkMinDistance.set("lkMinDistance", 4, 1, 16));
    gui.add(lkWinSize.set("lkWinSize", 8, 4, 64));
    gui.add(usefb.set("Use Farneback", true));
	gui.add(fbPyrScale.set("fbPyrScale", .5, 0, .99));
	gui.add(fbLevels.set("fbLevels", 4, 1, 8));
    gui.add(fbIterations.set("fbIterations", 2, 1, 8));
    gui.add(fbPolyN.set("fbPolyN", 7, 5, 10));
    gui.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
    gui.add(fbUseGaussian.set("fbUseGaussian", false));
    gui.add(fbWinSize.set("winSize", 32, 4, 64));
    gui.add(sleepTime.set("Sleep Time", 3000, 50, 3000));

	curFlow = &fb;
    
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        bool success = device.setup(devicesInfo[0], 115200);
        
        if(success)
        {
            ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
        }
        else
        {
            ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
    
    for(int i = 0; i < NUM_MOTORS; i++) {
        motors[i] = i;
    }
    
    timeOfLastMessage = 0.0;
        
//    ofSetFrameRate(10);
    //serial.listDevices();
    
    //auto devives = serial.getDeviceList();
    
}

void ofApp::update(){
    
    float time = ofGetElapsedTimeMillis();
    if(time - timeOfLastMessage > sleepTime) {
        Byte buff [3];
        
        int val = 258;

        Byte b1 = val / 256;
        Byte b2 = val % 256;
        
        buff[0] = startOfNumberDelimiter;
        buff[1] = b2;
        buff[2] = endOfNumberDelimiter;
        
        device.writeBytes(&buff[0], 3);
        timeOfLastMessage = time;
    }

    //    device.writeBytes("Test");
//    device.writeBytes(startOfNumberDelimiter + 't' + endOfNumberDelimiter);
//    for (int i = 0; i < 10; i++)
//    {
//        Serial.print (startOfNumberDelimiter);
//        Serial.print (rand ());    // send the number
//        Serial.print (endOfNumberDelimiter);
//        Serial.println ();
//    }  // end of for
//    
//    delay (5000);
    
//    float time = ofGetElapsedTimeMillis();
//    if(time - timeOfLastMessage > sleepTime) {
//        camera.update();
//        
//        int val = 201;
//        
//        Byte b1 = val / 256;
//        Byte b2 = val % 256;
//
//        Byte buf[2] = {
//            b1, b2
//        };
//        
//        device.writeBytes(&buf[0], 2);
//        timeOfLastMessage = time;
//    }
	
//
//	if(camera.isFrameNew()) {
//		
//		if(usefb) {
//			curFlow = &fb;
//			fb.setPyramidScale(fbPyrScale);
//			fb.setNumLevels(fbLevels);
//			fb.setWindowSize(fbWinSize);
//			fb.setNumIterations(fbIterations);
//			fb.setPolyN(fbPolyN);
//			fb.setPolySigma(fbPolySigma);
//            fb.setUseGaussian(fbUseGaussian);
//		} else {
//			curFlow = &lk;
//			lk.setMaxFeatures(lkMaxFeatures);
//			lk.setQualityLevel(lkQualityLevel);
//			lk.setMinDistance(lkMinDistance);
//			lk.setWindowSize(lkWinSize);
//			lk.setMaxLevel(lkMaxLevel);
//		}
//		
//		// you can use Flow polymorphically
//		curFlow->calcOpticalFlow(camera);
//        
////        Byte b1 = 500 / 256;
////        Byte b2 = (int)500 % 256;
////        Byte buf[2] = {b1, b2};
////        device.writeBytes(&buf[0], 2);
////        cout<<"Flow Sent: "<<b1*256 + b2<<endl;
//        
//        ofVec2f averageFlow = fb.getAverageFlow();
//        int averageXFlow = (int)(ofMap(averageFlow.x, -3, 3, 0, 1000, true));
//        Byte b1 = averageXFlow / 256;
//        Byte b2 = (int)averageXFlow % 256;
//        Byte buf[2] = {b1, b2};
//         if(abs(averageFlow.x) > 1) {
//             device.writeBytes(&buf[0], 2);
//             cout<<"Flow Sent: "<<b1*256 + b2<<endl;
//         } else {
////             device.writeBytes(&buf[0], 2);
//             cout<<"Flow UnSent: "<<b1*256 + b2<<endl;
//         }
//	}
}

void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(250, 100);
	camera.draw(0,0,640,480);
    curFlow->draw(0,0,640,480);
    ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()) + "fps", 10, 20);
    ofPopMatrix();
    gui.draw();
}

void ofApp::keyPressed(int key) {
    //device.writeByte('k');
    if(key == OF_KEY_RIGHT) {
        Byte b1 =  1000 / 256;
        Byte b2 = (int)1000 % 256;
        Byte buf[2] = {b1, b2};
        device.writeBytes(&buf[0], 2);
//        device.writeBytes(&buf[0], 2);
        cout<<"Flow Sent: "<<b1*256 + b2<<endl;
    } else if(key == OF_KEY_LEFT) {
        Byte b1 =  (0 / 256);
        Byte b2 = (int)(0 % 256);
        Byte buf[2] = {b1, b2};
        device.writeBytes(&buf[0], 2);
//        device.writeBytes(&buf[0], 2);
        cout<<"Flow Sent: "<<b1*256 + b2<<endl;
    }
}