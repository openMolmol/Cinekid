#pragma once

#include "ofMain.h"
#include "dataCollector.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

#define MAX_SERIALS 5


typedef struct {
    
    float energy;
    float energySmoothed;
    float accelEnergy;
    float nineDofEnergy;

    
} stats;



class xbeeManager {

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
    
        void exit();
    
        stats statList[3];
    
        void computeStats();
    
    
    
        int nSerials;
        ofSerial serials[MAX_SERIALS];
        string messages[MAX_SERIALS];
    
        vector < dataCollector > dataCollectors;
    
        ofxXmlSettings XML;


        ofParameter < float > minStrength;
        ofParameter < float > maxStrength;
        ofParameter < float > shaper;
    
        vector < ofParameter < float > > mins;
        vector < ofParameter < float > > maxs;
    
    
        //ofxToggle useOsc;
    
        ofxToggle calDynamicMinMax;
        ofParameter< float > maxOverMinDynamicRange;
        ofParameter< float > dynamicHistoryLength;
        ofParameter< float > dynamicChangeRate;

        ofParameter < bool > bUseDevices[5];
        ofParameter < bool > bUse9dofInsteadOfMuscle[5];
    
    
        ofxPanel panel;
    
        string sendMessage;
        //ofxOscSender sender;
        float overallEnergy;
    
    
    
};
