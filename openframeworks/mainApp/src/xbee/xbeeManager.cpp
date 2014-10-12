#include "xbeeManager.h"




//--------------------------------------------------------------
void xbeeManager::setup(){

    
    //      vector < ofParameter < float > > mins;
    //      vector < ofParameter < float > > maxs;
    
    
    XML.load("xbeeSettings.xml");
    
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
        dataCollectors[dataCollectors.size()-1].deviceNumber = "xbee_" + ofToString(serialDeviceNumber);
        XML.popTag();
    }
    XML.popTag();

    
    ofSetVerticalSync(false);

    panel.setup("data processing");
    //panel.add(useOsc.setup("use osc", false));
    
    
    for (int i = 0; i < nSerials; i++){
        panel.add(mins[i].set("min" + ofToString(i), 0, 0, 1024));
        panel.add(maxs[i].set("max" + ofToString(i), 1024, 0, 1024));
    }
    
    panel.add(calDynamicMinMax.setup("dynamic_min_max", false));
    panel.add(maxOverMinDynamicRange.set("max_over_min", 100, 0, 1000));
    panel.add(dynamicHistoryLength.set("dyn_history_len", 100, 0, 2000));
    panel.add(dynamicChangeRate.set("dyn_change_rate", 0.9, 0.9, 1.0));
    panel.add(dynamicMaxChangeRate.set("dyn_max_change_rate", 0.9, 0.9, 1.0));
    
    for (int i = 0; i < nSerials; i++){
        panel.add(bUseDevices[i].set("use_" + dataCollectors[i].deviceNumber, true));
    }
    
    for (int i = 0; i < nSerials; i++){
        panel.add(bUse9dofInsteadOfMuscle[i].set("nine_dof_muscle_" + dataCollectors[i].deviceNumber,false));
    }
    
    panel.loadFromFile("xbeePanelSettings.xml");
    //sender.setup(OSC_IP, OSC_PORT);
    
    //#define OSC_IP "127.0.0.1"
    //#define OSC_PORT 12345
    
    panel.setPosition(600-30, 50);
}





