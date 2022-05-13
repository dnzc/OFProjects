#version 440

uniform ivec2 screenSize;
uniform float fadeSpeed;
uniform float dt;

layout (binding = 1) buffer oTM {
    vec4 oldTrailMap[];
};

layout (binding = 2) buffer nTM {
    vec4 newTrailMap[];
};

out vec4 vFragColor;

int idx(int x, int y) {
    return x + y*screenSize.x;
}

vec4 lookup_cosine(float t) {

    // choose color palette from https://iquilezles.org/www/articles/palettes/palettes.htm

    float[] coltab = float[](0.5,0.5,0.5,0.5,0.5,0.5,1.0,0.7,0.4,0.00,0.15,0.20);

    vec4 col;
    col.x = coltab[0]+coltab[3]*cos(2*3.1416*(coltab[6]*t+coltab[9]));
    col.y = coltab[1]+coltab[4]*cos(2*3.1416*(coltab[7]*t+coltab[10]));
    col.z = coltab[2]+coltab[5]*cos(2*3.1416*(coltab[8]*t+coltab[11]));
    col.w = 1.0;

    return col;
}

vec4 lookup_linear(float t) {
    // color palette taken from https://github.com/elliotwaite/rule-30-and-game-of-life
    // also used this to convert from hex to vec3: https://airtightinteractive.com/util/hex-to-glsl/
    // recent colors first (so 1-t is used later)
    vec3[] colors = vec3[](
        vec3(1.,1.,1.),
        vec3(0.443,0.11,0.569),
        vec3(0.918,0.,0.851),
        vec3(0.039,0.741,0.776),
        vec3(0.075,0.243,0.486),
        vec3(0.035,0.094,0.2),
        vec3(0.,0.004,0.012)
    );
    float scaled_t = (1-t)*colors.length();
    vec3 firstColor = colors[int(scaled_t)];
    vec3 secondColor = colors[int(scaled_t)+1];
    float mixAmt = scaled_t - int(scaled_t);
    return vec4(mix(firstColor, secondColor, mixAmt), 1.0);

}

void main() {
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);

    // blur
    vec4 newVal = vec4(0.0);
    for(int dy=-1; dy<=1; ++dy) {
		for(int dx=-1; dx<=1; ++dx) {
           int newX = (x + dx + screenSize.x) % screenSize.x;
           int newY = (y + dy + screenSize.y) % screenSize.y;
           newVal += oldTrailMap[idx(newX, newY)];
		}
    }
    newVal /= 9.0;

    // fade
    newVal *= pow(2.718, -fadeSpeed*dt);

    // apply
    newTrailMap[idx(x, y)] = newVal;

    // vFragColor = lookup_cosine(newVal.x); // cosine interpolation
    // vFragColor = lookup_linear(newVal.x); // linear interpolation
    vFragColor = newVal; // no color

}
