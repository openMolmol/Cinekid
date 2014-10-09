//
//  fanManager.cpp
//  cinekidRelayControl
//
//  Created by cinekid on 10/9/14.
//
//

#include "fanManager.h"
#include "ofApp.h"


/*
 ofColor aT =ofColor::green;
 ofColor bT =ofColor::red;
 ofColor cT =ofColor::blue;
 */

int order0[] = {11,10,12,6,7,5,8,9,4,2,1,3};
int order1[] = {1,2,4,3,5,6,7,10,8,9,11,12};
int order2[] = {3,2,9,8,1,6,12,11,4,5,11,10};


bool bAllOnLastFrame = false;

void fanManager::setup(){
    
    serial.listDevices();
    devices = serial.getDeviceList();
    xml.load("relaySettings.xml");
    string deviceToMatch = xml.getValue("relayControl:deviceId", "");
    
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
    
    
    for (int i = 0; i < xml.getNumTags("pin"); i++){
        xml.pushTag("pin", i);
        ///cout << xml.getValue("num", 0) << endl;
        ///cout << xml.getValue("flipOnOff", "true") << endl;
        ofParameter <bool> pin;
        relayPins.push_back(pin);
        pinIds.push_back(xml.getValue("num", 0));
        bFlipMes.push_back(xml.getValue("flipOnOff", "true") == "true" ? true : false);
        bool bDefaultState = xml.getValue("defaultState", "true") == "true" ? true : false;
        
        ((ofApp*)ofGetAppPtr())->panel.add(relayPins[relayPins.size()-1].set("fan id " + ofToString(i) + " pin " + ofToString(xml.getValue("num", 0)), bDefaultState));
        arduino.sendDigitalPinMode(pinIds[i],ARD_OUTPUT);
        
        xml.popTag();
        
        
        
    }
    
    for (int i= 0; i < 12; i++){
        energyPerFan[i] = 0;
    }
}


void fanManager::setStrength( int person, float strength ){
    
    strength = powf(strength, 1.6);
    for (int i = 0; i < 12; i++){
        if (i < (int)(strength * 12)){
            
            if (person == 0) energyPerFan[order0[i]] = 1.0;
            if (person == 1) energyPerFan[order1[i]] = 1.0;
            if (person == 2) energyPerFan[order2[i]] = 1.0;
            
            //relayPins[i] = true;
        } else {
            
            ;//
            //relayPins[i] = false;
        }
    }
    
}



void fanManager::setStrength(float strength){
    for (int i = 0; i < relayPins.size(); i++){
        if (i < (int)(strength * relayPins.size())){
            relayPins[i] = true;
        } else {
            relayPins[i] = false;
        }
    }
}


void fanManager::computeFanEnergy(){
    
//    for (int i= 0; i < 12; i++){
//        energyPerFan[i] *= 0.96;
//        
//        cout << i << " --- " << energyPerFan[i] << endl;
//        if (energyPerFan[i] > 0.2){
//            relayPins[i] = true;
//        } else {
//            relayPins[i] = false;
//        }
//    }
}



void fanManager::update(){
    
    arduino.update();
    
    //cout << arduino.isArduinoReady() << endl;
    
    if (ofGetFrameNum() > 60*4){
        
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
//    
//    if (((ofApp*)ofGetAppPtr())->allOn != bAllOnLastFrame){
//        for (int i = 0; i < relayPins.size(); i++){
//            relayPins[i] = ((ofApp*)ofGetAppPtr())->allOn;
//        }
//    }
//    
//    bAllOnLastFrame = ((ofApp*)ofGetAppPtr())->allOn;
    
    
}

void fanManager::draw(){
    
    ofDrawBitmapStringHighlight("fan connected on: " + devices[whichDevice].getDeviceName(), 20, 20, ofColor::teal, ofColor::white);
    
    
//    for (int i = 0; i < devices.size(); i++){
//        if (i == whichDevice){
//            ofDrawBitmapStringHighlight("device: " + devices[i].getDeviceName(), 500, 20 + i * 20, ofColor::teal, ofColor::white);
//        } else {
//            ofDrawBitmapStringHighlight("device: " + devices[i].getDeviceName(), 500, 20 + i * 20);
//        }
//    }
}
