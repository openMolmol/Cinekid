#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    panel.setup("fan control");
    panel.add(allOn.setup("all on", false));
    panel.add(useOsc.setup("use osc", false));
    
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
    
    
    
    panel.setPosition(820, 50);
    SM.panel.setPosition(820, 500);
    
    
    bDrawInput = true;
}



static float crazyAngle = 0;


//--------------------------------------------------------------
void ofApp::update(){

    XM.update();
    
    /*while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
		receiver.getNextMessage(&m);
        if(m.getAddress() == "/strength");
        float strength = m.getArgAsFloat(0);
        overAllEnergy = strength;
        
        float overallEnergySmoothLastFrame = overallEnergySmooth;
        if (overallEnergySmooth < overAllEnergy){
            overallEnergySmooth = 0.94f * overallEnergySmooth + 0.06 * overAllEnergy;
        } else {
            overallEnergySmooth = 0.99f * overallEnergySmooth + 0.01 * overAllEnergy;
        }
        
        overallEnergyChange = overallEnergySmoothLastFrame-overallEnergySmooth;
        cout << fabs(overallEnergyChange) << endl;
        if (fabs(overallEnergyChange) < 0.001){
            nFramesNoEnergyChange++;
        } else {
            nFramesNoEnergyChange = 0;
        }
        
        if (nFramesNoEnergyChange > 100 && overallEnergySmooth < 0.05){
            noEnergyChangeEnergy = noEnergyChangeEnergy * 0.99 + 0.01 * 1.0;
        } else {
            
            noEnergyChangeEnergy = noEnergyChangeEnergy * 0.99 + 0.01 * 0.0;
        }
       // cout << nFramesNoEnergyChange << endl;
        
        // sub energies:
        
        // TODO param
        individEnergy[0] = 0.95f * individEnergy[0]  + 0.05 * m.getArgAsFloat(1);
        individEnergy[1] = 0.95f * individEnergy[1]  + 0.05 * m.getArgAsFloat(2);
        individEnergy[2] = 0.95f * individEnergy[2]  + 0.05 * m.getArgAsFloat(3);
        
        // TODO better logic here please
        
        if (useOsc == true){
            
            
            
            FM.setStrength(0, m.getArgAsFloat(1));
            FM.setStrength(1, m.getArgAsFloat(2));
            FM.setStrength(2, m.getArgAsFloat(3));
            
            
            //FM.setStrength(overAllEnergy);
            // TODO FIXME  //setStregnth
        }
        
        FM.computeFanEnergy();
    }
    */
    
    
    //cout << allOn << endl;
    
        
    
    //-------------------------------------------------------------
    // light
    
    ofColor a =ofColor::green;
    ofColor b =ofColor::red;
    ofColor c =ofColor::blue;
    
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
    
    ofColor aT =ofColor::green;
    ofColor bT =ofColor::red;
    ofColor cT =ofColor::blue;
    
    ofColor colors[3];
    colors[0]= aT;
    colors[1]= bT;
    colors[2]= cT;
    
    
    if (whoIsStrongest != -1 && strength > 0.15){
    
        mainTargetColor = 0.93f * mainTargetColor + 0.07 *  ofPoint(colors[whoIsStrongest].r, colors[whoIsStrongest].g, colors[whoIsStrongest].b);
        
    } else {
        mainTargetColor = 0.93f * mainTargetColor + 0.07 * ofPoint(255,255,255);
        
    }
    
    
    
    //overAllEnergy = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
    //overallEnergySmooth = 0.98f * overallEnergySmooth + 0.02*overAllEnergy;
    
    angleAdder = ofMap( powf(overallEnergySmooth, 5.0), 0, 1.0, 0.05, 0.19);
    
    angle += angleAdder;
    while(angle > TWO_PI) angle -= TWO_PI;
    while(angle < 0) angle += TWO_PI;
    
    
    crazyAngle += ofMap(powf(overallEnergySmooth, 3.0), 0,1.0, 0.1, 0.6);
    while(crazyAngle > TWO_PI) crazyAngle -= TWO_PI;
    while(crazyAngle < 0) crazyAngle += TWO_PI;
    
    for (int i = 0; i < 12; i++){
        int who = order[i];
        
        
        //dmx.setLevel(1 + who * 5, 255);
        for (int j = 0; j < 3; j++){
            float angleComp = (i/12.0) * TWO_PI;
            float angleDiff = angleComp - angle;
            while (angleDiff < -PI) angleDiff += TWO_PI;
            while (angleDiff > PI) angleDiff -= TWO_PI;
            float pct = 1 - fabs(angleDiff) / PI;
            pct = powf(pct, ofMap(overallEnergySmooth, 0, 1, 0.9, 3.0));
            //cout << order[i] << " " << pct << endl;
            ofColor result = ofColor(mainTargetColor.x, mainTargetColor.y, mainTargetColor.z);
            
            
            float crazyAmount = powf(overallEnergySmooth, 1.4);
            float onValue = ofMap( sin(crazyAngle + i*0.37), -1,1, 0  + overallEnergySmooth * 0.7, 1);
            float onPulse = crazyAmount * onValue + (1-crazyAmount) * pct;
            
            
            float newVal = onPulse *( 80 + powf(overallEnergySmooth,2.0) * (255-80));
             newVal = ofMap( powf(sin(ofGetElapsedTimeMillis()/2000.0)*0.5+0.5, 6.0), 0,1, newVal, newVal - newVal * noEnergyChangeEnergy);
            
            
            result.setBrightness( newVal );
                
            
            
            
            
            LC.setColor(i, result);
            //dmx.setLevel(2+j+ who * 5, 255 * pct);
            
        }
        
    }
    
    
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
    
    LC.setColor(12,  a);
    LC.setColor(13,  b);
    LC.setColor(14,  c);

    
    LC.update();
    FM.update();
    SM.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofBackground(ofColor(100,100,60));
    
    
        XM.draw();
        XM.panel.setPosition(600,30);
        panel.setPosition(800+20,30);
        SM.panel.setPosition(1000+40,30);
        SM.draw();
        FM.draw();
        LC.draw();
        panel.draw();

    
    
        
    
    
    
    
    
    
    
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

    XM.keyPressed(key);
    SM.keyPressed(key);
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
