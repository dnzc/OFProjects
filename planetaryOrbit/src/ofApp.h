#pragma once

#include "ofMain.h"
#include "simulator.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
	public:
	
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

		void drawInteractionArea();

        bool bHelpText;
        bool bDrawGrid;

		ofEasyCam cam; // add mouse controls for camera movement

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

        ofxPanel gui;
        ofxToggle m_showRealPaths;
        ofxToggle m_showKeplerPaths;
        ofxToggle m_showCowellPaths;
        ofxButton m_playReal;
        ofxButton m_playKepler;
        ofxButton m_playCowell;
        ofxIntSlider m_simSpeedSlider;

};
