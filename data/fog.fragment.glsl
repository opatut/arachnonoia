uniform sampler2D texture;
uniform vec2 size;

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy;
    vec4 col = texture2D(texture, coord);
    float f = length(gl_FragCoord.xy / size - vec2(0.5)) / 0.707106781;
    col.a = smoothstep(0.0, 2.0, f);
    col.a += rand(gl_FragCoord.xy) * 0.03;
    gl_FragColor = gl_Color * col;
}