//--------------------------------------------------------------
void xbeeManager::update(){
    
     //grabber.update();
    
    if (calDynamicMinMax){
        
        for (int i = 0; i < nSerials; i++){
            dataCollectors[i].historyLength = dynamicHistoryLength;
            float min = dataCollectors[i].getMin();
            float max = dataCollectors[i].getMax();
            
            //cout << "---- " << i << " " << min << " " << max << endl;
            
            if (min >= 0.0){
                dataCollectors[i].sets[0].min = (dynamicChangeRate) *  dataCollectors[i].sets[0].min +
                                                (1-dynamicChangeRate) * min;
                
                mins[i] = dataCollectors[i].sets[0].min;
            }
            if (max >= 0.0){
                dataCollectors[i].sets[0].max = (dynamicChangeRate) *  dataCollectors[i].sets[0].max +
                                                (1-dynamicChangeRate) * max;
                
                if ((dataCollectors[i].sets[0].max - dataCollectors[i].sets[0].min) < maxOverMinDynamicRange){
                    dataCollectors[i].sets[0].max = dataCollectors[i].sets[0].min + maxOverMinDynamicRange;
                }
                
                maxs[i] = dataCollectors[i].sets[0].max;
            }
        }
        
        
        
        
    } else {
        for (int i = 0; i < nSerials; i++){
            dataCollectors[i].sets[0].min = mins[i];
            dataCollectors[i].sets[0].max = maxs[i];
        }
    }
    for (int i = 0; i < nSerials; i++){
        while (serials[i].available()){
            unsigned char bytes[100];
            int howMany = serials[i].readBytes(bytes, 100);
            //cout << howMany << endl;
            
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
                    
                    // calc dx on stuff:
                    
                    if (dataCollectors[i].sets[1].values.size() > 1){
                        
                        
                        
                        float dx1 =  *(dataCollectors[i].sets[1].values.end()-1) -
                                     *(dataCollectors[i].sets[1].values.end()-2);
                        
//                        float dx2 =  *(dataCollectors[i].sets[2].values.end()-1) -
//                                    *(dataCollectors[i].sets[2].values.end()-2);
//                        
//                        float dx3 =  *(dataCollectors[i].sets[2].values.end()-1) -
//                                     *(dataCollectors[i].sets[2].values.end()-2);
                        
                        
                        statList[i].nineDofEnergy = 0.92f * statList[i].nineDofEnergy +
                                                    0.08f * (fabs(dx1) / 1.0);
                        
                        
                        if (bUse9dofInsteadOfMuscle[i]){
                            
                            dataCollectors[i].sets[0].dataName = "muscle (9dof)";
                            
                            
                            dataCollectors[i].sets[0].values[dataCollectors[i].sets[0].values.size()-1] =  statList[i].nineDofEnergy * 15.0; //
                            
                            dataCollectors[i].sets[0].currentvalue =statList[i].nineDofEnergy * 15.0; //
                            

                            
                        } else {
                            dataCollectors[i].sets[0].dataName = "muscle";
                            
                        }
                        
                        
                        // ------
                        dataCollectors[i].strengthHistory.push_back(dataCollectors[i].sets[0].values[dataCollectors[i].sets[0].values.size()-1]);
                        while ( dataCollectors[i].strengthHistory.size() > dynamicHistoryLength){
                             dataCollectors[i].strengthHistory.erase( dataCollectors[i].strengthHistory.begin());
                        }
                        
                        // ------

                        
                    }
                    
                    
                                        //cout << i << " --> " <<  messages[i] << endl;
                    
                  
                    
                    
                }
            }
        }
    }
    
    static float ToSend = 0;
    
    if (true){
        
        // lots of calc happening here
        //useOsc == true){
        
        int nDevicesToUse = 3;
        
      
        
        
        
        
        float energy = 0;
        int countOfDev = 0;
        for (int i = 0; i < nDevicesToUse; i++){
            if (dataCollectors[i].sets[0].values.size() > 0){
                float curValue = dataCollectors[i].sets[0].currentvalue;
                if(bUseDevices[i]){
                    energy += powf(ofMap(curValue, mins[i], maxs[i], 0, 1.0, true), 1.5) *1.6;
                    countOfDev++;
                }
                if (i < 3) {
                    if(bUseDevices[i]){
                        statList[i].energy = ofMap(curValue, mins[i], maxs[i], 0, 1.0, true);
                        //cout << statList[i].energy << " " << curValue << " " << mins[i] << " " << " " << maxs[i] << endl;
                    } else {
                        statList[i].energy = 0;
                    }
                }
                //cout << i << " " << ofMap(curValue, mins[i], maxs[i], 0, 1.0, true) << endl;
            }
        }
        
        energy/= (float)MAX(countOfDev,1);
        energy = ofClamp(energy, 0, 1);
            //energy /= (float)nDevicesToUse;
            
        overallEnergy = energy;
            float sendValue = energy;
    
        
        

        
        /*
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
        */
        
    }
    
    
    
}

//--------------------------------------------------------------
void xbeeManager::draw(){

    
    
    ofSetColor(255,255,255,100);
    //grabber.draw(0,0);
    
    
    
    int width = 500;
    for (int i = 0; i < dataCollectors.size(); i++){
        dataCollectors[i].draw( ofRectangle(50 + i*(10 + width/dataCollectors.size()), 100, width/dataCollectors.size(), 400));
        
        ofDrawBitmapString( "energy: \n" + ofToString(statList[i].energy,4), 50 + i*(10 + width/dataCollectors.size()), 550);
        ofDrawBitmapString( "energySmoothed: \n" + ofToString(statList[i].energySmoothed,4), 50+i*(10 + width/dataCollectors.size()), 590);
        ofDrawBitmapString( "accelEnergy: \n" + ofToString(statList[i].accelEnergy,4), 50 + i*(10 + width/dataCollectors.size()), 630);
        ofDrawBitmapString( "nineDofEnergy: \n" + ofToString(statList[i].nineDofEnergy,4), 50 + i*(10 + width/dataCollectors.size()), 670);
        
    }
    

    panel.draw();
    
    if (sendMessage.length() > 0){
        ofDrawBitmapStringHighlight(sendMessage, 50, 600);
    }
    
    ofDrawBitmapStringHighlight("overallEnergy: " + ofToString(overallEnergy, 4), 50, 750);
}

void xbeeManager::exit(){

}

//--------------------------------------------------------------
void xbeeManager::keyPressed(int key){



    
}

//--------------------------------------------------------------
void xbeeManager::keyReleased(int key){

}

//--------------------------------------------------------------
void xbeeManager::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void xbeeManager::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void xbeeManager::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void xbeeManager::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void xbeeManager::windowResized(int w, int h){

}

//--------------------------------------------------------------
void xbeeManager::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void xbeeManager::dragEvent(ofDragInfo dragInfo){ 

}
