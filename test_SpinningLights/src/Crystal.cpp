//
//  Crystal.cpp
//  spinningLights
//
//  Created by James Bentley on 5/9/16.
//
//

#include "Crystal.hpp"
#include "Light.hpp"


Crystal::Crystal() {
    location = ofVec2f(0, 0);
    speed = 0.0f;
    rotation = 0.0f;
}

void Crystal::draw() {
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(0);
    ofNoFill();

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(location.x, location.y);
    ofRotate(rotation, 0, 0, 1);
    float lightLength = 1000;
    for(int i = 0; i < 4; i++) {
        ofDrawLine(0, 0, lightLength, 0);
        ofDrawLine(0, 0, 0, lightLength);
        ofDrawLine(0, 0, -lightLength, 0);
        ofDrawLine(0, 0, 0, -lightLength);
    }
    ofDrawRectangle(0, 0, 20, 20);
    ofPopStyle();
    ofPopMatrix();
}

void Crystal::update() {
    rotation += speed;
}