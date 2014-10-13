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

}
void lightController::setColor(int location, ofColor color){
    int i = location;
    dmx.setLevel(1 + i * 5, 255);
    dmx.setLevel(2+ i * 5, color.r);
    dmx.setLevel(3+ i * 5, color.g);
    dmx.setLevel(4+ i * 5, color.b);
    
}


void lightController::setColor(int location, ofPoint color){
    int i = location;
    dmx.setLevel(1 + i * 5, 255);
    dmx.setLevel(2+ i * 5, color.x);
    dmx.setLevel(3+ i * 5, color.y);
    dmx.setLevel(4+ i * 5, color.z);
}
void lightController::update(){

//    for (int i = 0; i < 18; i++){
//        dmx.setLevel(1 + i * 5, 255);
//        dmx.setLevel(2+ i * 5, 255);
//        dmx.setLevel(3+ i * 5, 255);
//        dmx.setLevel(4+ i * 5, 255);
//	}
    
    
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
    
    //stage.draw(0,300);
}