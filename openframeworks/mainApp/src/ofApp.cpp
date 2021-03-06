
#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    
    LC.setup();
    FM.setup();
    SM.setup();
    XM.setup();
    
    
    
    
    ofSoundStreamSetup(2,0,this, 44100, 256, 4);
    
    angle = 0;
    angleAdder = 0.01;
    mainTargetColor.set(255,255,255);
    overallEnergySmooth = 0;

    noEnergyChangeEnergy = 0;
    
    bDrawInput = true;
    
   
    font.loadFont("OsakaMono.ttf", 55);
    
    
    
    co2panel.setup();
    co2panel.add(bUseCo2.set("use c02", true));
    co2panel.add(minutesBetweenC02.set("minutes betw", 5, 3, 60));
    co2panel.add(resetTimer.set("reset timer", false));
    co2panel.add(fireSoon.set("advance timer", false));
    co2panel.add(fireNow.set("fire now", false));
    co2panel.add(nFramesHighToFire.set("n high frames to fire", 50, 1, 1000));
    
    
    XM.panel.setPosition(600,30);
    LC.panel.setPosition(800+20,30);
    SM.panel.setPosition(1000+40,30);
    FM.panel.setPosition(600,30 + 400);
    co2panel.setPosition(1000+40, 400);
    
    lastC02event = ofGetElapsedTimeMillis();
    nextC02eventTime = ofGetElapsedTimeMillis() + (minutesBetweenC02 * 60 * 1000);
    
    //cout << minutesBetweenC02 << endl;
    //cout << nextC02eventTime << endl;
    //std::exit(0);
    
    
    bDrawHelpScreen = true;
    
    
    buttons[0].setup(100 + (300+25)*0,200+ 200, 300, 100, false, false);
    buttons[1].setup(100 + (300+25)*1,200+ 200, 300, 100, false, false);
    buttons[2].setup(100 + (300+25)*2,200+ 200, 300, 100, false, false);
    
    
    buttons[0].setType(TYPE_TOGGLE);
    buttons[1].setType(TYPE_TOGGLE);
    buttons[2].setType(TYPE_TOGGLE);
    
    buttons[0].setColor(ofColor(180,180,180));
    buttons[0].setToggleColor(ofColor::green);
    buttons[1].setColor(ofColor(180,180,180));
    buttons[1].setToggleColor(ofColor::green);
    buttons[2].setColor(ofColor(180,180,180));
    buttons[2].setToggleColor(ofColor::green);
    
    buttons[0].setName("use 11");
    buttons[1].setName("use 10");
    buttons[2].setName("use 12");
    
    
    
    co2button.setup(100, 600, 50, 50, false, false);
   // co2button.setType(TYPE_TOGGLE);
    co2button.setName("fire co2");
    lightMode.setup(100+100, 600, 50, 50, false, false);
    lightMode.setName("light mode");
    //ofxSimpleButton co2button;
    //ofxSimpleButton lightMode;
    
    
    co2button.setColor(ofColor::lightGray);
    co2button.setToggleColor(ofColor::green);
    
    lightMode.setColor(ofColor::lightGray);
    lightMode.setToggleColor(ofColor::green);
    
    // fire co2
    // lights on white
    // lights off
    
    co2lastFrame = false;
    lightModeLastFrame = false;
    lightModeNum = 0;
}



