//
//  Crystal.hpp
//  spinningLights
//
// This is our dichroic light crystal simulator, it will very simply just spin and draw lines when a line to pointed at it.
//  Created by James Bentley on 5/9/16.
//
//

#ifndef Crystal_hpp
#define Crystal_hpp

#include "ofMain.h"

class Light;

class Crystal {
public:
    //Constructor
    Crystal();
    
    //Setters
    void setLocation(ofVec2f _location) { location = _location; };
    void setSpeed(float _speed) { speed = _speed; };
    void setRotation(float _rotation) { rotation = _rotation; };
    void setLight(Light* _light) { light = _light; };
    void setInitialSpeed(float _speed) { initialSpeed = _speed; };
    
    //Getters
    ofVec2f getLocation() { return location; };
    float getSpeed() { return speed; };
    float getRotation() { return rotation; };
    Light* getLight() { return light; };
    float getInitialSpeed() { return initialSpeed; };
    
    //Functionality
    void draw();
    void update();
    
private:
    ofVec2f location;
    float speed;
    float rotation;
    Light* light;
    float initialSpeed;
};

#endif /* crystal_hpp */
