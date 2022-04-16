#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);  // Programmable pipeline
    settings.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}
