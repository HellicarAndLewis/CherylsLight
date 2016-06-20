//
//  Crystal.cpp
//  spinningLights
//
//  Created by James Bentley on 5/9/16.
//
//

#include "Crystal.h"

Crystal::Crystal() {
    baseSpeed = 0.0f;
    rotation = 0.0f;
    noiseStart = ofRandom(0, 10000);
    noiseStart = 0.01;
}

void Crystal::draw() {
    // ofPushMatrix();
    // ofPushStyle();
    // ofSetColor(0);
    // ofNoFill();

    // ofSetRectMode(OF_RECTMODE_CENTER);
    // ofTranslate(location.x, location.y);
    // ofRotate(rotation, 0, 0, 1);
    // float lightLength = 1000;
    // for(int i = 0; i < 4; i++) {
    //     ofDrawLine(0, 0, lightLength, 0);
    //     ofDrawLine(0, 0, 0, lightLength);
    //     ofDrawLine(0, 0, -lightLength, 0);
    //     ofDrawLine(0, 0, 0, -lightLength);
    // }
    // ofDrawRectangle(0, 0, 20, 20);
    // ofPopStyle();
    // ofPopMatrix();
}

void Crystal::update(float flow) {
    float speed;
    speed = baseSpeed;
    speed += ofMap(flow, -5, 5, -1, 1);
    //baseSpeed += ofMap(ofNoise(noiseStart + noiseStep), 0, 1, -0.01, 0.01);
    rotation += speed;
    send(speed);
}

void Crystal::send(float _speed) {
    if(abs(_speed) > 0.5) {
        if(_speed > 0) {
           serial->writeByte('c');
           serial->writeByte('1');
        } else {
           serial->writeByte('c');
           serial->writeByte('0');
        }
    }
}