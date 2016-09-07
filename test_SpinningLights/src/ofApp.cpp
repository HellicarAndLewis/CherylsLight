#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    float theta = 0;
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        
        crystals[i].setLocation(ofVec2f(circleCenter.x + circleRadius * cos(theta), circleCenter.y + circleRadius * sin(theta)));
        crystals[i].setInitialSpeed(ofRandom(-0.02, 0.02));
        crystals[i].setRotation(ofRandom(0.0, 360.0));

        lights[i].setLocation(ofVec2f(circleCenter.x + (circleRadius + 50) * cos(theta), circleCenter.y + (circleRadius + 50) * sin(theta)));
        lights[i].setLookAt(circleCenter);
        
        theta += (2 * PI)/NUM_CRYSTALS;
        
        crystals[i].setLight(&lights[i]);
        lights[i].setCrystal(&crystals[i]);
    }
    gui.setup();
    gui.add(flow.set("Optical Flow", 0.0, -1.0, 1.0));
    gui.add(flowResetSpeed.set("Flow ResetSpeed", 0.01, 0.01, 0.1));
    gui.add(flowEffect.set("Flow Effect", 0.5, 0.0, 1.0));
    
    circleMask.load("shaders/mask.vert", "shaders/mask.frag");
    
    buffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    buffer.begin();
    ofClear(0);
    buffer.end();
    ofBackground(127);

}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].update();
        crystals[i].setSpeed(ofLerp(crystals[i].getSpeed(), crystals[i].getInitialSpeed() + flow * flowEffect, 0.1));
    }
    flow = ofLerp(flow, 0.0, flowResetSpeed);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //buffer.begin();

    ofPushStyle();
    ofSetColor(255);
    ofSetCircleResolution(50);
    ofNoFill();
    ofDrawCircle(circleCenter.x, circleCenter.y, circleRadius);
    ofPopStyle();
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        lights[i].draw();
    }
    
    buffer.begin();
    ofClear(0);
    ofBackground(127);
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].draw();
    }
    buffer.end();
    //buffer.end();

    circleMask.begin();
    circleMask.setUniform1f("radius", circleRadius-1);
    circleMask.setUniform2f("size", ofGetWidth(), ofGetHeight());
    buffer.draw(0, 0);
    circleMask.end();

    gui.draw();
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
