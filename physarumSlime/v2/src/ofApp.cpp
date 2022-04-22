#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // shader for updating the agents
    agentShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders_gl3/agent.compute");
    agentShader.linkProgram();

    // shader for drawing + updating the trailMap
    trailMapShader.load("shaders_gl3/trailMap");
    
    ofBackground(0);

    // init + allocate buffers
    vector<ofVec4f> trailMapInit(W * H);
    for (auto& i : trailMapInit) {
        i.x = 0;
        i.y = 0;
        i.z = 0;
        i.w = 0;
    }
    trailMapBuffer.allocate(trailMapInit);
    trailMap.allocate(W, H, GL_RGBA);

    vector<ofVec4f> agentInit(NUM_AGENTS);
    for (auto& i : agentInit) {
        float r = (float)rand() / RAND_MAX * W / 4;
        i.z = (float)rand() / RAND_MAX * 2 * 3.1416;
        //i.x = r * cos(i.z) + W / 2;
        i.x = rand() % W;
        //i.y = r * sin(i.z) + H / 2;
        i.y = rand() % H;
        i.w = 0;
    }
    agentBuffer.allocate(agentInit, GL_DYNAMIC_DRAW);


    curTime = ofGetElapsedTimeMillis() / 1000.0;

    // bind buffers for shaders
    agentBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    trailMapBuffer.src->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    trailMapBuffer.dst->bindBase(GL_SHADER_STORAGE_BUFFER, 2);


}

//--------------------------------------------------------------
void ofApp::update(){

    ofSetWindowTitle("FPS:" + to_string(ofGetFrameRate()));

    // time
    float newTime = ofGetElapsedTimeMillis() / 1000.0;
    float dt = newTime - curTime;
    curTime = newTime;

    agentShader.begin();
    agentShader.setUniform1f("speed", MOVE_SPEED);
    agentShader.setUniform1f("turnSpeed", TURN_SPEED);
    agentShader.setUniform1f("dt", dt);
    agentShader.setUniform2i("screenSize", W, H);
    agentShader.setUniform1f("sensorAngleSpacing", SENSOR_ANGLE_SPACING);
    agentShader.setUniform1f("sensorOffsetDist", SENSOR_OFFSET_DIST);
    agentShader.dispatchCompute(NUM_AGENTS / 1024 + 1, 1, 1); // one workgroup for every 1024 agents
    agentShader.end();

    trailMapShader.begin();
    trailMapShader.setUniform2i("screenSize", W, H);
    trailMapShader.setUniform1f("fadeSpeed", FADE_SPEED);
    trailMapShader.setUniform1f("dt", dt);
    trailMap.begin();
    trailMap.draw(0, 0);
    trailMap.end();
    trailMapShader.end();

    trailMapBuffer.swap();
	trailMapBuffer.src->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	trailMapBuffer.dst->bindBase(GL_SHADER_STORAGE_BUFFER, 2);

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    trailMap.draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
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
