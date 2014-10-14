
#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"
#include "ofxBlur.h"



class light {
    int addressStart;
    int addressType;
};



class player {
public:
    float playerEnergy;
    float playerEnergySmooth;
    int playerDirection;
    bool bFlippedAlread;
    ofPoint playerPos;
    ofColor baseColor;
    float playerEnergySlowRate;
    float playerEnergySmoothRate;
    float energyAddAmount;
    float minRectWidth;
    float maxRectWidth;
    
    void drawRectangleInFbo( ofRectangle rect, ofColor color);
    
    void setup(){
        
        playerEnergySmooth = 0;
        playerEnergy = 0;
        playerDirection = ofRandom(0,1) > 0.5 ? -1 : 1;
        bFlippedAlread = false;
        
        
    }
    
    ofColor getColor(){
        ofColor temp = baseColor;
        temp.a = (ofMap(playerEnergySmooth, 0, 2.0, 0, 1, true) * 255);
        //cout << (int)temp.a << " " << playerEnergySmooth << endl;
        return temp;
    }
    
    void addEnergy(){
        playerEnergy += energyAddAmount;
    }
    
    void addEnergy( float amount ){
        playerEnergy += amount;
    }
    
    void draw(){
        float w = 0;
        w = ofMap(playerEnergySmooth, 0, 6, minRectWidth, maxRectWidth, true);
        drawRectangle( ofRectangle(playerPos.x-w/2.0,0,w,30),  getColor());
    }
    
    void drawRectangle( ofRectangle rect, ofColor color){
        //   color.a = 255;
        //    cout << "?? " << color << " " << rect <<  endl;
        ofSetColor( color);
        ofRect(rect);
        rect.translate(-120, 0);
        ofRect(rect);
        rect.translate(2*120, 0);
        ofRect(rect);
    }
    
    void update(){
        
        playerEnergySmooth =   playerEnergySmoothRate * playerEnergySmooth + (1-playerEnergySmoothRate) * playerEnergy;
        
        playerEnergy *= playerEnergySlowRate;
        playerPos += playerEnergy * playerDirection;
        
        if (playerEnergySmooth < 0.1){
            if (bFlippedAlread == false){
                playerDirection = -1 * playerDirection;
                bFlippedAlread = true;
            }
        } else {
            bFlippedAlread = false;
        }
        
        while (playerPos.x > 120){
            playerPos.x  -= 120;
        }
        
        while (playerPos.x < 0){
            playerPos.x  += 120;
        }
        
        
    }
};

class lightController {
    
public:
    //ofImage stage;
    
    void setup();
    void update();
    void draw();
    
    void fireC02(){
        bOnA = true;
        startTimeA = ofGetElapsedTimeMillis();
    }
    
    void addEnergy (int player, float energy);
    
    
    void setColor(int location, ofPoint color);
    void setColor(int location, ofColor color);
    
    light stageLights[12];
    light platformLights[3];
    light tankLights[2];

	ofxDmx dmx;
    
    int startTimeA;
    int durationA;
    bool bOnA;
    
    //---------------------
    
    ofPoint colors[12];
    void drawRectangleInFbo( ofRectangle rect, ofColor color);
    
    ofxPanel panel;
    
    ofParameter < float > playerEnergySlowRate;
    ofParameter < float > playerEnergySmoothRate;
    ofParameter < float > energyAddAmount;
    ofParameter < float > minRectWidth;
    ofParameter < float > maxRectWidth;
    ofParameter < float > fadeToBlackRate;

    ofParameter < float > energyAddScaleAmount;
    ofParameter < float > energyAddShapeAmount;

    
    
    ofParameter < ofPoint > color1;
    ofParameter < ofPoint > color2;
    ofParameter < ofPoint > color3;
    
    
    float screenSaverEnergy;
    float screenSaverEnergyTarget;
    
    void setScreenSaver(bool bScr){
        if (bScr == true) screenSaverEnergyTarget = 1;
        else screenSaverEnergyTarget = 0;
    }
    
    
    ofxBlur blur;
    
    
    player p[3];

    ofFbo lightStrip;
};