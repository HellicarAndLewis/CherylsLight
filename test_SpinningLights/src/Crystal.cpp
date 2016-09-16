//
//  Crystal.cpp
//  spinningLights
//
//  Created by James Bentley on 5/9/16.
//
//

#include "Crystal.h"
#include "Light.h"


Crystal::Crystal() {
    location = ofVec2f(0, 0);
    speed = 0.0f;
    rotation = 0.0f;
    maxSpeed = 50.0f;
}

void Crystal::draw() {
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(0);
    ofNoFill();

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(location.x, location.y);
    ofRotate(rotation, 0, 0, 1);
    
    ofSetLineWidth(20);
    
    float lightLength = 300;
    for(int i = 0; i < 4; i++) {
        ofSetColor(255, 0, 0, 50);
        ofDrawLine(0, 0, lightLength, 0);
        
        ofSetColor(0, 0, 255, 50);
        ofDrawLine(0, 0, 0, lightLength);
        
        ofSetColor(0, 255, 0, 50);
        ofDrawLine(0, 0, -lightLength, 0);
        
        ofSetColor(255, 255, 0, 50);
        ofDrawLine(0, 0, 0, -lightLength);
    }
    ofDrawRectangle(0, 0, 20, 20);
    ofPopStyle();
    ofPopMatrix();
}

void Crystal::update() {
    rotation += speed;
}