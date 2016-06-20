#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    float theta = 0;
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        
        crystals[i].setLocation(ofVec2f(circleCenter.x + circleRadius * cos(theta), circleCenter.y + circleRadius * sin(theta)));
        crystals[i].setInitialSpeed(ofRandom(-0.02, 0.02));
        crystals[i].setRotation(ofRandom(0, 360));
        lights[i].setLocation(ofVec2f(circleCenter.x + (circleRadius + 50) * cos(theta), circleCenter.y + (circleRadius + 50) * sin(theta)));
        lights[i].setLookAt(circleCenter);
        
        theta += (2 * PI)/NUM_CRYSTALS;
        
        crystals[i].setLight(&lights[i]);
        lights[i].setCrystal(&crystals[i]);
    }
    
    mask.load("images/Circle.png");
    
    gui.setup("main", "settings/Settings.xml");
    
    gui.add(flow.set("Flow", 0.0, -5, 5));
    gui.add(flowResetSpeed.set("Reset Speed", 0.01, 0.00001, 0.1));
    gui.loadFromFile("settings/Settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].update();
    }
    
    if(flow < 0 - flowResetSpeed) {
        flow += flowResetSpeed;
    } else if(flow > 0 + flowResetSpeed) {
        flow -= flowResetSpeed;
    } else {
        flow = 0.0;
    }
    
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].setSpeed(crystals[i].getInitialSpeed() + ofMap(flow, -5, 5, -1, 1));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
    ofSetColor(255);
    ofSetCircleResolution(50);
    ofNoFill();
    ofDrawCircle(circleCenter.x, circleCenter.y, circleRadius);
    ofPopStyle();
    
    ofSetColor(0);
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].draw();
        lights[i].draw();
    }
    drawMask();
    
    gui.draw();
}

void ofApp::drawMask() {
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255);
    mask.draw(circleCenter, circleRadius*2, circleRadius*2);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofDrawRectangle(0, 0, ofGetWidth(), (ofGetHeight() - circleRadius*2)/2);
    ofDrawRectangle(0, 0, (ofGetWidth() - circleRadius*2)/2, ofGetHeight());
    ofDrawRectangle(0, ofGetHeight() - (ofGetHeight() - circleRadius*2)/2, ofGetWidth(), (ofGetHeight() - circleRadius*2)/2);
    ofDrawRectangle(ofGetWidth() - (ofGetWidth() - circleRadius*2)/2, 0, (ofGetWidth() - circleRadius*2)/2, ofGetHeight());
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
