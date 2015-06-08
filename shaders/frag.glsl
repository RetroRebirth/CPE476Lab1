#version 120

// Object information
uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform mat4 uRot;
uniform vec3 uView;
uniform float uTrans;

// Texture
uniform sampler2D uSampler0; // diffuse
uniform sampler2D uSampler1; // glow
uniform sampler2D uSampler2; // normal
// Glow texture
uniform int BlendMode;
uniform float BloomAmount;
// Bump mapping
uniform int BumpMode;

// Geometry information
varying vec4 vNor;
varying vec4 vTan;
varying vec4 vPos;
varying vec2 vTexCoord;
varying vec3 lDirCamera;
varying vec3 eDirCamera;

void main ()
{
    vec4 diffuse =  texture2D(uSampler0, vTexCoord);
    vec4 glow =     texture2D(uSampler1, vTexCoord);
    vec4 norm =     texture2D(uSampler2, vTexCoord);
    
    // local normal in tangent space
    vec3 texNorTangent = normalize(2 * norm.rgb - 1);
    vec3 normal = normalize(mat3(uV * uM) * vNor.xyz);
    vec3 tangent = normalize(mat3(uV * uM) * vTan.xyz);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpNormal = 2.0 * norm.xyz - vec3(1.0, 1.0, 1.0);
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 newNormal = normalize(TBN * bumpNormal);
    
    // rendered scene w/ directional lighting
    vec3 lColor = vec3(1.0, 1.0, 1.0);
    float aIntensity = 0.5;
    float dIntensity = max(0.0, dot(normalize(vNor.xyz), lDirCamera));
    vec4 dst = diffuse * vec4(lColor * (aIntensity + dIntensity), uTrans);
    
    // Bump mapping
    if ( BumpMode == 1) {
        //float intensity = max(0.0, dot(newNormal, lDirCamera));
        float intensity = max(0.0, dot(texNorTangent, lDirCamera));
        dst = diffuse * vec4(lColor * (aIntensity + intensity), uTrans);
    }
    // Additive blending (strong result, high overexposure)
    if ( BlendMode == 1 ) {
        dst = texture2D(uSampler0, vTexCoord);
        gl_FragColor = min((glow + dst) * BloomAmount, 1.0);
    }
    // Screen blending (mild result, medium overexposure)
    else if ( BlendMode == 2 ) {
        dst = texture2D(uSampler0, vTexCoord);
        gl_FragColor = clamp(((glow + dst) - (glow * dst)) * BloomAmount, 0.0, 1.0);
        gl_FragColor.w = 1.0;
    }
    // Softlight blending (light result, no overexposure)
    else if ( BlendMode == 3) {
        dst = texture2D(uSampler0, vTexCoord);
        glow = (glow * 0.5) + 0.5;
        
        gl_FragColor.xyz = vec3((glow.x <= 0.5) ? (dst.x - (1.0 - 2.0 * glow.x) * dst.x * (1.0 - dst.x)) : (((glow.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * glow.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * glow.x - 1.0) * (sqrt(dst.x) - dst.x))),
                                (glow.y <= 0.5) ? (dst.y - (1.0 - 2.0 * glow.y) * dst.y * (1.0 - dst.y)) : (((glow.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * glow.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * glow.y - 1.0) * (sqrt(dst.y) - dst.y))),
                                (glow.z <= 0.5) ? (dst.z - (1.0 - 2.0 * glow.z) * dst.z * (1.0 - dst.z)) : (((glow.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * glow.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * glow.z - 1.0) * (sqrt(dst.z) - dst.z))));
        gl_FragColor.w = 1.0;
    }
    // No bloom added; render normally
    else {
        gl_FragColor = dst;
    }
}