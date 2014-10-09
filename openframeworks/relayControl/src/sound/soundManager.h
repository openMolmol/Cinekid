#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "granularManager.h"

class soundManager {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    
    void audioRequested(float * input, int bufferSize, int nChannels);
	
    granularManager	GM;
	
    float * audioData;
    
    ofxPanel panel;

    ofParameter < float > grainsPerSecond;
    ofParameter < float > avgFreq;
    ofParameter < float > avgFreqSpread;
    ofParameter < float > avgGrainLength;
    ofParameter < float > avgGrainSpreadLength;
    ofParameter < float > avgVolume;
    ofParameter < float > avgVolumeSpread;
    ofParameter < float > maxVolume;
    ofParameter < float > actualVolume;
    ofParameter < float > soundHitShaper;
    
    
};
