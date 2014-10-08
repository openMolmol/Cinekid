#pragma once

#include "ofMain.h"
#include "dataCollector.h"
#include "ofxXmlSettings.h"
#include "ofxImageSequenceRecorder.h"
#include "dataRecorder.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define OSC_IP "127.0.0.1"
#define OSC_PORT 12345

#define MAX_SERIALS 5



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
    
        void exit();
    
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
    
    
        ofxToggle useOsc;
        ofxPanel panel;
    
        string sendMessage;
        ofxOscSender sender;
    
    
    
};
