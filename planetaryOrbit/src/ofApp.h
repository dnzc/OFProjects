#pragma once

#include "ofMain.h"
#include "simulator.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
	public:
        // openframeworks boilerplate
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

    private:
        // draw camera yellow circle
		void drawInteractionArea(); 

        // user interface booleans
        bool bHelpText;
        bool bDataText;
        bool bDrawGrid;

        // the 3d scene camera
		ofEasyCam cam; 

        // simulate the solar system from 1950 to 2010 (the approximate data range of the kernel) , timestep 24hrs, axis scale 10^7 metres
        Simulator simulator = Simulator(
            {
                "MERCURY BARYCENTER",
                "VENUS BARYCENTER",
                "EARTH BARYCENTER",
                "MARS BARYCENTER",
                "JUPITER BARYCENTER",
                "SATURN BARYCENTER",
                "URANUS BARYCENTER",
                "NEPTUNE BARYCENTER",
            },
            "sun", "1 Jan, 1950", "1 Jan, 2010", 3600*24, 7);

        // gui panel variables
        ofxPanel gui;
        ofxToggle m_showRealPaths;
        ofxToggle m_showKeplerPaths;
        ofxToggle m_showCowellPaths;
        ofxToggle m_showRealBodies;
        ofxToggle m_showKeplerBodies;
        ofxToggle m_showCowellBodies;
        ofxIntSlider m_simSpeedSlider;
        ofxButton m_resetBodies;

        // store when simulation was last started
        double m_lastPlayTimeMillis;
        void resetSimTime();

};
