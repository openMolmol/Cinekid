
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
    
    ofSerial ser;
    
    dataSet sets[4];
    
    
    void setupSerial( string _serialName){
        serialName = _serialName;
        ser.setup(serialName, 115200);
        message ="";
    }
    
    void update(){
        
        
    }
    
    void parseRecordedMessage( string & message){
        vector < string > vals = ofSplitString(message, ",");
        addVal( ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]), ofToFloat(vals[3])   );
    }
    
    void parseMessage(string & message){
        
        vector < string > vals = ofSplitString(message, ",");

        if (vals.size() < 7) return;
        
        addVal( ofToFloat(vals[3]), ofToFloat(vals[4]), ofToFloat(vals[5]), ofToFloat(vals[6])   );
        
        //strength = 0.9f * strength + 0.1 * ofToFloat(vals[3]);
        
    }
    
    void setupData(){
        sets[0].setup("muscle", 100, 0, 1024);
        sets[1].setup("heading", 100, -180, 180);
        sets[2].setup("pitch", 100, -180, 180);
        sets[3].setup("yaw", 100, -180, 180);
    }
    
    void addVal(float a,float b,float c,float d){
        sets[0].newValue(a);
        sets[1].newValue(b);
        sets[2].newValue(c);
        sets[3].newValue(d);
        
    }

    void draw(ofRectangle bounds){
        ofDrawBitmapStringHighlight(serialName, ofPoint(bounds.x, bounds.y-20));
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