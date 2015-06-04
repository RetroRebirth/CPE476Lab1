// reference: http://ruh.li/GraphicsCookTorrance.html

uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform mat4 uRot;
uniform vec3 uView;
uniform float uTrans; // transparency
// Texture
uniform sampler2D uSampler0; // diffuse
uniform sampler2D uSampler1; // glow
// Glow texture
uniform int BlendMode;
uniform float BloomAmount;
// Blur texture
uniform int BlurMode;
uniform int BlurAmount;
uniform float BlurScale;
uniform float BlurStrength;
// Geometry information
varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

// TODO pass in values from Object.cpp
// Cook Torrance values
// reference: http://forums.cgarchitect.com/74801-fresnel-ior.html
/*
uniform float roughness;
uniform float fresnel;
uniform float geometric;
*/

/// Gets the Gaussian value in the first dimension.
float Gaussian (float x, float deviation)
{
   return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

void main() {
   // TODO use the global variables
   float roughness = 1.0;
   float fresnel = 1.6;
   float geometric = 1.0;

   // TODO remove when things are not as dark
   vec3 ambientLight = vec3(0.3, 0.3, 0.3); // hack, add ambient light to simulate more daytime
   
   // Create directional lighting
   vec3 viewDir = normalize(uView - (uV * uM * vPos).xyz);
   vec3 lColor = vec3(1.0, 1.0, 1.0);
   vec3 lDirection = vec3(-1.0, 1.0, 0.278);

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
      float mSquared = roughness * roughness;

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
      float schlick = pow(1.0 - VdotH, 5.0);
      schlick *= (1.0 - fresnel);
      schlick += fresnel;

      specular = (schlick * geoAtt * roughness) / (NdotV * NdotL * 3.14);
   }

   vec3 finalValue = lColor * NdotL * (geometric + specular * (1.0 - geometric)) + ambientLight;

   vec4 src = texture2D(uSampler1, vTexCoord); // glowmap

   // rendered scene w/ directional lighting
   vec4 tColor = texture2D(uSampler0, vTexCoord);
//   vec4 dst = tColor * vec4(lColor * (aIntensity + dIntensity), uTrans);
   vec4 dst = tColor * vec4(finalValue, uTrans);

   // ~~~~~~ KAYLA'S STUFF ~~~~~

   // Pure bloom
   if ( BlendMode == 1 )
   {
      dst = texture2D(uSampler0, vTexCoord); // rendered scene
      gl_FragColor = min((src + dst) * BloomAmount, 1.0);
   }
   // Additive blending (strong result, high overexposure)
   else if ( BlendMode == 2)
   {
      gl_FragColor = min((src + dst) * BloomAmount, 1.0);
   }
   // Screen blending (mild result, medium overexposure)
   else if ( BlendMode == 3 )
   {
      gl_FragColor = clamp((src + dst) - (src * dst), 0.0, 1.0);
      gl_FragColor.w = 1.0;
   }
   // Softlight blending (light result, no overexposure)
   else if ( BlendMode == 4)
   {
      src = (src * 0.5) + 0.5;

      gl_FragColor.xyz = vec3((src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x))),
            (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y))),
            (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z))));
      gl_FragColor.w = 1.0;
   }
   // No bloom added; render normally
   else
   {
      gl_FragColor = dst;
   }

   // Blur the scene
   if ( BlurMode == 1)
   {
      float halfBlur = float(BlurAmount) * 0.5;
      vec4 colour = vec4(0.0);
      float deviation = halfBlur * 0.35;
      deviation *= deviation;
      float strength = 1.0 - BlurStrength;

      // Horizontal blur
      for (int i = 0; i < 10; ++i)
      {
         if ( i >= BlurAmount )
            break;
         float offset = float(i) - halfBlur;
         vec4 h_blr = texture2D(uSampler1, vTexCoord + vec2(offset * (1.0/148.0) * BlurScale, 0.0)) * Gaussian(offset * strength, deviation);
         colour += h_blr;
      }
      // Vertical blur
      for (int i = 0; i < 10; ++i)
      {
         if ( i >= BlurAmount )
            break;
         float offset = float(i) - halfBlur;
         vec4 v_blr = texture2D(uSampler1, vTexCoord + vec2(0.0, offset * (1.0/148.0) * BlurScale)) * Gaussian(offset * strength, deviation);
         colour += v_blr;
      }

      // Apply colour
      gl_FragColor = clamp(colour, 0.0, 1.0);
      gl_FragColor.w = 1.0;
   }
}
