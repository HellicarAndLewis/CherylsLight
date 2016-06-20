#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    float theta = 0;
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        
        crystals[i].setLocation(ofVec2f(circleCenter.x + circleRadius * cos(theta), circleCenter.y + circleRadius * sin(theta)));
        crystals[i].setSpeed(ofRandom(0.01, 0.02));
        lights[i].setLocation(ofVec2f(circleCenter.x + (circleRadius + 50) * cos(theta), circleCenter.y + (circleRadius + 50) * sin(theta)));
        lights[i].setLookAt(circleCenter);
        
        theta += (2 * PI)/NUM_CRYSTALS;
        
        crystals[i].setLight(&lights[i]);
        lights[i].setCrystal(&crystals[i]);
    }
    
    gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].update();
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
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].draw();
        lights[i].draw();
    }
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
