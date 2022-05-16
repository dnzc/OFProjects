#include "ofApp.h"
#include "SpiceUsr.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

void ofApp::setup(){

    // load/calculate simulation data
    simulator.generateRealPaths();
    simulator.generateKeplerPaths();
    simulator.generateCowellPaths();

    // uncomment the line below to output simulation data to external file
    simulator.dumpPaths("./simulation_data.txt");

    //////////////////////////////
    // initialize GUI

    m_resetBodies.addListener(this, &ofApp::resetSimTime); // reset simulation time when button clicked

    gui.setup();
    gui.setPosition(450, 20);

    gui.add(m_showRealPaths.setup("show real paths", true));
    m_showRealPaths.setTextColor(ofColor::cyan);

    gui.add(m_showRealBodies.setup("show real bodies", true));
    m_showRealBodies.setTextColor(ofColor::cyan);

    gui.add(m_showKeplerPaths.setup("show kepler paths", false));
    m_showKeplerPaths.setTextColor(ofColor::orange);

    gui.add(m_showKeplerBodies.setup("show kepler bodies", false));
    m_showKeplerBodies.setTextColor(ofColor::orange);

    gui.add(m_showCowellPaths.setup("show cowell paths", false));
    m_showCowellPaths.setTextColor(ofColor::purple);

    gui.add(m_showCowellBodies.setup("show cowell bodies", false));
    m_showCowellBodies.setTextColor(ofColor::purple);

    gui.add(m_simSpeedSlider.setup("time speedup factor", 10000000, 1000000, 100000000));
    gui.add(m_resetBodies.setup("reset bodies"));

    //////////////////////////////

    // lock fps to display
	ofSetVerticalSync(true);

    // circles are drawn as a polygon with a high number of sides
    ofSetCircleResolution(64);

    // default values on startup (will show help text and data, but not gridlines)
	bHelpText = true;
	bDataText = true;
	bDrawGrid = false;
}

void ofApp::update(){}

void ofApp::draw(){

    ofBackground(20);
	cam.begin();
	// use depth information for occlusion, rather than always drawing things on top of each other
	ofEnableDepthTest();

    /////////////////////////////////////
    // draw gridlines

    if(bDrawGrid) {
        ofDrawGrid(100, 4, true, true, true, true);
        // draw scales
        ofSetColor(255);
        std::stringstream ss;
        ss << "  /10^" << simulator.getScale() << "km";
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(450, 0, 0));
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(0, 450, 0));
        ofDrawBitmapString(ss.str().c_str(), glm::vec3(0, 0, 450));
    } else {
        ofDrawAxis(50);
    }

    /////////////////////////////////////
    // draw orbit paths

    if(m_showRealPaths) {
        ofSetColor(ofColor::cyan);
        for(auto path : simulator.getPaths("real")) {
            path.draw();
        }
    }

    if(m_showKeplerPaths) {
        ofSetColor(ofColor::orange);
        for(auto path : simulator.getPaths("kepler")) {
            path.draw();
        }
    }

    if(m_showCowellPaths) {
        ofSetColor(ofColor::purple);
        for(auto path : simulator.getPaths("cowell")) {
            path.draw();
        }
    }

    /////////////////////////////////////
    // draw orbiting bodies

    double simulatedTimeElapsed = (double) (ofGetElapsedTimeMillis() - m_lastPlayTimeMillis) / 1000 * m_simSpeedSlider;

    stringstream data;
    data << std::fixed << std::setprecision(3); // fix values to 3dp

    if(m_showRealBodies) {
        ofSetColor(ofColor::cyan);
        for(auto pair : simulator.getNamedBarycentersAtTime(simulatedTimeElapsed, "real")) {
            const char* name = pair.first;
            ofPoint p = pair.second;
            ofDrawSphere(p.x, p.y, p.z, 5);
            data << "    REAL " << name << " x=" << p.x << " y=" << p.y << " z=" << p.z << "\n";
        }
    }
    if(m_showKeplerBodies) {
        ofSetColor(ofColor::orange);
        for(auto pair : simulator.getNamedBarycentersAtTime(simulatedTimeElapsed, "kepler")) {
            const char* name = pair.first;
            ofPoint p = pair.second;
            ofDrawSphere(p.x, p.y, p.z, 5);
            data << "    KEPLER " << name << " x=" << p.x << " y=" << p.y << " z=" << p.z << "\n";
        }
    }
    if(m_showCowellBodies) {
        ofSetColor(ofColor::purple);
        for(auto pair : simulator.getNamedBarycentersAtTime(simulatedTimeElapsed, "cowell")) {
            const char* name = pair.first;
            ofPoint p = pair.second;
            ofDrawSphere(p.x, p.y, p.z, 5);
            data << "    COWELL " << name << " x=" << p.x << " y=" << p.y << " z=" << p.z << "\n";
        }
    }

    /////////////////////////////////////
    
    ofDisableDepthTest();
    cam.end();
	drawInteractionArea();

    gui.draw();

    /////////////////////////////////////
    // draw help text

    if (bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << "\n\n";
        ss << "Toggle:\n";
        ss << "    This help: press 'h' key.\n";
        ss << "    Data panel: press 'H' key.\n";
        ss << "    Fullscreen: press 'f' key.\n";
        ss << "    Gridlines: press 'g' key.\n\n";
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

    /////////////////////////////////////
    // draw data text

    if(bDataText) {
        stringstream ss;
        ss << "Data is displayed to 3 d.p.\n\n";
        ss << std::fixed << std::setprecision(3); // round values to 3dp
        ss << "Ephemeris range: " << simulator.getStartTime() << " to " << simulator.getEndTime() << "\n";
        ss << "Timestep: " << simulator.getDeltaTime() / 3600 << " hrs\n\n";
        if(simulatedTimeElapsed < simulator.getTimeRange()) {
            ss << "Current ephemeris time (seconds after epoch): " << simulatedTimeElapsed << "\n";
            ss << "Position data (J2000) / 10^" << simulator.getScale() << "km:\n";
            ss << data.str();
        } else {
            ss << "Simulation complete - click 'reset bodies' to run again.";
        } 

        ofSetColor(255);
        ofDrawBitmapString(ss.str().c_str(), 20, bHelpText ? 400 : 20);
    }

}

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

void ofApp::resetSimTime(){
    m_lastPlayTimeMillis = ofGetElapsedTimeMillis(); // update the last time that the simulation was reset
}

void ofApp::keyPressed(int key){
    // choose what to do based on the key that was pressed
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
		case 'H':
            bDataText ^= true;
            break;
		case 'h':
			bHelpText ^= true;
			break;
		case 'g':
			bDrawGrid ^= true;
			break;
	}
}

void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
