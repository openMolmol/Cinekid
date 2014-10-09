
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

#pragma once 

class fanManager {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void allOnChanged( );
    
    int whichDevice;
    ofSerial serial;
    vector <ofSerialDeviceInfo> devices;
    ofArduino arduino;
    ofxXmlSettings xml;
    vector < ofParameter <bool> > relayPins;
    vector < int > pinIds;
    vector < bool > bFlipMes;
    
    void setStrength(float strength);
    
    
    void setStrength( int person, float strength );

    void clearFanEnergy();
    void computeFanEnergy();
    float energyPerFan[12];
    
    
    ofxToggle allOn;
    ofxPanel panel;

};