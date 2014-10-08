#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){

    
    //      vector < ofParameter < float > > mins;
    //      vector < ofParameter < float > > maxs;
    
    
    XML.load("settings.xml");
    
    nSerials = XML.getValue("data:numSerials", 0);
    
    
    
    XML.pushTag("data");
    for (int i = 0; i < nSerials; i++){
        
        ofParameter <float> val;
        mins.push_back( val);
        ofParameter <float> val2;
        maxs.push_back( val2);
        
        XML.pushTag("device", i);
        string serialName= "serial" + ofToString(i);
        
        int serialDeviceNumber =XML.getValue("xbeeName", 0);
        string serialDeviceName =XML.getValue("serial", "");
        serials[i].setup(serialDeviceName, 115200);
        
        
        dataCollector DD;
        dataCollectors.push_back(DD);
        dataCollectors[dataCollectors.size()-1].setupData();
        dataCollectors[dataCollectors.size()-1].serialName = serialDeviceName;
        dataCollectors[dataCollectors.size()-1].deviceNumber = "xbee: " + ofToString(serialDeviceNumber);
        XML.popTag();
    }
    XML.popTag();

    
    ofSetVerticalSync(false);

    panel.setup("pin control");
    panel.add(useOsc.setup("use osc", false));
    
    panel.add(minStrength.set("minStrength", 0, 0, 1000));
    panel.add(maxStrength.set("maxStrength", 500, 0, 1000));
    panel.add(shaper.set("shaper", 1, 0.3, 3.0));
    ofxLabel label;
    panel.add(label.setup("", "min/max"));
    for (int i = 0; i < nSerials; i++){
        panel.add(mins[i].set("min" + ofToString(i), 0, 0, 1024));
        panel.add(maxs[i].set("max" + ofToString(i), 1024, 0, 1024));
    }
    
    sender.setup(OSC_IP, OSC_PORT);
    
    //#define OSC_IP "127.0.0.1"
    //#define OSC_PORT 12345
    
}





//--------------------------------------------------------------
void ofApp::update(){
    
     //grabber.update();
    
    for (int i = 0; i < nSerials; i++){
        dataCollectors[i].sets[0].min = mins[i];
        dataCollectors[i].sets[0].max = maxs[i];
        
    }
    
    for (int i = 0; i < nSerials; i++){
        while (serials[i].available()){
            unsigned char bytes[100];
            int howMany = serials[i].readBytes(bytes, 100);
            cout << howMany << endl;
            
            if (howMany == 0 || howMany == -1) return;
            
            dataCollectors[i].nBytesRecvd += howMany;
            
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
                    
                    
                    //cout << i << " --> " <<  messages[i] << endl;
                    
                  
                    
                    
                }
            }
        }
    }
    
    if (useOsc == true){
        
        if (dataCollectors[0].sets[0].values.size() > 0){
            
            float curValue = dataCollectors[0].sets[0].currentvalue;
            float sendValue = ofMap(curValue, minStrength, maxStrength, 0, 1, true);
            sendValue = powf(sendValue, shaper);
            
            sendMessage = "sending " + ofToString(sendValue, 4);
            
            ofxOscMessage message;
            message.setAddress("/strength");
            message.addFloatArg(sendValue);
            sender.sendMessage(message);
        }
        
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofBackgroundGradient(ofColor::darkMagenta, ofColor::darkGoldenRod);
    
    
    ofSetColor(255,255,255,100);
    //grabber.draw(0,0);
    
    
    
    int width = 400;
    for (int i = 0; i < dataCollectors.size(); i++){
        dataCollectors[i].draw( ofRectangle(400 + i*(50 + width/dataCollectors.size()), 100, width/dataCollectors.size(), 600));
    }
    

    panel.draw();
    
    if (sendMessage.length() > 0){
        ofDrawBitmapStringHighlight(sendMessage, 50, 600);
    }
}

void ofApp::exit(){

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