static float crazyAngle = 0;

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    if (bDrawHelpScreen){
        XM.panel.setPosition(-2000+600,30);
        LC.panel.setPosition(-2000+800+20,30);
        SM.panel.setPosition(-2000+1000+40,30);
        FM.panel.setPosition(-2000+600,30 + 400);
        co2panel.setPosition(-2000+1000+40, 400);
    } else {
        XM.panel.setPosition(600,30);
        LC.panel.setPosition(800+20,30);
        SM.panel.setPosition(1000+40,30);
        FM.panel.setPosition(600,30 + 400);
        co2panel.setPosition(1000+40, 400);
    }
    
    
    buttons[0].setToggleColor( ofColor( LC.color1->x, LC.color1->y, LC.color1->z ));
    buttons[1].setToggleColor( ofColor( LC.color2->x, LC.color2->y, LC.color2->z ));
    buttons[2].setToggleColor( ofColor( LC.color3->x, LC.color3->y, LC.color3->z ));
    
    
    //--------------------------------------------------------------
    // handle c02 logic:
    if (bUseCo2){
        
        if (fireNow == true){
            LC.fireC02();
            nextC02eventTime = ofGetElapsedTimeMillis() + (minutesBetweenC02 * 60 * 1000);
            fireNow = false;
        }
        
        if (resetTimer == true){
            nextC02eventTime = ofGetElapsedTimeMillis() + (minutesBetweenC02 * 60 * 1000);
            resetTimer = false;
        }
        
        if (fireSoon == true){
            nextC02eventTime = ofGetElapsedTimeMillis();
            fireSoon = false;
        }
        
        if (overallEnergySmooth > 0.95){
            nFramesGoodForC02++;
        } else {
            nFramesGoodForC02 = 0;
        }
        
        int timeTillNext =  nextC02eventTime - ofGetElapsedTimeMillis();
        
        if (timeTillNext <= 0){
            if (nFramesGoodForC02 > nFramesHighToFire){
                LC.fireC02();
                nextC02eventTime = ofGetElapsedTimeMillis() + (minutesBetweenC02 * 60 * 1000);
            }
        }
    }
    //--------------------------------------------------------------
    
    
    
    //--------------------------------------------------------------
    XM.update();
    
    //--------------------------------------------------------------        update energy
    overAllEnergy =  XM.overallEnergy;
    float overallEnergySmoothLastFrame = overallEnergySmooth;
    if (overallEnergySmooth < overAllEnergy){
        overallEnergySmooth = 0.94f * overallEnergySmooth + 0.06 * overAllEnergy;
    } else {
        overallEnergySmooth = 0.99f * overallEnergySmooth + 0.01 * overAllEnergy;
    }
    overallEnergyChange = overallEnergySmoothLastFrame-overallEnergySmooth;
    if (nFramesNoEnergyChange > 100 && overallEnergySmooth < 0.05){
        noEnergyChangeEnergy = noEnergyChangeEnergy * 0.99 + 0.01 * 1.0;
    } else {
        
        noEnergyChangeEnergy = noEnergyChangeEnergy * 0.99 + 0.01 * 0.0;
    }
    
    if (fabs(overallEnergyChange) < 0.001){
        nFramesNoEnergyChange++;
    } else {
        nFramesNoEnergyChange = 0;
    }
    
    // TODO param
    individEnergy[0] = 0.95f * individEnergy[0]  + 0.05 * XM.statList[0].energy;
    individEnergy[1] = 0.95f * individEnergy[1]  + 0.05 * XM.statList[1].energy;
    individEnergy[2] = 0.95f * individEnergy[2]  + 0.05 * XM.statList[2].energy;
    
    
    bool bUseAnyDevices = false;
    for (int i = 0; i < 3; i++){
        if (XM.bUseDevices[i] == true){
            bUseAnyDevices = true;
        }
    }
    LC.setScreenSaver(!bUseAnyDevices);

    for (int i = 0; i < 3; i++){
        if (XM.bUseDevices[i]){
            LC.addEnergy(i,individEnergy[i]);
        }
    }
    
    //--------------------------------------------------------------
    
    
    //--------------------------------------------------------------  update fans
    
    if (true){
        FM.setStrength(0, XM.statList[0].energy);
        FM.setStrength(1, XM.statList[1].energy);
        FM.setStrength(2, XM.statList[2].energy);
    }
    
    FM.computeFanEnergy();
    
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------
    // light
    
    ofColor a = ofColor(LC.color1->x,LC.color1->y,LC.color1->z); // aqua
    ofColor b = ofColor(LC.color2->x,LC.color2->y,LC.color2->z); // spring green
    ofColor c = ofColor(LC.color3->x,LC.color3->y,LC.color3->z);   // violet
    
    ofColor aT = a;
    ofColor bT = b;
    ofColor cT = c;
    
    ofColor colors[3];
    colors[0]= aT;
    colors[1]= bT;
    colors[2]= cT;
    
    a.setBrightness( ofMap(individEnergy[0], 0, 1, 100, 255, true));
    b.setBrightness( ofMap(individEnergy[1], 0, 1, 100, 255, true));
    c.setBrightness( ofMap(individEnergy[2], 0, 1, 100, 255, true));
    
    int order[] = { 0,1,2,3,5,4,11,10,9,6,8,7 };
    
    int whoIsStrongest = -1;
    float strength = -1;
    for (int i = 0; i < 3; i++){
        if (strength < individEnergy[i]){
            strength = individEnergy[i];
            whoIsStrongest = i;
        }
    }
    
    if (whoIsStrongest != -1 && strength > 0.15){
        mainTargetColor = 0.93f * mainTargetColor + 0.07 *  ofPoint(colors[whoIsStrongest].r, colors[whoIsStrongest].g, colors[whoIsStrongest].b);
    } else {
        mainTargetColor = 0.93f * mainTargetColor + 0.07 * ofPoint(255,255,255);
    }
    
    //------------------------------------------------------------- color animation
    overAllEnergy = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
    overallEnergySmooth = 0.98f * overallEnergySmooth + 0.02*overAllEnergy;
    overAllEnergy = ofClamp(overAllEnergy, 0, 1);
    overallEnergySmooth = ofClamp(overallEnergySmooth, 0, 1);
    
    
    // angle is for the screensaver
    angleAdder = ofMap( powf(overallEnergySmooth, 5.0), 0, 1.0, 0.05, 0.19);
    angle += angleAdder;
    while(angle > TWO_PI) angle -= TWO_PI;
    while(angle < 0) angle += TWO_PI;
    
    // crazy angle if for the
    crazyAngle += ofMap(powf(overallEnergySmooth, 3.0), 0,1.0, 0.1, 0.45);
    while(crazyAngle > TWO_PI) crazyAngle -= TWO_PI;
    while(crazyAngle < 0) crazyAngle += TWO_PI;
    
   
    
    for (int i = 0; i < 12; i++){
        int who = order[i];
        float angleComp = (i/12.0) * TWO_PI;
        float angleDiff = angleComp - angle;
        while (angleDiff < -PI) angleDiff += TWO_PI;
        while (angleDiff > PI) angleDiff -= TWO_PI;
        float pct = 1 - fabs(angleDiff) / PI;
        pct = powf(pct, ofMap(overallEnergySmooth, 0, 1, 0.9, 3.0));
        
        briForScreensaver[who] = pct *  ofMap( powf(sin(ofGetElapsedTimeMillis()/2000.0)*0.5+0.5, 1.9), 0, 1, 0, 1, true);
        
        float crazyAmount = powf(overallEnergySmooth, 1.4);
        float onValue = ofMap( sin(crazyAngle + i*0.37), -1,1, 0  + overallEnergySmooth * 0.7, 1);
        float onPulse = crazyAmount * onValue + (1-crazyAmount) * pct;
        
        briForOn[who] =  onPulse;
        
        
    }
    
