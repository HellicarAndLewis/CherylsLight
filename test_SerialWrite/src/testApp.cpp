//RPI VERSION

#include "testApp.h"

void testApp::setup() {
    
    firstContact = false;
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    for(int i = 0; i < deviceList.size(); i++) {
        cout<<deviceList[i].getDevicePath()<<endl;
    }
    
    int baud = 9600;
    serial.setup(0, baud);
    
    serialCount = 0;
}

void testApp::update(){
    if(serial.available() <= 0) {
//        unsigned char myByte = '9';
//        bool byteWasWritten = serial.writeByte(myByte);
//        if ( !byteWasWritten )
//            printf("byte was not written to serial port\n");
//        else
//            printf("byte was written %d\n", myByte);
    }
    else {
        inByte = serial.readBytes(buffer, 3);
        if(!firstContact) {
            if(inByte == 1) {
                serial.flush();
                firstContact = true;
                serial.writeByte('A');
                cout<<"Sending Initial Serial"<<endl;
            }
        } else if(inByte == 3) {
            serialIntArray[0] = buffer[0];
            serialIntArray[1] = buffer[1];
            serialIntArray[2] = buffer[2];
            
            for(int i = 0; i < serialCount; i++) {
                cout<<serialIntArray[i]<<endl;
            }
        }
    }
}

void testApp::draw(){
    ofSetColor(0);
    for(int i = 0; i < 3; i++) {
        ofDrawBitmapString(ofToString(serialIntArray[i]), 10, 10 + i * 10);
    }
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 0, ofGetHeight() - 20);
}

void testApp::keyPressed(int key) {
    if(key == 'a') {
        serial.writeByte('A');
        cout<<"Sending Serial at Time: "<<ofGetElapsedTimef()<<endl;
    } else if(key == 'b') {
        serial.writeByte('B');
        cout<<"Sending Serial at Time: "<<ofGetElapsedTimef()<<endl;
    }

}