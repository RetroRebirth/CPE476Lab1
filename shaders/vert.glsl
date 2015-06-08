#version 120

// Object coordinates
attribute vec4 aPos;        // vertex coordinates
attribute vec2 aTexCoord;   // texture coordinates
attribute vec4 aNor;        // normals
attribute vec3 aTangent;    // tangents

// Matrices
uniform mat4 uP;            // projection matrix
uniform mat4 uV;            // view matrix
uniform mat4 uM;            // model matrix
uniform mat4 uRot;          // rotation matrix (for lighting)
uniform vec3 uView;

// Outgoing variables
varying vec4 vNor;
varying vec4 vTan;
varying vec4 vPos;
varying vec2 vTexCoord;
varying vec3 lDirCamera;
varying vec3 eDirCamera;

void main() {
   gl_Position = uP * uV * uM * aPos;
   vNor = uRot * aNor;
   vTan = uRot * vec4(aTangent, 0.0);
   vPos = uRot * aPos;
   vTexCoord = aTexCoord;
   
   // light and eye direction
   lDirCamera = vec3(-1.0, 1.0, 0.278);
   eDirCamera = normalize(uView - (uV * uM * aPos).xyz);
}