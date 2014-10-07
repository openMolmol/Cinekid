#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxOsc.h"


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
    
    
        void allOnChanged( );
        
        int whichDevice;
        ofSerial serial;
        vector <ofSerialDeviceInfo> devices;
        ofArduino arduino;
        ofxXmlSettings xml;
        
        vector < ofParameter <bool> > relayPins;
    
    
        ofxToggle useOsc;
        ofxToggle allOn;
        
        ofxPanel panel;
        vector < int > pinIds;
        vector < bool > bFlipMes;
    
    
    ofxOscReceiver receiver;
    
    //ofxGui gui;
    
};
