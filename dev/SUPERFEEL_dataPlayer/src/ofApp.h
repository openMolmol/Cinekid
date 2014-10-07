#pragma once

#include "ofMain.h"
#include "dataCollector.h"
#include "ofxXmlSettings.h"


#define MAX_SERIALS 5



class dataPlayer {

public: 
    vector < string > dataPacket;
    ofVideoPlayer player;

    void loadFromDirectory ( string directory ){
        player.loadMovie(directory + "/out.mp4");
        //cout << player.getWidth() << endl;
        
        std::string line;
        std::ifstream myfile( ofToDataPath(directory + "/data.txt").c_str());
        
        if(!myfile){
            ;
            cout << "error" << endl;
            return;
            
        }
        
        while (std::getline(myfile, line)){
            dataPacket.push_back(line);
        }
        
        player.play();
        
        player.setPosition(0.5);
    }
   
    void update(){
        player.update();
    }
    
    string getCurrentString(){
        float pct = player.getPosition();
        int who = dataPacket.size() * pct;
        who = ofClamp(who, 0, dataPacket.size()-1);
        return dataPacket[who];
    }
    
    void draw(){
        player.draw(0,0);
    }
};



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
    
        int nSerials;
        ofSerial serials[MAX_SERIALS];
        string messages[MAX_SERIALS];
    
    
    
        vector < dataCollector > dataCollectors;
    
        ofxXmlSettings XML;
    
    
        string folderPath;
        bool bRecording;
        ofstream myfile;
        int saveCount;
    
        dataPlayer DP;
    
    
};
