#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup(){

    
    // this is a great hand rotation video
    //DP.loadFromDirectory("recording_2014-09-30-16-40-30-651");
    
    
    XML.load("settings.xml");
    
    nSerials = XML.getValue("data:numSerials", 0);
    
    for (int i = 0; i < nSerials; i++){
        
        string serialName= "serial" + ofToString(i);
        string serialDeviceName =XML.getValue("data:" + serialName, "");
        serials[i].setup(serialDeviceName, 115200);
        dataCollector DD;
        dataCollectors.push_back(DD);
        dataCollectors[dataCollectors.size()-1].setupData();
        dataCollectors[dataCollectors.size()-1].serialName = serialDeviceName;
    }
    
    bRecording = false;
    
    DP.loadFromDirectory("recording_2014-09-30-16-37-21-627");
    
    saveCount = 0;
}



string lastMessage = "";
//--------------------------------------------------------------
void ofApp::update(){
    
    
    DP.update();
    
    string message = DP.getCurrentString();
    if (lastMessage != message){
        dataCollectors[0].parseRecordedMessage(message);
        lastMessage = message;
    }
    
//    for (int i = 0; i < nSerials; i++){
//        while (serials[i].available()){
//            unsigned char bytes[100];
//            int howMany = serials[i].readBytes(bytes, 100);
//            cout << howMany << endl;
//            
//            if (howMany == 0 || howMany == -1) return;
//            
//            for (int j = 0; j < howMany; j++){
//                
//                int byte =  bytes[j];
//                
//                if (byte <=0 || byte > 127){
//                    //break;
//                }
//                
//                if ((char) byte == '!'){
//                    messages[i] += (char)byte;
//                } else {
//                    messages[i] += (char)byte;
//                }
//                
//                if ((char) byte == '\n'){
//                    dataCollectors[i].parseMessage(messages[i]);
//                    messages[i] = "";
//                    cout << i << " --> " <<  messages[i] << endl;
//                    if (bRecording == true && i == 0){
//                        myfile  << dataCollectors[i].sets[0].currentvalue
//                                << "," << dataCollectors[i].sets[1].currentvalue
//                                << "," << dataCollectors[i].sets[2].currentvalue
//                                << "," << dataCollectors[i].sets[3].currentvalue << endl;
//                        
//                    }
//                    
//                    
//                }
//            }
//        }
//    }
//    
    
    
    
    

//    for (int i = 0; i < dataCollectors.size(); i++){
//
//        if (ofGetFrameNum() %2 == i %2) dataCollectors[i].update();
//    }
//    
//    cout << ofGetFrameNum() << endl;
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    
    
    ofBackgroundGradient(ofColor::darkMagenta, ofColor::darkGoldenRod);
    
    
    DP.draw();
    
    ofSetColor(255,255,255,100);
    
    
    
    int width = 800;
    for (int i = 0; i < dataCollectors.size(); i++){
        dataCollectors[i].draw( ofRectangle(100 + i*(50 + width/dataCollectors.size()), 100, width/dataCollectors.size(), 600));
    }

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    
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
