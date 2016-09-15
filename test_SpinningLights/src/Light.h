//
//  Light.hpp
//  spinningLights
//
//  Created by James Bentley on 5/9/16.
//
//

#ifndef Light_h
#define Light_h

#include "ofMain.h"

class Crystal;

class Light {
public:
    //Constructor
    Light();
    
    //Setters
    void setLocation(ofVec2f _location) { location = _location; };
    void setLookAt(ofVec2f _lookAt) { lookAt = _lookAt; };
    void setCrystal(Crystal* _crystal) { crystal = _crystal; };
    void setIndex(int _index) { index = _index; };
    
    //Getters
    ofVec2f getLocation() { return location; };
    ofVec2f getLookAt() { return lookAt; };
    Crystal* getCrystal() { return crystal; };
    
    //Functionality
    void draw();
    
private:
    ofVec2f location;
    ofVec2f lookAt;
    Crystal* crystal;
    int index;
    
    //Functionality
    bool isCollinear(ofVec2f point);
};

#endif /* Light_hpp */
