#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    float theta = 90;
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        
        crystals[i].setLocation(ofVec2f(circleCenter.x + circleRadius * cos(theta), circleCenter.y + circleRadius * sin(theta)));
        crystals[i].setInitialSpeed(ofRandom(-0.02, 0.02));
        crystals[i].setRotation(ofRandom(0.0, 360.0));

        lights[i].setLocation(ofVec2f(circleCenter.x + (circleRadius + 50) * cos(theta), circleCenter.y + (circleRadius + 50) * sin(theta)));
        lights[i].setLookAt(circleCenter);
        lights[i].setIndex(i);
        
        theta += (2 * PI)/NUM_CRYSTALS;
        
        crystals[i].setLight(&lights[i]);
        lights[i].setCrystal(&crystals[i]);
    }
    gui.setup("settings");
    
    gui.add(flow.set("Optical Flow", 0.0, -1.0, 1.0));
    gui.add(flowDownSpeed.set("Flow Down Speed", 0.001, 0.001, 0.1));
    gui.add(flowUpSpeed.set("Flow Up Speed", 0.1, 0.1, 1.0));
    gui.add(flowEffect.set("Flow Effect", 0.5, 0.0, 1.0));
#ifdef PROP_WEIGHTED_LERP
    gui.add(propagationFactor.set("Propagation Factor", 0.1, 0.5, 1.0));
#endif

    gui.loadFromFile("settings.xml");
    
    flow.set("Optical Flow", 0.0, -3.0, 3.0);
    
    camera.setup(320, 240);
    
    circleMask.load("shaders/mask.vert", "shaders/mask.frag");
    
    buffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    buffer.begin();
    ofClear(0);
    buffer.end();
    ofBackground(127);
    
#ifdef PROP_DELAY
    for(int i = 0; i < 60; i++) {
        flowHistory.push_back(0);
    }
#endif
    
#ifdef PROP_LOCATION
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        flowsFromLeftToRight[i] = 0.0f;
    }
#endif

}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[i].update();
        //crystals[i].setSpeed(ofLerp(crystals[i].getSpeed(), crystals[i].getInitialSpeed() + flow * flowEffect, 0.1));
    }
    propagateImpulse(flow);
    //flow = ofLerp(flow, 0.0, flowResetSpeed);
    
    camera.update();
    if(camera.isFrameNew()) {
        flowFb.calcOpticalFlow(camera);
#ifdef PROP_LOCATION
        for(int i = 0; i < NUM_CRYSTALS; i++) {
            float width = camera.getWidth() / NUM_CRYSTALS;
            float x = i * width;
            if(abs(flowFb.getAverageFlowInRegion(ofRectangle(x, 0, width, 240)).x) > abs(flowsFromLeftToRight[i])) {
                flowsFromLeftToRight[i] = ofLerp(flowsFromLeftToRight[i], flowFb.getAverageFlowInRegion(ofRectangle(x, 0, width, 240)).x, flowUpSpeed);
            }
            else {
                flowsFromLeftToRight[i] = ofLerp(flowsFromLeftToRight[i], 0.0, flowDownSpeed);
            }
        }
    }
#else
        if(abs(flowFb.getAverageFlowInRegion(ofRectangle(0, 0, 320, 240)).x) > abs(flow)) {
            flow = ofLerp(flow, flowFb.getAverageFlowInRegion(ofRectangle(0, 0, 320, 240)).x, flowUpSpeed);
        }
        else {
            flow = ofLerp(flow, 0.0, flowDownSpeed);
        }
    }