//    for (int i = 0; i < 12; i++){
//        cout << i << " " << briForScreensaver[i] << " " <<  briForOn[i] << endl;
//    }
    
    SM.actualVolume = ofMap( powf(overallEnergySmooth, SM.soundHitShaper), 0, 1, 0, SM.maxVolume, true);
    // FM.setStrength(overallEnergySmooth);
    
    
    //if (noEnergyChangeEnergy > 0.01){
//    a.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0 + TWO_PI/4), -1,1,  a.r,  a.r -  a.r * noEnergyChangeEnergy);
//    a.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/4), -1,1,  a.g,  a.g -  a.g * noEnergyChangeEnergy);
//    a.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/4), -1,1,  a.b,  a.b -  a.b * noEnergyChangeEnergy);
//    
//    b.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.r,  b.r -  b.r * noEnergyChangeEnergy);
//    b.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.g,  b.g -  b.g * noEnergyChangeEnergy);
//    b.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.b,  b.b -  b.b * noEnergyChangeEnergy);
//    
//    c.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.r,  c.r -  c.r * noEnergyChangeEnergy);
//    c.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.g,  c.g -  c.g * noEnergyChangeEnergy);
//    c.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.b,  c.b -  c.b * noEnergyChangeEnergy);
    
    //}
    
    LC.setColor(12,  a);
    LC.setColor(13,  b);
    LC.setColor(14,  c);
    
    
    LC.update();
    FM.update();
    SM.update();
    
    
    bool bCo2Now = co2button.getIsSelect();
    bool lModeNow = lightMode.getIsSelect();
    
    if (bCo2Now == true &&  co2lastFrame == false){
            // fire c02
    }
    
    if (lModeNow == true &&  lightModeLastFrame == false){
        // fire c02
        lightModeNum++;
        lightModeNum %= 3;
    }
    
    co2lastFrame = co2button.getIsSelect();
    lightModeLastFrame = lightMode.getIsSelect();
    
    cout << lightModeNum << endl;
