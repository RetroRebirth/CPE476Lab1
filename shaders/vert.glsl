// Object coordinates
attribute vec4 aPos;        // vertex coordinates
attribute vec4 aNor;        // normals
attribute vec2 aTexCoord;   // texture coordinates

// Matrices
uniform mat4 uP;            // projection matrix
uniform mat4 uV;            // view matrix
uniform mat4 uM;            // model matrix
uniform mat4 uRot;          // rotation matrix (for lighting)
uniform mat4 uShadow;
uniform float transparancy;

// Outgoing variables
varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
   gl_Position = uP * uV * uM * aPos;
   vPos = uRot * aPos;
   vNor = uRot * aNor;
   vTexCoord = aTexCoord;
}