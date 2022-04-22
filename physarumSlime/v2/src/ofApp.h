#pragma once

#include "ofMain.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int W = 3200;
const int H = 1800;
const int NUM_AGENTS = 1'600'000;
const float MOVE_SPEED = 100; // pixels per second (scaled by number of threads)
const float TURN_SPEED = 5; // radians per second
const float FADE_SPEED = 2; // each frame, the pixel value is timesd by e^(-FADE_SPEED*dt), so that the fade rate is almost constant
const float SENSOR_ANGLE_SPACING = 3.1416 / 6; // radians
const float SENSOR_OFFSET_DIST = 20; // pixels

struct pingPongBuffer {
    public:
        void allocate(vector<ofVec4f> data) {
            // Allocate
            for(int i = 0; i < 2; i++){
                BOs[i].allocate(data, GL_DYNAMIC_DRAW);
            }

            //Assign
            src = &BOs[0];
            dst = &BOs[1];
        };

        void swap() {
            std::swap(src, dst);
        };

        ofBufferObject& operator[]( int n ){ return BOs[n];}
        ofBufferObject* src;       // source ->  ping
        ofBufferObject* dst;       // dest   ->  pong

    private:
        ofBufferObject   BOs[2];    // actual addresses of ping/pong FBOs
};

class ofApp : public ofBaseApp {
    public:
        void setup();
        void update();
        void draw();

        void keyPressed  (int key);
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

        ofShader agentShader;
        ofShader trailMapShader;

        ofBufferObject agentBuffer;
        pingPongBuffer trailMapBuffer;

        ofFbo trailMap;

        float curTime;
};