//    co2lastFrame = false;
//    lightModeLastFrame = false;
//    lightModeNum = 0;
    
    //cout << co2button.getIsSelect() <<
    ;
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofBackground(ofColor(100,100,60));
    
    
    
    
    
    ofPushMatrix();
        if (bDrawHelpScreen){
            ofTranslate(-2000,0);
        } else {
            ofTranslate(0,0);
        }
        ofDrawBitmapString("last c02: " +  ofToString(lastC02event / 1000.0, 3) , 1000+40, 600);
        int timeTillNext =  nextC02eventTime - ofGetElapsedTimeMillis();
        if (timeTillNext < 0) timeTillNext = 0;
        ofDrawBitmapString("seconds till next c02: " +  ofToString(timeTillNext / 1000.0, 3) , 1000+40, 650);
    
    
        XM.draw();
        SM.draw();
        FM.draw();
        LC.draw();
    
    
    ofPopMatrix();
    
    
        if (bDrawHelpScreen){
            
            if (!buttons[0].getIsToggl()) ofSetColor(255,0,0);
            if (buttons[0].getIsToggl()) ofSetColor(0,255,0);
            
            font.drawString("11", 100, 200 - 40);
            XM.dataCollectors[0].sets[0].draw( ofRectangle(100,200, 300, 150));
           
            if (!buttons[1].getIsToggl()) ofSetColor(255,0,0);
            if (buttons[1].getIsToggl()) ofSetColor(0,255,0);
            
            
            
            font.drawString("10", 100 + (300+25)*1, 200-40);
            XM.dataCollectors[1].sets[0].draw( ofRectangle(100 + (300+25)*1,200, 300, 150));
            
            if (!buttons[2].getIsToggl()) ofSetColor(255,0,0);
            if (buttons[2].getIsToggl()) ofSetColor(0,255,0);
            
            
            font.drawString("12", 100 + (300+25)*2,200-40);
            XM.dataCollectors[2].sets[0].draw( ofRectangle(100 + (300+25)*2,200, 300, 150));
            
            buttons[0].show();
            buttons[1].show();
            buttons[2].show();
            co2button.show();
            lightMode.show();
            
            
            ofDrawBitmapStringHighlight("regular", ofPoint(100+100+100, 600), lightModeNum == 0 ? ofColor::green : ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("white", ofPoint(100+100+100, 630), lightModeNum == 1 ? ofColor::green : ofColor::black, ofColor::white);
            ofDrawBitmapStringHighlight("off", ofPoint(100+100+100, 660), lightModeNum == 2 ? ofColor::green : ofColor::black, ofColor::white);
            
            
            
        } else {
            buttons[0].hide();
            buttons[1].hide();
            buttons[2].hide();
            co2button.hide();
            lightMode.hide();
            
            
        }
    
    
    
    
  
    
    
    
    FM.panel.draw();
    co2panel.draw();
    
    
    
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    for (int i = 0; i < bufferSize; i++){
		output[i*nChannels    ] = 0;
		output[i*nChannels + 1] = 0;
	}
    
    // TODO: optimize
    
    SM.audioRequested(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 's'){
        
        // todo save and load settings
        
        XM.panel.saveToFile("xbeePanelSettings.xml");
        
        string fileName = ofToDataPath("xbeePanelSettings.xml");
        string cmd = "sed '/\\/>/d' " + fileName + " > " + fileName + ".fix";
        system(cmd.c_str());
        cmd = "mv " +  fileName + ".fix" + " " + fileName;
        system(cmd.c_str());
        
        
        FM.panel.saveToFile("fanPanelSettings.xml");
        
        fileName = ofToDataPath("fanPanelSettings.xml");
        cmd = "sed '/\\/>/d' " + fileName + " > " + fileName + ".fix";
        system(cmd.c_str());
        cmd = "mv " +  fileName + ".fix" + " " + fileName;
        system(cmd.c_str());
        
        SM.panel.saveToFile("soundSettings.xml");
        
        fileName = ofToDataPath("soundSettings.xml");
        cmd = "sed '/\\/>/d' " + fileName + " > " + fileName + ".fix";
        system(cmd.c_str());
        cmd = "mv " +  fileName + ".fix" + " " + fileName;
        system(cmd.c_str());
        
        LC.panel.saveToFile("lightSettings.xml");
    }
    
    XM.keyPressed(key);

    
    if (key == 'a'){
        LC.p[0].addEnergy();
    }
    
    
    if (key == 'p'){
        LC.p[1].addEnergy();
    }
    
    if (key == 'r'){
        LC.p[2].addEnergy();
    }
    
    if (key == 'd'){
        bDrawHelpScreen = !bDrawHelpScreen;
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

    
    if (bDrawHelpScreen){
    buttons[0].touchDown(x,y);
    buttons[1].touchDown(x,y);
    buttons[2].touchDown(x,y);
    co2button.touchDown(x,y);
    lightMode.touchDown(x,y);
    }
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
