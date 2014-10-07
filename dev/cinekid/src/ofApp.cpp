#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    loader.loadModel("stage-cinekid-091714.obj");
    cout << loader.getNumMeshes() << endl;
    
    for (int i = 0; i < loader.getNumMeshes(); i++){
        meshes.push_back(loader.getMesh(i));
    }
    //vector < string > names = loader.getMeshNames();
    
//    for (int i = 0; i < names.size(); i++){
//        cout << names[i] << endl;
//    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    cam.begin();
    ofScale(100,100,100);
    for (int i = 0; i < meshes.size(); i++){
        meshes[i].drawWireframe();
    }
    cam.end();
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
