////
////  dataRecorder.h
////  wearableReader
////
////  Created by zach on 10/7/14.
////
////
//
//#ifndef wearableReader_dataRecorder_h
//#define wearableReader_dataRecorder_h
//
//
//class dataRecorder{
//    
//    public:
//    
//    
//    string ZeroPadNumber(int num)
//    {
//        stringstream ss;
//        
//        // the number is converted to string with the help of stringstream
//        ss << num;
//        string ret;
//        ss >> ret;
//        
//        // Append zero chars
//        int str_length = ret.length();
//        for (int i = 0; i < 4 - str_length; i++)
//            ret = "0" + ret;
//        return ret;
//    }
//
//    
//    ofVideoGrabber grabber;
//    string folderPath;
//    bool bRecording;
//    ofstream myfile;
//    int saveCount;
//    ofxImageSequenceRecorder ISR;
//    
//    void setup
//    
//};
//
//
//#endif

//
//DR.setup();
//
//bRecording = false;
//saveCount = 0;
//
//
//#ifdef DATARECORD
//if (bRecording == true && i == 0){
//    myfile  << dataCollectors[i].sets[0].currentvalue
//    << "," << dataCollectors[i].sets[1].currentvalue
//    << "," << dataCollectors[i].sets[2].currentvalue
//    << "," << dataCollectors[i].sets[3].currentvalue << endl;
//    
//    }
//#endif
//    


//
//#ifdef DATARECORD
//ISR.waitForThread();
//#endif

//
//
//#ifdef DATARECORD
//dataRecorder DR;
//#endif



//#ifdef DATARECORD
//string rec = (bRecording == true ? "true" : "false");
//
//
//
//ofDrawBitmapStringHighlight("space to record -- recording = " + rec, ofPoint(850, 50));
//if (bRecording == true){
//    ofDrawBitmapStringHighlight("recording path = " + folderPath, ofPoint(850, 75));
//    
//}
//
//stringstream c;
//c << "Recording: " << bRecording << "\nThread running: " << ISR.isThreadRunning() <<  "\nQueue Size: " << ISR.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl;
//
//ofDrawBitmapString(c.str(), 850, 100);
//#endif


//#ifdef DATARECORD
//if (key == ' '){
//    if (bRecording == false){
//        folderPath = "output/recording_" + ofGetTimestampString();
//        bRecording = true;
//        
//        
//        ISR.setPrefix(ofToDataPath(folderPath + "/frame_")); // this directory must already exist
//        ISR.setFormat("jpg");
//        ISR.counter = 0;
//        if (ofDirectory(folderPath).exists() == false){
//            ofDirectory(folderPath).create();
//        };
//        
//        
//        myfile.open ( ofToDataPath(folderPath + "/data.txt").c_str());
//        
//        
//    } else {
//        myfile.close();
//        bRecording = false;
//    }
//    
//    
//}
//
//
//if(key == 't'){
//    if(ISR.isThreadRunning()){
//        ISR.stopThread();
//    } else {
//        ISR.startThread(false, true);
//    }
//}
//#endif
