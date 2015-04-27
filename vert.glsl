attribute vec4 aPos;
attribute vec4 aNor;
attribute vec2 aTexCoord;

uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform vec3 uAClr;
uniform vec3 uDClr;
uniform vec3 uSClr;
uniform float uS;

varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
   gl_Position = uP * uV * uM * aPos;
   vPos = aPos;
   vNor = aNor;
   vTexCoord = aTexCoord;
}
