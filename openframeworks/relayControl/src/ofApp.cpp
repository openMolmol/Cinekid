#include "ofApp.h"


//#define Relay_1  5  // Wind1
//#define Relay_2  6   //speed1
//#define Relay_3  7   //fog machine***
//#define Relay_4  8   //wind2
//#define Relay_5  9   // speed2
//#define Relay_6  10  // wind3
//#define Relay_7  11  // speed3
//#define Relay_8  52  // wind4
//#define Relay_9  4  // speed4

#define Relay_1  5  // Wind1
#define Relay_2  6   //speed1
#define Relay_3  7   //fog machine***
#define Relay_4  8   //wind2
#define Relay_5  9   // speed2
#define Relay_6  10  // wind3
#define Relay_7  11  // speed3
#define Relay_8  12  // wind4
#define Relay_9  4  // speed4


//--------------------------------------------------------------
void ofApp::setup(){
    
    serial.listDevices();
    devices = serial.getDeviceList();
    xml.load("relay.xml");
    string deviceToMatch = xml.getValue("relayControl:deviceId", "");
    
    receiver.setup(12345);
    
    whichDevice= 0;
    for (int i= 0;  i < devices.size(); i++){
        int howMany = ofStringTimesInString(devices[i].getDeviceName(), deviceToMatch);
        if (howMany > 0){
            whichDevice = i;
        }
    }
    
    
    arduino.connect(devices[whichDevice].getDeviceName());
    
    xml.pushTag("relayControl");
    xml.pushTag("pins");
    cout << xml.getNumTags("pin") << endl;
    
    
    panel.setup("pin control");
    
    
    for (int i = 0; i < xml.getNumTags("pin"); i++){
        xml.pushTag("pin", i);
        ///cout << xml.getValue("num", 0) << endl;
        ///cout << xml.getValue("flipOnOff", "true") << endl;
        ofParameter <bool> pin;
        relayPins.push_back(pin);
        pinIds.push_back(xml.getValue("num", 0));
        bFlipMes.push_back(xml.getValue("flipOnOff", "true") == "true" ? true : false);
        bool bDefaultState = xml.getValue("defaultState", "true") == "true" ? true : false;
        
        panel.add(relayPins[relayPins.size()-1].set("fan id " + ofToString(i) + " pin " + ofToString(xml.getValue("num", 0)), bDefaultState));
                arduino.sendDigitalPinMode(pinIds[i],ARD_OUTPUT);
        
        xml.popTag();
        
        
        
    }
    panel.add(allOn.setup("all on", false));
    panel.add(useOsc.setup("use osc", false));
    

}




bool bAllOnLastFrame = false;
//--------------------------------------------------------------
void ofApp::update(){

    
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
		receiver.getNextMessage(&m);
        if(m.getAddress() == "/strength");
        float strength = m.getArgAsFloat(0);
        
        if (useOsc == true){
            for (int i = 0; i < relayPins.size(); i++){
                if (i < strength * relayPins.size()){
                    relayPins[i] = true;
                } else {
                    relayPins[i] = false;
                }
            }
        }
    }
    
    
    
    //cout << allOn << endl;
    
    if (allOn != bAllOnLastFrame){
        for (int i = 0; i < relayPins.size(); i++){
            relayPins[i] = allOn;
        }
    }
    
    bAllOnLastFrame = allOn;
    
    
    if (arduino.isArduinoReady()){
//    for (int i = 0; i < 60; i++){
//        bool bAmOne = false;
//        for (int j = 0; j < 4; j++){
//            if (i != relayPins[j]) bAmOne = true;
//        }
//        
//        if (!bAmOne) arduino.sendDigital(pinIds[i], 0);
//    
//    }
        
        
//        arduino.sendDigital(Relay_1, 0);
//        arduino.sendDigital(Relay_2, 1);
//        arduino.sendDigital(Relay_4, 0);
//        arduino.sendDigital(Relay_5, 1);
//        arduino.sendDigital(Relay_6, 0);
//        arduino.sendDigital(Relay_7, 1);
//        arduino.sendDigital(Relay_8, 0);
//        arduino.sendDigital(Relay_9, 1);
        
//        arduino.sendDigital(6, 1);
//        arduino.sendDigital(9, 1);
//        arduino.sendDigital(11, 1);
//        arduino.sendDigital(4, 1);
//    
    
    for (int i = 0; i < relayPins.size(); i++){
        bool bOn = relayPins[i];
        if (bFlipMes[i] == true) bOn = !bOn;
        if (bOn){
            arduino.sendDigital(pinIds[i], 1);
        }else {
            arduino.sendDigital(pinIds[i], 0);
        }
    }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    for (int i = 0; i < devices.size(); i++){
        if (i == whichDevice){
            ofDrawBitmapStringHighlight("device: " + devices[i].getDeviceName(), 500, 20 + i * 20, ofColor::teal, ofColor::white);
        } else {
            ofDrawBitmapStringHighlight("device: " + devices[i].getDeviceName(), 500, 20 + i * 20);
        }
    }
    
    panel.draw();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
