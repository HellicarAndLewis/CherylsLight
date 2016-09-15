//
//  Light.cpp
//  spinningLights
//
// These are the lights which we'll point at the crystals
//
//  Created by James Bentley on 5/9/16.
//
//

#include "Light.h"
#include "Crystal.h"

Light::Light() {
    location = ofVec2f(0, 0);
    lookAt = ofVec2f(0, 0);
}


void Light::draw() {
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(0);
    ofNoFill();
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    float deltaX = location.x - lookAt.x;
    float deltaY = location.y - lookAt.y;
    
    float angle = atan(deltaY / deltaX) * 180 / PI;
    
    //ofDrawLine(location.x, location.y, lookAt.x, lookAt.y);
    
    ofVec2f crystalLocation = crystal->getLocation();
    if(isCollinear(crystalLocation)) {
        ofDrawLine(location.x, location.y, crystalLocation.x, crystalLocation.y);
    } else {
        ofDrawLine(location.x, location.y, lookAt.x, lookAt.y);
    }
    
    ofDrawBitmapString( ofToString(index), location.x, location.y);

    ofTranslate(location.x, location.y);
    ofRotate(angle, 0, 0, 1);
    ofDrawRectangle(0, 0, 30, 20);
    
    
    ofPopStyle();
    ofPopMatrix();
}

bool Light::isCollinear(ofVec2f point) {
    float slopeToLookAt = (location.y - lookAt.y) / (location.x - lookAt.x);
    float slopeToPoint = (location.y - point.y) / (location.x - point.x);
    
    float tolerance = 0.01;
    if(slopeToPoint < (slopeToLookAt + tolerance) && slopeToPoint > (slopeToLookAt - tolerance)) {
        return true;
    }
    return false;
}