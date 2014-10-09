
#pragma once

#include "ofMain.h"
#include "ofxDmx.h"

class light {
    int addressStart;
    int addressType;
};

class lightController {
    
public:
    ofImage stage;
    
    void setup();
    void update();
    void draw();
    
    void setColor(int location, ofPoint color);
    void setColor(int location, ofColor color);
    
    light stageLights[12];
    light platformLights[3];
    light tankLights[2];

	ofxDmx dmx;
};