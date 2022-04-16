#version 150

uniform sampler2DRect trailMapTex;
uniform float blurSpeed;
uniform float fadeSpeed;
uniform float timestep;
uniform vec2 screenSize;

in vec2 vTexCoord;
out vec4 vFragColor;

void main() {
    vec4 original = texture(trailMapTex, vTexCoord);
    vec4 blurred = vec4(0.0, 0.0, 0.0, 0.0);
	
    for(int dy=-1; dy<=1; ++dy) {
        for(int dx=-1; dx<=1; ++dx) {
            vec2 samplePos = vTexCoord + vec2(dx, dy);

            // wrap around
            if(samplePos.x < 0) samplePos.x += screenSize.x;
            else if(samplePos.x >= screenSize.x) samplePos.x -= screenSize.x;
            if(samplePos.y < 0) samplePos.y += screenSize.y;
            else if(samplePos.y >= screenSize.y) samplePos.y -= screenSize.y;

            blurred += texture(trailMapTex, samplePos) / 9;
        }
    }
    
    vFragColor = mix(original, blurred, blurSpeed*timestep);

    // fade
    vFragColor.xyz *= pow(2.718, -fadeSpeed*timestep);
}
