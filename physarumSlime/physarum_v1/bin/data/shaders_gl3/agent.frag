#version 150

uniform sampler2DRect agentTex;
uniform sampler2DRect trailMapTex;
uniform float speed;
uniform float turnSpeed;
uniform float timestep;
uniform vec2 screenSize;
uniform float sensorAngleSpacing;
uniform float sensorOffsetDist;

in vec2 vTexCoord;

out vec4 vFragColor;

uint hash(uint state) {
    state ^= 2747636419u;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    return state;
}

float sense(vec2 agentPos, float agentAngle, float sensorAngleOffset) {
    float absAngle = agentAngle + sensorAngleOffset;
    vec2 sensorDir = vec2(cos(absAngle), sin(absAngle));
    vec2 sensorPos = agentPos + sensorOffsetDist * sensorDir;
    return texture(trailMapTex, sensorPos).x;
}

void main(void) {
    // MOVE
    // get position and angle from texture data
    vec2 pos = texture(agentTex, vTexCoord).xy;
    float angle = texture( agentTex, vTexCoord).z;
    
    vec2 direction = vec2(cos(angle), sin(angle));

    // update pos
    pos += direction * speed * timestep; 

    // wrap around
    while(pos.x < 0) pos.x += screenSize.x;
    while(pos.x >= screenSize.x) pos.x -= screenSize.x;
    while(pos.y < 0) pos.y += screenSize.y;
    while(pos.y >= screenSize.y) pos.y -= screenSize.y;

    /////////////////////////////////////////

    //STEER
    float weightForward = sense(pos, angle, 0);
    float weightLeft = sense(pos, angle, sensorAngleSpacing);
    float weightRight = sense(pos, angle, -sensorAngleSpacing);

    //float randomSteerStrength = float(  hash(uint( pos.x + pos.y*screenSize.x + hash(uint(vTexCoord.x)) ))  ) / 4294967295.0;
    float randomSteerStrength = 0.5;

    // turn randomly
    if(weightForward < weightLeft && weightForward < weightRight) {
        angle += (2*randomSteerStrength-1) * turnSpeed * timestep;
    // turn right
    } else if (weightRight > weightLeft) {
        angle -= randomSteerStrength * turnSpeed * timestep;
    // turn left
    } else if (weightRight < weightLeft) {
        angle += randomSteerStrength * turnSpeed * timestep;
    }
    
    //////////////////////////////
    vFragColor = vec4(pos.x, pos.y, angle, 1.0);
}
