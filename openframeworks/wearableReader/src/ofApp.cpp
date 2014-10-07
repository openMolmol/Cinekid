#include "ofApp.h"



string ZeroPadNumber(int num)
{
	stringstream ss;
	
	// the number is converted to string with the help of stringstream
	ss << num;
	string ret;
	ss >> ret;
	
	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < 4 - str_length; i++)
		ret = "0" + ret;
	return ret;
}

//--------------------------------------------------------------
void ofApp::setup(){

    
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
    
    //grabber.initGrabber(320, 240);
    
    saveCount = 0;
    
    
    ofSetVerticalSync(false);
   // ISR.startThread(false, true);

}





//--------------------------------------------------------------
void ofApp::update(){
    
     //grabber.update();
    
    
//    if (bRecording){
//        if (grabber.isFrameNew()){
//            ISR.addFrame(grabber);
//            //ofSaveImage( grabber.getPixelsRef(),  folderPath + "/" + ZeroPadNumber(saveCount) + ".jpg");
//            //saveCount ++;
//            //cout << folderPath + "/" + ZeroPadNumber(saveCount) + ".jpg"<< endl;
//        }
//    }
    
    
    for (int i = 0; i < nSerials; i++){
        while (serials[i].available()){
            unsigned char bytes[100];
            int howMany = serials[i].readBytes(bytes, 100);
            cout << howMany << endl;
            
            if (howMany == 0 || howMany == -1) return;
            
            for (int j = 0; j < howMany; j++){
                
                int byte =  bytes[j];
                
                if (byte <=0 || byte > 127){
                    //break;
                }
                
                if ((char) byte == '!'){
                    messages[i] += (char)byte;
                } else {
                    messages[i] += (char)byte;
                }
                
                if ((char) byte == '\n'){
                    dataCollectors[i].parseMessage(messages[i]);
                    messages[i] = "";
                    cout << i << " --> " <<  messages[i] << endl;
                    if (bRecording == true && i == 0){
                        myfile  << dataCollectors[i].sets[0].currentvalue
                                << "," << dataCollectors[i].sets[1].currentvalue
                                << "," << dataCollectors[i].sets[2].currentvalue
                                << "," << dataCollectors[i].sets[3].currentvalue << endl;
                        
                    }
                    
                    
                }
            }
        }
    }
    
    
    
    
    

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
    
    
    ofSetColor(255,255,255,100);
    //grabber.draw(0,0);
    
    
    
    int width = 800;
    for (int i = 0; i < dataCollectors.size(); i++){
        dataCollectors[i].draw( ofRectangle(100 + i*(50 + width/dataCollectors.size()), 100, width/dataCollectors.size(), 600));
    }
    string rec = (bRecording == true ? "true" : "false");
    
    ofDrawBitmapStringHighlight("space to record -- recording = " + rec, ofPoint(500, 50));
    if (bRecording == true){
        ofDrawBitmapStringHighlight("recording path = " + folderPath, ofPoint(500, 75));

    }
    
    stringstream c;
    c << "Recording: " << bRecording << "\nThread running: " << ISR.isThreadRunning() <<  "\nQueue Size: " << ISR.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl;
    
    ofDrawBitmapString(c.str(), 650, 100);
    
}

void ofApp::exit(){
    ISR.waitForThread();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    if (key == ' '){
        if (bRecording == false){
            folderPath = "output/recording_" + ofGetTimestampString();
            bRecording = true;
            
            
            ISR.setPrefix(ofToDataPath(folderPath + "/frame_")); // this directory must already exist
            ISR.setFormat("jpg");
            ISR.counter = 0;
            if (ofDirectory(folderPath).exists() == false){
                ofDirectory(folderPath).create();
            };
            
            
            myfile.open ( ofToDataPath(folderPath + "/data.txt").c_str());
            
            
        } else {
            myfile.close();
            bRecording = false;
        }

        
    }
    
    
    if(key == 't'){
        if(ISR.isThreadRunning()){
            ISR.stopThread();
        } else {
            ISR.startThread(false, true);
        }
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
