// Object coordinates
attribute vec4 aPos;        // vertex coordinates
attribute vec4 aNor;        // normals
attribute vec2 aTexCoord;   // texture coordinates

// Matrices
uniform mat4 uP;            // projection matrix
uniform mat4 uV;            // view matrix
uniform mat4 uM;            // model matrix

// Outgoing variables
varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
   gl_Position = uP * uV * uM * aPos;
   vPos = aPos;
   vNor = aNor;
   vTexCoord = aTexCoord;
}