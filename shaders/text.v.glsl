attribute vec4 aCoord;
varying vec2 texpos;

void main(void) {
   gl_Position = vec4(aCoord.xy, 0, 1);
   texpos = aCoord.zw;
}
