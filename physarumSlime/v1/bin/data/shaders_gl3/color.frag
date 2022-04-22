#version 150

uniform sampler2DRect trailMapTex;

in vec2 vTexCoord;
out vec4 vFragColor;

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
    vec4 trailMapColor = texture(trailMapTex, vTexCoord);
    //vFragColor = lookup_cosine(trailMapColor.x); // cosine interpolation
    //vFragColor = lookup_linear(trailMapColor.x); // linear interpolation
    vFragColor = trailMapColor; // no colour
}
