#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofGLWindowSettings settings;
    settings.setGLVersion(4, 3);
    settings.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}

/*extern "C"
{
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}*/
