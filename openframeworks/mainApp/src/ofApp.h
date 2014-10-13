#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
//#include "ofxOsc.h"
#include "lightController.h"
#include "fanManager.h"
#include "soundManager.h"
#include "xbeeManager.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioRequested(float * input, int bufferSize, int nChannels);

        
        //ofxOscReceiver receiver;
        lightController  LC;
    
        float overAllEnergy;
        float individEnergy[3];
    
        fanManager FM;
        
        soundManager SM;
        
        float angle;
        float angleAdder;
        
        ofPoint mainTargetColor;
        float overallEnergySmooth;
        float overallEnergyChange;
        int nFramesNoEnergyChange;
        float noEnergyChangeEnergy;
        
        
        xbeeManager XM;
    
    
    float briForScreensaver[12];
    float briForOn[12];
    
    
        bool bDrawInput;
    
//    
        ofxPanel co2panel;
        ofParameter < bool > bUseCo2;
        ofParameter < int > minutesBetweenC02;
        ofParameter < bool > resetTimer;
        ofParameter < bool > fireSoon;
        ofParameter < bool > fireNow;
        ofParameter < int > nFramesHighToFire;
    
        int lastC02event;
        int nextC02eventTime;
        int nFramesGoodForC02;
    
    
//
//
//    
    
    
    
};
