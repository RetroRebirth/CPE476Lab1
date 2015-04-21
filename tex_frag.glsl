uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform int uID;
uniform vec3 uView;
uniform vec3 uAClr;
uniform vec3 uDClr;
uniform vec3 uSClr;
uniform float uS;
uniform sampler2D uTexUnit;

varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
   vec3 col;
   float light; // Dot prod b/t normal and light source
   float h; // Specular light

   vec3 pos = vec3(uM * vPos);
   vec3 nor = vec3(vNor);
   vec3 view = uView - pos;

   // Light vector and dot product (no light if facing away)
   vec3 lightV = normalize(vec3(0, 1, 0)); // TODO keep light vector in separate class
   light = max(0.0, dot(normalize(nor), normalize(lightV)));

   vec3 H = normalize(view + lightV);
   h = pow(max(0.0, dot(normalize(nor), H)), uS);

   // Diffuse light
   vec3 dClr = light * uDClr;

   // Ambient light
   vec3 aClr = uAClr;

   // Specular light
   vec3 sClr = light * h * uSClr;

   col = dClr + aClr + sClr;

   gl_FragColor = vec4(col, 1.0);

   // Color based on textures
   vec4 texColor = texture2D(uTexUnit, vTexCoord);
   gl_FragColor = texColor;
}
