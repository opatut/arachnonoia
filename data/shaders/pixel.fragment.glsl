uniform sampler2D texture;
uniform vec2 center;
uniform vec2 size;

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

float mod(float a, float b) {
    return a - b * floor(a / b);
}

vec2 mod(vec2 a, vec2 b) {
    return vec2(mod(a.x, b.x), mod(a.y, b.y));
}

void main() {
    // lookup the pixel in the texture
    vec2 coord = gl_TexCoord[0].xy;
    vec4 col = texture2D(texture, coord);
    float f;

    // vignette
    vec2 center = vec2(0.5, 0.5);
    f = length(coord - center) / (0.5 * sqrt(2));
    col *= 1.0 - 0.7 * smoothstep(0.8, 1.1, f);

    // vertical vignette
    f = abs(coord.y - 0.5) * 2.0;
    col *= 1.0 - smoothstep(0.8, 1.2, f) * 0.6;

    // noise
    f = rand(round(coord * size / 4.0) / size * 100.0);
    col.rgb *= 1.0 - f * 0.07;

    // multiply it by the color
    gl_FragColor = gl_Color * col;
} 