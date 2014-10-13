
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
    
    XM.panel.setPosition(600,30);
    LC.panel.setPosition(800+20,30);
    SM.panel.setPosition(1000+40,30);
    FM.panel.setPosition(600,30 + 400);
    
    
    
    co2panel.setup();
    co2panel.add(bUseCo2.set("use c02", true));
    co2panel.add(minutesBetweenC02.set("minutes betw", 5, 3, 60));
    co2panel.add(resetTimer.set("reset timer", false));
    co2panel.add(fireSoon.set("advance timer", false));
    co2panel.add(fireNow.set("fire now", false));
    co2panel.add(nFramesHighToFire.set("n high frames to fire", 50, 1, 1000));
    
    
    
    co2panel.setPosition(1000+40, 400);
    
    lastC02event = ofGetElapsedTimeMillis();
    nextC02eventTime = ofGetElapsedTimeMillis() + (minutesBetweenC02 * 60 * 1000);
    
    //cout << minutesBetweenC02 << endl;
    //cout << nextC02eventTime << endl;
    //std::exit(0);
}



static float crazyAngle = 0;

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
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
                //LC.fireC02();
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
    
    //--------------------------------------------------------------
    
    
    //--------------------------------------------------------------  update fans
    
    if (true){
        FM.setStrength(0, XM.statList[0].energy);
        FM.setStrength(1,XM.statList[1].energy);
        FM.setStrength(2, XM.statList[2].energy);
    }
    
    FM.computeFanEnergy();
    
    //--------------------------------------------------------------
    
    
    //-------------------------------------------------------------
    // light
    
    ofColor a = ofColor(0,250,250); // aqua
    ofColor b = ofColor(180,255,0); // spring green
    ofColor c = ofColor(255,80,255);   // violet
    
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
    a.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0 + TWO_PI/4), -1,1,  a.r,  a.r -  a.r * noEnergyChangeEnergy);
    a.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/4), -1,1,  a.g,  a.g -  a.g * noEnergyChangeEnergy);
    a.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/4), -1,1,  a.b,  a.b -  a.b * noEnergyChangeEnergy);
    
    b.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.r,  b.r -  b.r * noEnergyChangeEnergy);
    b.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.g,  b.g -  b.g * noEnergyChangeEnergy);
    b.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+ TWO_PI/2), -1,1,  b.b,  b.b -  b.b * noEnergyChangeEnergy);
    
    c.r = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.r,  c.r -  c.r * noEnergyChangeEnergy);
    c.g = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.g,  c.g -  c.g * noEnergyChangeEnergy);
    c.b = ofMap( sin(ofGetElapsedTimeMillis()/1000.0+3*TWO_PI/2), -1,1,  c.b,  c.b -  c.b * noEnergyChangeEnergy);
    
    //}
    
    //LC.setColor(12,  a);
    //LC.setColor(13,  b);
    //LC.setColor(14,  c);
    
    
    LC.update();
    FM.update();
    SM.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofBackground(ofColor(100,100,60));
    
    
        XM.draw();
    
        SM.draw();
        FM.draw();
        LC.draw();
        FM.panel.draw();

    co2panel.draw();
    
        
    ofDrawBitmapString("last c02: " +  ofToString(lastC02event / 1000.0, 3) , 1000+40, 600);
    int timeTillNext =  nextC02eventTime - ofGetElapsedTimeMillis();
    if (timeTillNext < 0) timeTillNext = 0;
    //cout << ofGetElapsedTimeMillis() << " " << nextC02eventTime << endl;
    
    ofDrawBitmapString("seconds till next c02: " +  ofToString(timeTillNext / 1000.0, 3) , 1000+40, 650);
    
    
    //lastC02event = ofGetElapsedTimef();
    //nextC02eventTime = ofGetElapsedTimeMillis() + minutesBetweenC02 * 60 * 1000;
    

//    for (int i = 0; i < 12; i++){
//        float w = ofGetWidth() / 12.0;
//        ofSetColor(briForScreensaver[i]*255);
//        ofFill();
//        ofRect(i*w, 600, w, 20);
//    
//        ofSetColor(briForOn[i]*255);
//        ofFill();
//        ofRect(i*w, 600+20, w, 20);
//        
//        
//    }
    
    
    
    
    
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
