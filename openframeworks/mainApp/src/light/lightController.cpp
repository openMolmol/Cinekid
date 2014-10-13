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
    
    //int radius = 32, float shape = .2, int passes = 1, float downsample = .5
    blur.setup(120, 10, 16, 0.01, 1, 1.0);

    
    lightStrip.begin();
    ofClear(0,0,0, 255);
    lightStrip.end();
    
    //   playerPos = 0;
    
    p[0].setup();
    p[0].baseColor = ofColor::aqua;
    
    p[1].setup();
    p[1].baseColor = ofColor::orange;
    
    p[2].setup();
    p[2].baseColor = ofColor::cyan;
    
    
    panel.setup();
    panel.add(playerEnergySlowRate.set("playerEnergySlowRate", 0.97, 0.8, 1.0));
    panel.add(playerEnergySmoothRate.set("playerEnergySmoothRate", 0.93, 0.8, 1.0));
    panel.add(energyAddAmount.set("energyAddAmount", 5.0, 0.0, 20.0));
    panel.add(minRectWidth.set("minRectWidth", 4,0, 100));
    panel.add(maxRectWidth.set("maxRectWidth", 40.0, 0.0, 100.0));
    
    panel.add(fadeToBlackRate.set("fadeToBlackRate", 0.01, 0, 0.2));
    
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
    p[2].baseColor.set( color3->x, color3->y, color3->z);
    
    for (int i = 0; i < 3; i++){
        p[i].update();
        p[i].playerEnergySmoothRate = playerEnergySmoothRate;
        p[i].playerEnergySlowRate = playerEnergySlowRate;
        p[i].energyAddAmount = energyAddAmount;
        p[i].minRectWidth = minRectWidth;
        p[i].maxRectWidth = maxRectWidth;
    }

    blur.begin();
    ofEnableAlphaBlending();
    glColor4f(0,0,0, fadeToBlackRate);
    ofFill();
    ofRect(0,0,120,10);
    //ofClear(0,0,0,1);
    ofSetColor( ofColor::white);
    
    ofDisableAlphaBlending();
    ofEnableAlphaBlending();
    
    p[0].draw();
    p[1].draw();
    p[2].draw();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (p[0].playerEnergySmooth > 4.0 || p[1].playerEnergySmooth > 4.0 || p[2].playerEnergySmooth > 4.0){
       drawRectangleInFbo( ofRectangle( ofRandom(0,120), 0, ofRandom(10,20), 30), ofColor(ofRandom(0,80), ofRandom(0,80), ofRandom(0,80), 200));
    }
    ofEnableAlphaBlending();
    
    
    ofEnableAlphaBlending();
    blur.end();
    
    
    ofPixels pix;
    blur.getTextureReference().readToPixels(pix);
    
    
    
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
        
        
        //ofPoint col = colorMix;
        
        ofColor col;
        col.r = colorMix.x;
        col.g = colorMix.y;
        col.b = colorMix.z;
        
        float hueShift = ofSignedNoise(ofGetElapsedTimeMillis()/400.0, i/10.0);
        float satShift = ofSignedNoise(ofGetElapsedTimeMillis()/2000.0 + 10000.0, i/20.0);
        float briShift = powf(ofNoise(ofGetElapsedTimeMillis()/500.0 - 10000.0, i/4.0), 2.3);
        
        ofColor colTemp ;
        
        colTemp.setHsb(  col.getHue(), ofClamp(col.getSaturation() + satShift*20, 0, 255), col.getBrightness() * (0.7 + 0.3*briShift));

        
        setColor(order[i] ,  colTemp);
        
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
    blur.getTextureReference().draw(0,ofGetHeight()-50, ofGetWidth(),  (lightStrip.getHeight() / lightStrip.getWidth()) * ofGetWidth());
    ofEnableAlphaBlending();
    
    panel.draw();
    //stage.draw(0,300);
}