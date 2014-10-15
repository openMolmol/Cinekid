//
//  lightController.cpp
//  cinekidRelayControl
//
//  Created by cinekid on 10/8/14.
//
//

#include "lightController.h"


void lightController::setup(){
    dmx.connect("tty.usbserial-EN149955", 512); // use the name
    
    startTimeA = 0;
    durationA = 100;
    bOnA = false;
    
    
    
    lightStrip.allocate(120, 10, GL_RGBA32F);
    
    
    
    lightStrip.begin();
    ofClear(0,0,0, 255);
    lightStrip.end();
    
    //   playerPos = 0;
    
    p[0].setup();
    p[0].baseColor = ofColor::green;
    
    p[1].setup();
    p[1].baseColor = ofColor::orange;
    
    
    panel.setup();
    panel.add(playerEnergySlowRate.set("playerEnergySlowRate", 0.97, 0.8, 1.0));
    panel.add(playerEnergySmoothRate.set("playerEnergySmoothRate", 0.93, 0.8, 1.0));
    panel.add(energyAddAmount.set("energyAddAmount", 5.0, 0.0, 20.0));
    panel.add(minRectWidth.set("minRectWidth", 4,0, 100));
    panel.add(maxRectWidth.set("maxRectWidth", 40.0, 0.0, 100.0));
    panel.add(color1.set("color1", ofPoint(0,0,0), ofPoint(0,0,0), ofPoint(255,255,255)));
    panel.add(color2.set("color2", ofPoint(0,0,0), ofPoint(0,0,0), ofPoint(255,255,255)));
    panel.add(color3.set("color3", ofPoint(0,0,0), ofPoint(0,0,0), ofPoint(255,255,255)));
    
    color1.set(ofPoint( ofColor::orange.r,ofColor::orange.g,ofColor::orange.b));
    color2.set(ofPoint( ofColor::red.r,ofColor::red.g,ofColor::red.b));
    color3.set(ofPoint( ofColor::aqua.r,ofColor::aqua.g,ofColor::aqua.b));
    
    panel.loadFromFile("colorSettings.xml");


}
void lightController::setColor(int location, ofColor color){
    int i = location;
    dmx.setLevel(1 + i * 5, 255);
    dmx.setLevel(2+ i * 5, color.r);
    dmx.setLevel(3+ i * 5, color.g);
    dmx.setLevel(4+ i * 5, color.b);
    
}

void lightController::drawRectangleInFbo( ofRectangle rect, ofColor color){
    //   color.a = 255;
    //    cout << "?? " << color << " " << rect <<  endl;
    ofSetColor( color);
    ofRect(rect);
    rect.translate(-120, 0);
    ofRect(rect);
    rect.translate(2*120, 0);
    ofRect(rect);
}


void lightController::setColor(int location, ofPoint color){
    int i = location;
    dmx.setLevel(1 + i * 5, 255);
    dmx.setLevel(2+ i * 5, color.x);
    dmx.setLevel(3+ i * 5, color.y);
    dmx.setLevel(4+ i * 5, color.z);
}
void lightController::update(){

    
    p[0].baseColor.set( color1->x, color1->y, color1->z);
    p[1].baseColor.set( color2->x, color2->y, color2->z);
    
    for (int i = 0; i < 2; i++){
        p[i].update();
        p[i].playerEnergySmoothRate = playerEnergySmoothRate;
        p[i].playerEnergySlowRate = playerEnergySlowRate;
        p[i].energyAddAmount = energyAddAmount;
        p[i].minRectWidth = minRectWidth;
        p[i].maxRectWidth = maxRectWidth;
    }

    lightStrip.begin();
    ofEnableAlphaBlending();
    glColor4f(0,0,0, 0.1);
    ofFill();
    ofRect(0,0,120,10);
    //ofClear(0,0,0,1);
    ofSetColor( ofColor::white);
    
    ofDisableAlphaBlending();
    ofEnableAlphaBlending();
    
    p[0].draw();
    p[1].draw();
    
    if (p[0].playerEnergySmooth > 4.0 || p[1].playerEnergySmooth > 4.0){
        drawRectangleInFbo( ofRectangle( ofRandom(0,120), 0, ofRandom(10,20), 30), ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255)));
    }
    
    
    
    ofEnableAlphaBlending();
    lightStrip.end();
    
    
    ofPixels pix;
    lightStrip.readToPixels(pix);
    
    
    
    int order[] = { 0,1,2,3,5,4,11,10,9,6,8,7 };
    
    for (int i = 0; i < 12; i++){
        
        ofPoint colorMix;
        for (int j = i * 10; j < (i+1) * 10; j++){
            for (int k = 0; k < 10; k++){
                
                ofColor c = pix.getColor(j,k);
                colorMix.x += c.r;
                colorMix.y += c.g;
                colorMix.z += c.b;
                
            }
        }
        colorMix /= 100.0;
        
        setColor(order[i] ,  colorMix);
        
    }
    
    
    ofNoFill();
    
    
    if (bOnA){
        
        dmx.setLevel(127, 255);
        dmx.setLevel(128, 255);
        dmx.setLevel(129, 255);
    } else {
        dmx.setLevel(127, 0);
        dmx.setLevel(128, 0);
        dmx.setLevel(129, 0);
    }

    
    if (bOnA == true){
        
        if ( (ofGetElapsedTimeMillis() - startTimeA) > durationA){
            
            bOnA = false;
        }
    }
    
    dmx.update();
    
}

void lightController::draw(){
    
    
    
    ofDisableAlphaBlending();
    ofSetColor(255);
    lightStrip.draw(0,ofGetHeight()-50, ofGetWidth(),  (lightStrip.getHeight() / lightStrip.getWidth()) * ofGetWidth());
    ofEnableAlphaBlending();
    
    panel.draw();
    //stage.draw(0,300);
}