#endif
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

    circleMask.begin();
    circleMask.setUniform1f("radius", circleRadius-1);
    circleMask.setUniform2f("size", ofGetWidth(), ofGetHeight());
    buffer.draw(0, 0);
    circleMask.end();
    
    camera.draw(ofGetWidth() - camera.getWidth(), 0);
    
    flowFb.draw(ofGetWidth() - camera.getWidth(), 0, 320, 240);
        
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::propagateImpulse(float flow) {
    // Ordered from left to right the lights go:
    // 1, 2, 0, 3, 6, 4, 5
    // So if we want to change the speed and propagate the impulse accross the face of the piece then we'll need to
    // save a history of the speeds, either that OR we just lerp up at different rates depending upon the direction
    // We could also hard-code a delay in.
    
    int valuesFromLeftToRight[NUM_CRYSTALS];
    valuesFromLeftToRight[0] = 1;
    valuesFromLeftToRight[1] = 2;
    valuesFromLeftToRight[2] = 0;
    valuesFromLeftToRight[3] = 3;
    valuesFromLeftToRight[4] = 6;
    valuesFromLeftToRight[5] = 4;
    valuesFromLeftToRight[6] = 5;

    
    //We'll try with different lerp up speeds and see how we go!
#ifdef PROP_WEIGHTED_LERP
    if(flow < 0) {
        float rate = 0.1;
        for(int i = 0; i < NUM_CRYSTALS; i++) {
            crystals[valuesFromLeftToRight[i]].setSpeed(ofLerp(crystals[valuesFromLeftToRight[i]].getSpeed(), crystals[valuesFromLeftToRight[i]].getInitialSpeed() + flow * flowEffect, rate));
            rate *= propagationFactor;
        }
    } else if(flow > 0) {
        float rate = 0.1;
        for(int i = NUM_CRYSTALS-1; i > -1; i--) {
            crystals[valuesFromLeftToRight[i]].setSpeed(ofLerp(crystals[valuesFromLeftToRight[i]].getSpeed(), crystals[valuesFromLeftToRight[i]].getInitialSpeed() + flow * flowEffect, rate));
            rate *= propagationFactor;
        }
    }
#endif
    // This works really well actually...
    // will try a history option here:
    
#ifdef PROP_DELAY
    flowHistory.pop_back();
    flowHistory.push_front(flow);
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        int flowValue;
        if(flow < 0)
            flowValue = (int)ofMap(i, 0, NUM_CRYSTALS, 0, flowHistory.size());
        else
            flowValue = (int)ofMap(i, 0, NUM_CRYSTALS, flowHistory.size(), 0);
        crystals[valuesFromLeftToRight[i]].setSpeed(ofLerp(crystals[valuesFromLeftToRight[i]].getSpeed(), crystals[valuesFromLeftToRight[i]].getInitialSpeed() + (flowHistory[flowValue]) * flowEffect, 0.1));
    }
#endif
    // I reckon this one works even better honestly. Really smooth and comes from the correct side, can change the delay duration by changing the length of the buffer.
    
    //Will try different sections of the screen controlling individual motors next!
    
#ifdef PROP_LOCATION
    for(int i = 0; i < NUM_CRYSTALS; i++) {
        crystals[valuesFromLeftToRight[i]].setSpeed(ofLerp(crystals[valuesFromLeftToRight[i]].getSpeed(), crystals[valuesFromLeftToRight[i]].getInitialSpeed() + flowsFromLeftToRight[NUM_CRYSTALS - 1 - i] * flowEffect, 0.1));
    }
#endif
    
    // This also works very well! Maybe the best! Still just left to right for now, Joel wants it top to bottom as well to do this I'll need to make an ordering algorithm based on the vector. I can also orient the region? can you do slanted regions? I don't think so...
    // Maybe we just do a top to bottom propagation jobby? I guess what I really need to do is algorithmically determine the order of the points. There are several options:
    // Stupid way, start at x = 0, y = 0 and step along the vector until you get past a point, you know you're past a point when you distance to the point increases instead of decreases. Need to do this every frame
    // Alternative, determine all orderings and angles that create those orderings and boolean test your angles against those arcs, shouldn't be too hard???
    // Let's try the stepping one, actually this is shitty because if we stand straight up and down we'll get nothing.
    // Basically our order will change very slightly, but I don't think that's even worth much... We only have one dimensions worth of variables, not 2!!
    // Our alternative could be moving along a line through the object and "pushing" the lights out of the way, so left and right motion is not simply clock-wise or counter-clockwise
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
