#include "ofApp.h"
#include "SpiceUsr.h"
#include <iostream>
#include <cmath>
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){

    simulator.generateRealPaths();
    simulator.generateCowellPaths();

    gui.setup();
    gui.setPosition(20, 350);
    gui.add(m_showRealPaths.setup("show real paths", true));
    gui.add(m_showKeplerPaths.setup("show kepler paths", false));
    gui.add(m_showCowellPaths.setup("show cowell paths", false));
    gui.add(m_simSpeedSlider.setup("time speedup factor", 6000000, 1000000, 30000000));
    gui.add(m_playReal.setup("play real"));
    gui.add(m_playKepler.setup("play kepler"));
    gui.add(m_playCowell.setup("play cowell"));

    // lock fps to display
	ofSetVerticalSync(true);

    // circles are drawn as a polygon with a high number of sides
    ofSetCircleResolution(64);

	bHelpText = true;
	bDrawGrid = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(20);
	cam.begin();
	// use depth information for occlusion, rather than always drawing things on top of each other
	ofEnableDepthTest();

    if(bDrawGrid) {
        ofDrawGrid(100, 4, true, true, true, true);
        // draw scales
        std::stringstream ss;
        ss << "  /10^" << simulator.getScale() << "m";
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(450, 0, 0));
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(0, 450, 0));
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(0, 0, 450));
    } else {
        ofDrawAxis(50);
    }

    if(m_showRealPaths) {
        ofSetColor(ofColor::white);
        for(auto path : simulator.getPaths("real")) {
            path.draw();
        }
    }

    if(m_showCowellPaths) {
        ofSetColor(ofColor::purple);
        for(auto path : simulator.getPaths("cowell")) {
            path.draw();
        }
    }
    
    ofDisableDepthTest();
    cam.end();
	drawInteractionArea();

    if (bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << "\n\n";
        ss << "Toggle:\n";
        ss << "    This help: press 'h' key.\n";
        ss << "    Fullscreen: press 'f' key.\n";
        ss << "    Gridlines: press 'g' key.\n\n";

        /*ss << "Toggle fullscreen:\n";
        ss << "    press 'f' key.\n\n";
        ss << "Toggle gridlines:\n";
        ss << "    press 'g' key.\n\n";*/
        ss << "Reset camera view:\n";
        ss << "    DOUBLE MOUSE CLICK\n\n";
        ss << "Camera x,y rotation (pitch+yaw):\n";
        ss << "    LEFT MOUSE BUTTON DRAG inside yellow circle\n\n";
        ss << "Camera z rotation (roll):\n";
        ss << "    LEFT MOUSE BUTTON DRAG outside yellow circle\n\n";
        ss << "Camera translation (pan):\n";
        ss << "    LEFT MOUSE BUTTON DRAG + m\n";
        ss << "    MIDDLE MOUSE BUTTON DRAG\n\n";
        ss << "Camera zoom:\n";
        ss << "    RIGHT MOUSE BUTTON DRAG\n";
        ss << "    VERTICAL SCROLL\n";

        ofSetColor(255);
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }

    gui.draw();

}

//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
	ofRectangle vp = ofGetCurrentViewport();
	float r = std::min<float>(vp.width, vp.height) * 0.5f;
	float x = vp.width * 0.5f;
	float y = vp.height * 0.5f;

	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 255, 0);
	ofNoFill();
	glDepthMask(false);
	ofDrawCircle(x, y, r);
	glDepthMask(true);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
		case 'H':
		case 'h':
			bHelpText ^= true;
			break;
		case 'G':
		case 'g':
			bDrawGrid ^= true;
			break;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
