uniform sampler2D texture;
uniform float blurSize;

void main() {
    vec2 coord = gl_TexCoord[0].xy;
    vec4 sum = vec4(0.0);
    float size = blurSize * length(coord - vec2(0.5)) / 0.707106781;
    sum += texture2D(texture, vec2(coord.y - 2.0*size, coord.y)) * 0.09;
    sum += texture2D(texture, vec2(coord.y - 1.0*size, coord.y)) * 0.31;
    sum += texture2D(texture, vec2(coord.y, coord.y))            * 0.40;
    sum += texture2D(texture, vec2(coord.y + 1.0*size, coord.y)) * 0.31;
    sum += texture2D(texture, vec2(coord.y + 2.0*size, coord.y)) * 0.09;
    gl_FragColor = sum;
}
