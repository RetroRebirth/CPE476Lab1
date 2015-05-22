varying vec2 texpos;
uniform sampler2D uTex;
uniform vec4 uCol;

void main(void) {
   gl_FragColor = vec4(1, 1, 1, texture2D(uTex, texpos).r) * uCol;
}
