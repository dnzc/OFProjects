#pragma once

#include "ofMain.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define W 2560
#define H 1440
#define NUM_AGENTS_SQRT 1000 // agents are stored on square texture, so this is the side length of that texture (max offbo size is 16384, so must be less than this)

const static float MOVE_SPEED = 100; // pixels per second (scaled by number of threads)
const static float TURN_SPEED = 20; // radians per second
const static float BLUR_SPEED = 30; // blur is calculated as average of surrounding pixels, and then lerped by BLUR_SPEED*dt
const static float FADE_SPEED = 1; // each frame, the pixel value is timesd by e^(FADE_SPEED*dt), so that the fade rate is almost constant
const static float SENSOR_ANGLE_SPACING = 3.1416 / 4; // radians
const static float SENSOR_OFFSET_DIST = 20; // pixels

struct pingPongBuffer {
    public:
        void allocate( int _width, int _height, int _internalformat = GL_RGBA) {
            for(int i = 0; i < 2; i++){
                FBOs[i].allocate(_width,_height, _internalformat );
                FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            }

            //Assign
            src = &FBOs[0];
            dst = &FBOs[1];

            clear(0, 0, 0, 255);
        };

        void swap() {
            std::swap(src, dst);
        };

        void clear(int r, int g, int b, int a) {
            for(int i = 0; i < 2; i++) {
                FBOs[i].begin();
                ofClear(r, g, b, a);
                FBOs[i].end();
            }
        };

        ofFbo& operator[]( int n ){ return FBOs[n];}
        ofFbo   *src;       // source ->  ping
        ofFbo   *dst;       // dest   ->  pong

    private:
        ofFbo   FBOs[2];    // actual addresses of ping/pong FBOs
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
        ofShader effectShader;
        ofShader trailMapShader;
        ofShader colorShader;

        pingPongBuffer agentBuffer;
        ofFbo trailMapBuffer;

        ofVboMesh mesh;

        float curTime;
};
