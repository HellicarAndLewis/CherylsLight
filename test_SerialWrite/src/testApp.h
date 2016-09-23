//RPI VERSION

#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    ofSerial serial;
    
    bool firstContact;
    
    int serialIntArray[3];
    
    int inByte;
    
    int serialCount;
    
    unsigned char buffer[3];
};

