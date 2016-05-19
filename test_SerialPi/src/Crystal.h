//
//  Crystal.hp
//  spinningLights
//
// This is going to be our motor controller
// It should take a single parameter from the optical flow algorithm, a single float between -5 and 5.
// It sets it's own speed and maintains its own internal speed which will change as noise over time.
// TBD should this have an idea of where it sits ont he circle? Perhaps each should have a static ID?
// Created by James Bentley on 5/9/16.
//
//

#ifndef Crystal_h
#define Crystal_h

#include "ofMain.h"

class Crystal {
public:
    //Constructor
    Crystal();
    
    //Setters
    void setId(int _id) { id = _id; };
    void setPinId(char _pinId) { pinId = _pinId; };
    void setSpeed(float _speed) { baseSpeed = _speed; };
    void setRotation(float _rotation) { rotation = _rotation; };
    void setSerial(ofSerial* _serial) { serial = _serial; };
    
    //Getters
    int getId() { return id; };
    char getPinId() { return pinId; };
    float getBaseSpeed() { return baseSpeed; };
    float getRotation() { return rotation; };
    ofSerial* getSerial() { return serial; };
    
    //Functionality
    void draw();
    void update(float flow);

private:
    //Fields
    int id;
    char pinId;
    float baseSpeed;
    float noiseStart;
    float noiseStep;
    float rotation;
    ofSerial* serial;

    //Functionality
    void send(float _speed);
};

#endif /* crystal_h */