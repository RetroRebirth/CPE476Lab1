uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform vec3 uView;
/*
uniform vec3 uAClr;
uniform vec3 uDClr;
uniform vec3 uSClr;
uniform float uS;
*/
uniform sampler2D uSampler;

varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
    // Color based on textures
    gl_FragColor = texture2D(uSampler, vTexCoord);;
    
    // Shade the textures
    vec3 pos = vec3(uM * vPos);
    vec3 nor = vec3(vNor);
    vec3 view = uView - pos;
    vec3 lightVec = normalize(vec3(0, 1, 0));
    float light = max(0.0, dot(normalize(nor), normalize(lightVec)));
    
    //vec3 half = normalize(view + lightVec);
    //float spec = pow(max(0.0, dot(normalize(nor), half)), 1.0); // temporary spec value is 1.0
    //vec3 diffuseClr = light * texColor;
    //vec3 ambientClr = texColor;
    //vec3 specularClr = light * spec * texColor;
    
    //gl_FragColor = diffuseClr + ambientClr + specularClr;
    //gl_FragColor.w = 1.0;

   /*
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
   */
}
