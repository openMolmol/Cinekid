#include "soundManager.h"
#include "ofxXmlSettings.h"


//--------------------------------------------------------------
void soundManager::setup(){

    
    panel.setup("sound settings");
    
    
    panel.add(grainsPerSecond.set("grainsPerSecond", 200, 0, 500.0));
    panel.add(avgFreq.set("avgFreq", 30, 0, 100));
    panel.add(avgFreqSpread.set("avgFreqSpread", 10, 0, 1000));
    panel.add(avgGrainLength.set("avgGrainLength", 50, 0, 200));
    panel.add(avgGrainSpreadLength.set("avgGrainSpreadLength", 10, 0, 100));
    panel.add(avgVolume.set("avgVolume", 0.01, 0, 0.45));
    panel.add(avgVolumeSpread.set("avgVolumeSpread", 0.005, 0, 0.45));
    panel.add(maxVolume.set("maxVolume", 0.0, 0, 1.0));
    panel.add(actualVolume.set("actualVolume", 0.0, 0, 1.0));
    
    panel.add(soundHitShaper.set("soundHitShaper", 6.3, 0,10.0));
    
    
    
    ofxXmlSettings settings;
    settings.loadFile("soundSettings.xml");
    grainsPerSecond = settings.getValue("sound_settings:grainsPerSecond", 0);
    avgFreq = settings.getValue("sound_settings:avgFreq", 0);
    avgFreqSpread = settings.getValue("sound_settings:avgFreqSpread", 0);
    avgGrainLength = settings.getValue("sound_settings:avgGrainLength", 0);
    avgGrainSpreadLength = settings.getValue("sound_settings:avgGrainSpreadLength", 0);
    avgVolume = settings.getValue("sound_settings:avgVolume", 0.0);
    avgVolumeSpread = settings.getValue("sound_settings:avgVolumeSpread", 0.0);
    maxVolume = settings.getValue("sound_settings:maxVolume", 0.0);
    actualVolume = settings.getValue("sound_settings:actualVolume", 0.0);
    soundHitShaper = settings.getValue("sound_settings:soundHitShaper", 0.0);
    
    panel.loadFromFile("soundsSettings.xml");
    
    
    
    
    
    panel.setPosition(250,50);
    
    /*
     sampleCount			= 0;
     grainsPerSecond		= 200;
     grainCount			= 0;
     
     avgFrequency			= 400;
     avgFrequencySpread		= 10;   // will go from 390-410
     avgGrainLength			= 50;
     avgGrainLengthSpread	= 10;
     avgVolume				= 0.01f;
     avgVolumeSpread			= 0.005f;
     */
    

    //audioData = new float[1024];
    
}

//--------------------------------------------------------------
void soundManager::update(){

    GM.avgFrequency = avgFreq;
    GM.grainsPerSecond = grainsPerSecond;
    GM.avgFrequencySpread = avgFreqSpread;
    GM.avgGrainLength = avgGrainLength;
    GM.avgGrainLengthSpread = avgGrainSpreadLength;
    GM.avgVolume = avgVolume;
    GM.avgVolumeSpread = avgVolumeSpread;
    GM.overallVolume = actualVolume;
    
    
    GM.idle();
}

void soundManager::audioRequested(float * output, int bufferSize, int nChannels){
    
    
    for (int i = 0; i < bufferSize; i++){
		output[i*nChannels    ] = 0;
		output[i*nChannels + 1] = 0;
	}
	
	GM.addToSoundBuffer(output, bufferSize);
	
	//for (int i = 0; i < bufferSize; i++){
	//	audioData[i] = output[i*2]*overallVolume;
	//}

    
}


//--------------------------------------------------------------
void soundManager::draw(){

    
    panel.draw();
}

//--------------------------------------------------------------
void soundManager::keyPressed(int key){

  //  cout << key << endl;
  
}
