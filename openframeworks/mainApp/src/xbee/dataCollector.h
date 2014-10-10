
#include "ofMain.h"



class dataSet {
    
public:
    
    string dataName;
    float currentvalue;
    int maxValsToStore;
    vector < float > values;
    float min, max;
    
    void setup( string name, int maxValues, float minV, float maxV){
        dataName = name;
        maxValsToStore = maxValues;
        min = minV;
        max = maxV;
    }
    
    void newValue (float val){
        currentvalue = val;
        values.push_back(currentvalue);
        if (values.size() > maxValsToStore){
            values.erase(values.begin());
        }
    }
    
    void draw(ofRectangle rect){
        
        float xStep = rect.width / maxValsToStore;
        
        ofSetColor(ofColor::peachPuff);
        ofRect(rect);
        ofSetColor(ofColor::white);
        ofNoFill();
        ofBeginShape();
        ofVertex(rect.x,rect.y + rect.height);
        for (int i = 0; i < values.size(); i++){
            
            float x = rect.x + xStep * i;
            float y = ofMap(values[i], min, max, rect.y + rect.height, rect.y, true);
            
            ofVertex(x,y);
        }
        
        ofVertex(rect.x + values.size() * xStep,rect.y + rect.height);
        
        ofEndShape();
        ofDrawBitmapStringHighlight(ofToString(dataName), ofPoint(rect.x, rect.y));
        
    }
    
};

class dataCollector {
    
public:
    
    string serialPort;
    bool bMakeFakeData;
    string serialName;
    string message;
    string deviceNumber;
    
    
    vector < float > strengthHistory;
    int historyLength = 200;
    
    ofSerial ser;
    
    dataSet sets[4];
    int nBytesRecvd;
    
    
    float getMin(){
        
        if (strengthHistory.size() == 0){
            return -1;
        } else {
            
            vector < float > temp = strengthHistory;
            ofSort(temp);
            float avgMin = 0;
            for (int i = 0; i < temp.size()*0.15; i++){
                avgMin += temp[i];
            }
            avgMin /= (float)temp.size()*0.15;
            return avgMin;
            //return *(std::min_element(strengthHistory.begin(),strengthHistory.end()));
        }
        return -1;
    }
    
    float getMax(){
        if (strengthHistory.size() == 0){
            return -1;
        } else {
            
            vector < float > temp = strengthHistory;
            ofSort(temp);
            float avgMax = 0;
            for (int i = temp.size()-1; i >= temp.size()*0.85; i--){
                avgMax += temp[i];
            }
            avgMax /= (float)temp.size()*0.15;
            return avgMax;
            //return *(std::min_element(strengthHistory.begin(),strengthHistory.end()));
        }
        return -1;
    }
    
    void setupSerial( string _serialName){
        serialName = _serialName;
        ser.setup(serialName, 115200);
        message =""; 
    }
    
    void update(){
        
        
    }
    
    void parseMessage(string & message){
        
        vector < string > vals = ofSplitString(message, ",");

        
        if (vals.size() != 5) return;
       
        
        addVal( ofToFloat(vals[1]), ofToFloat(vals[2])/1000.0, ofToFloat(vals[3])/1000.0, ofToFloat(vals[4])/1000.0 );
        
        // todo optimize
        
        strengthHistory.push_back(ofToFloat(vals[1]));
        while (strengthHistory.size() > historyLength){
            strengthHistory.erase(strengthHistory.begin());
        }
        
        //printf(" --> %f %f %f %f \n", ofToFloat(vals[1]), ofToFloat(vals[2])/1000.0, ofToFloat(vals[3])/1000.0, ofToFloat(vals[4])/1000.0 );
        //strength = 0.9f * strength + 0.1 * ofToFloat(vals[3]);
        
    }
    
    void setupData(){
        sets[0].setup("muscle", 100, 0, 1024);
        sets[1].setup("heading", 100, -180, 180);
        sets[2].setup("pitch", 100, -180, 180);
        sets[3].setup("yaw", 100, -180, 180);
        
        nBytesRecvd = 0;
        //sets[4].setup("battery", 100, 0, 1024);
    }
    
    void addVal(float a,float b,float c,float d){
        sets[0].newValue(a);
        sets[1].newValue(b);
        sets[2].newValue(c);
        sets[3].newValue(d);
    }

    void draw(ofRectangle bounds){
        
        //ofDrawBitmapStringHighlight(serialName, ofPoint(bounds.x, bounds.y-20));
        //ofDrawBitmapStringHighlight(ofToString(nBytesRecvd),ofPoint(bounds.x, bounds.y-40));
        ofDrawBitmapStringHighlight(deviceNumber,ofPoint(bounds.x, bounds.y-40), ofColor::magenta, ofColor::white);
        for (int i = 0; i < 4; i++){
            ofRectangle newBounds;
            newBounds.x = bounds.x;
            newBounds.y = bounds.y + (bounds.height / 4) * i;
            newBounds.width = bounds.width;
            newBounds.height = bounds.height / 4;
            sets[i].draw(newBounds);
            
        }
        
        
    }
    
    
};