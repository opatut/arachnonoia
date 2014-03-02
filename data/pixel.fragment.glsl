uniform sampler2D texture;

void main() {
    // lookup the pixel in the texture
    vec2 coord = gl_TexCoord[0].xy;
    vec4 col = texture2D(texture, coord);
    float f;

    // vignette
    vec2 center = vec2(0.5, 0.5);
    f = length(coord - center) / (0.5 * sqrt(2));
    col *= 1.0 - smoothstep(0.3, 0.9, f);

    // multiply it by the color
    gl_FragColor = gl_Color * col;
}