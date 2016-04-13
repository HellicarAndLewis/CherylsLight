#include "ofApp.h"

string drawTypeArray[] = {"Plane", "Circular", "Spring", "Disc", "Disc from inside", "Plane from inside"};

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Sonic Sculpture - 's' to save - 'd' for debug");
    ofSetVerticalSync(true);
    ofSetFrameRate(60); ///lets only sample 30 times a second instead of 60 - too much data otherwise
    
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    ofSetSmoothLighting(true);
    
    light.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    light.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    light.setPointLight();
    
    shapes.setup();

    mainGui.setup();
    mainGui.setName("Parameters");
    mainGui.loadFont("Avenir Light.otf", 10);

    mainGui.add(shapes.getParameters());
    mainGui.minimizeAll();
    mainGui.loadFromFile("settings.xml");
    
    
    // Sound player settings
    // Uncomment following to reseed song shape
    // soundPlayer.load("MovinOnUp.mp3");
    // soundPlayer.play();

    
    // For debugging purpose
    // soundPlayer.setSpeed(16.0f);
    // soundPlayer.setVolume(0.01f);
    
    shapes.setPauseMesh(false);
    shapes.setColorScheme(5);
    
    
    // Comment following to reseed song shape
    shapes.update();
    shapes.draw();
    shapes.buildShapeOnce();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    // Uncomment following to reseed song shape
    
    
//    shapes.update();
    // Stop playing and drawing when music is finished
//    if (soundPlayer.getPosition() > 0.99f && soundPlayer.isPlaying()){
//        cout<<"stopped"<<endl;
//        soundPlayer.setVolume(0.0f);
//        soundPlayer.stop();
//        soundPlayer.setPosition(0.0f);
//        shapes.buildShapeOnce();
//        shapes.draw();
//        shapes.setPauseMesh(true);
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(100), ofColor::white);
    
    ofPushMatrix();
    ofSetColor(255);
    shapes.draw();
    ofPopMatrix();
    
    mainGui.draw();
    
    ofSetColor(255);
    string msg = ofToString((int) ofGetFrameRate()) + " fps";
    ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
    
    //Cursor showing bug right now..
    ofShowCursor();
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 5);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='r'){
        shapes.reset();
    }
    if(key=='s'){
        shapes.saveToFile();
    }
    if(key=='p') shapes.setPauseMesh(!shapes.getPauseMesh());
    if(key=='d'){
        shapes.setShowDebug(!shapes.getShowDebug());
    };
    
    if (key=='j') {
        mainGui.saveToFile("settings.xml");
    }
    
    if (key=='l') {
        // Nice to have but not working yet
        // auto settings = ofSystemLoadDialog("Please choose the settings file to load...");
        mainGui.loadFromFile("settings.xml");
    }
    
    // Reset everything and start again
    if(key=='h'){
//        shapes.clearSpectrums();
//        soundPlayer.setPosition(0.0f);
//        shapes.setPauseMesh(true);
//        shapes.reset();
//        soundPlayer.play();
//        shapes.setPauseMesh(false);
//        shapes.setColorScheme(5);
    }
    // Build shape once
    if(key=='b'){
        mainGui.loadFromFile("settings.xml");
        shapes.setPauseMesh(true);
        shapes.reset();
        shapes.setPauseMesh(false);
        shapes.buildShapeOnce();
        shapes.draw();
    }
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
