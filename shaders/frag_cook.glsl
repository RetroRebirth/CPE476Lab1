// reference: http://ruh.li/GraphicsCookTorrance.html

uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform mat4 uRot;
uniform vec3 uView;
uniform sampler2D uSampler;
uniform float transparancy;

varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
   // Add textures
   vec4 tColor = texture2D(uSampler, vTexCoord);

   // Create directional lighting
   vec3 viewDir = normalize(uView - (uV * uM * vPos).xyz);
   vec3 lColor = vec3(1.0, 1.0, 1.0);
   vec3 lDirection = vec3(-1.0, 1.0, 0.278);
   vec3 ambientLight = vec3(0.2, 0.2, 0.2); // hack, add ambient light to simulate more daytime

   // Set important material values
// TODO set values per matrial
   float roughnessValue = 1.0; // 0 : smooth, 1: rough
   float F0 = 1.0; // fresnel reflectance at normal incidence
   float k = 0.75; // fraction of diffuse reflection (specular reflection = 1 - k)

   // Interpolating normals will change the length of the normal, so renormalize the normal.
   vec4 nor = normalize(vNor);
   vec3 normal = normalize(vec3(vNor.x, vNor.y, vNor.z));

   // Do the lighting calculation for each fragment.
   float NdotL = max(dot(normal, lDirection), 0.0);

   float specular = 0.0;
   if(NdotL > 0.0) {
      vec3 eyeDir = normalize(viewDir);

      // Calculate intermediary values
      vec3 halfVector = normalize(lDirection + eyeDir);
      float NdotH = max(dot(normal, halfVector), 0.0); 
      float NdotV = max(dot(normal, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
      float VdotH = max(dot(eyeDir, halfVector), 0.0);
      float mSquared = roughnessValue * roughnessValue;

      // Geometric attenuation
      float NH2 = 2.0 * NdotH;
      float g1 = (NH2 * NdotV) / VdotH;
      float g2 = (NH2 * NdotL) / VdotH;
      float geoAtt = min(1.0, min(g1, g2));

      // Roughness (or: microfacet distribution function)
      // Beckmann distribution function
      float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
      float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
      float roughness = r1 * exp(r2);

      // Fresnel
      // Schlick approximation
      float fresnel = pow(1.0 - VdotH, 5.0);
      fresnel *= (1.0 - F0);
      fresnel += F0;

      specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
   }

   vec3 finalValue = lColor * NdotL * (k + specular * (1.0 - k)) + ambientLight;
   gl_FragColor = tColor * vec4(finalValue, 1.0);
//   gl_FragColor = vec4(finalValue, 1.0);
}
