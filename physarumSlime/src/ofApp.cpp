#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // shader for updating the texture that stores the agents position data (x, y, angle on R, G, B channels respectively)
    agentShader.load("shaders_gl3/passthru.vert", "shaders_gl3/agent.frag");

    // shader for applying effects to trail map (blur, fade)
    effectShader.load("shaders_gl3/passthru.vert", "shaders_gl3/effect.frag");

    // shader for updating the trail map based on new agent positions
    trailMapShader.load("shaders_gl3/trailMap");

    // shader for post processing i.e. coloring the final output
    colorShader.load("shaders_gl3/passthru.vert", "shaders_gl3/color.frag");
    
    // initializer list of agent information
    vector<float> init(NUM_AGENTS_SQRT*NUM_AGENTS_SQRT*3);
    for (int y = 0; y < NUM_AGENTS_SQRT; ++y) {
        for (int x = 0; x < NUM_AGENTS_SQRT; ++x) {
            int i = y * NUM_AGENTS_SQRT + x;

            /*float angle = (float) rand() / RAND_MAX * 2*3.1416;
            int dist = rand() % 100;
            init[i*3 + 0] = W/2 + cos(angle)*dist; // x
            init[i*3 + 1] = H/2 + sin(angle)*dist; // y
            init[i*3 + 2] = angle;*/
            
            init[i*3 + 0] = rand() % W;
            init[i*3 + 1] = rand() % H;
            init[i*3 + 2] = (float) rand() / RAND_MAX * 2*3.1416;
        }
    }

    // load info into relevant texture (ping pong)
    agentBuffer.allocate(NUM_AGENTS_SQRT, NUM_AGENTS_SQRT, GL_RGB32F);
    agentBuffer.src->getTexture().loadData(init.data(), NUM_AGENTS_SQRT, NUM_AGENTS_SQRT, GL_RGB);
    agentBuffer.dst->getTexture().loadData(init.data(), NUM_AGENTS_SQRT, NUM_AGENTS_SQRT, GL_RGB);
    
    // init trail map texture
    trailMapBuffer.allocate(W, H, GL_RGB32F);
    trailMapBuffer.begin();
    ofClear(0, 0, 0, 255);
    trailMapBuffer.end();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for(int y = 0; y < NUM_AGENTS_SQRT; ++y) {
        for(int x = 0; x < NUM_AGENTS_SQRT; ++x) {
            mesh.addVertex({x, y, 0});
            mesh.addTexCoord({x, y});
        }
    }

    curTime = ofGetElapsedTimeMillis() / 1000.0;

}

//--------------------------------------------------------------
void ofApp::update(){
    // we update the agents first and then the trail map, each one with a different shader and FBO

    // time
    float newTime = ofGetElapsedTimeMillis() / 1000.0;
    float dt = newTime - curTime;
    curTime = newTime;
    
    // agents (ping pong)
    agentBuffer.dst->begin();
    agentShader.begin();
    agentShader.setUniformTexture("agentTex", agentBuffer.src->getTexture(), 0); // texture containing position data
    agentShader.setUniformTexture("trailMapTex", trailMapBuffer.getTexture(), 1); // pixels
    agentShader.setUniform1f("speed", MOVE_SPEED);
    agentShader.setUniform1f("turnSpeed", TURN_SPEED);
    agentShader.setUniform1f("timestep", dt);
    agentShader.setUniform2f("screenSize", ofVec2f(W, H));
    agentShader.setUniform1f("sensorAngleSpacing", SENSOR_ANGLE_SPACING);
    agentShader.setUniform1f("sensorOffsetDist", SENSOR_OFFSET_DIST);
    agentBuffer.src->draw(0, 0);
    agentShader.end();
    agentBuffer.dst->end();
    agentBuffer.swap();
    
    // update trail map: apply effects then draw agents
    trailMapBuffer.begin();

    // apply effects
    effectShader.begin();
    effectShader.setUniformTexture("trailMapTex", trailMapBuffer.getTexture(), 2);
    effectShader.setUniform1f("blurSpeed", BLUR_SPEED);
    effectShader.setUniform1f("fadeSpeed", FADE_SPEED);
    effectShader.setUniform1f("timestep", dt);
    effectShader.setUniform2f("screenSize", ofVec2f(W, H));
    trailMapBuffer.draw(0, 0);
    effectShader.end();
    // draw agents
    trailMapShader.begin();
    trailMapShader.setUniformTexture("agentTex", agentBuffer.dst->getTexture(), 3);
    mesh.draw(); // "draw" the mesh: the shader draws result on texture, which we can also apply effects to
    trailMapShader.end();

    trailMapBuffer.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
    colorShader.begin();
    colorShader.setUniformTexture("trailMapTex", trailMapBuffer.getTexture(), 4);
    ofSetColor(255);
    trailMapBuffer.draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    colorShader.end();
    
    ofSetColor(0, 255, 0);
    ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate()), 15, 15);
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
