// Object information
uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform mat4 uRot;
uniform vec3 uView;
uniform float transparancy;
// Textures and glow texture
uniform sampler2D uSampler0;
uniform sampler2D uSampler1;
uniform int BlendMode;
uniform float BloomAmount;
// Geometry information
varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

float Gaussian (float x, float deviation)
{
    return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

void main ()
{
    vec4 dst = texture2D(uSampler0, vTexCoord); // rendered scene
    vec4 src = texture2D(uSampler1, vTexCoord); // glowmap
    
    // Directional light texture shading
    vec4 tColor = texture2D(uSampler0, vTexCoord);
    vec3 viewDir = normalize(uView - (uV * uM * vPos).xyz);
    vec3 lColor = vec3(1.0, 1.0, 1.0);
    vec3 lDirection = vec3(-1.0, 1.0, 0.278);
    float aIntensity = 0.5;
    float dIntensity = max(0.0, dot(normalize(vNor.xyz), lDirection));
    dst = tColor * vec4(lColor * (aIntensity + dIntensity), transparancy);
    
    // No bloom added; render normally
    if ( BlendMode == 0 )
    {
        gl_FragColor = dst;
    }
    // Additive blending (strong result, high overexposure)
    else if ( BlendMode == 1 )
    {
        dst = texture2D(uSampler0, vTexCoord); // rendered scene
        gl_FragColor = min((src + dst) * BloomAmount, 1.0);
    }
    // Screen blending (mild result, medium overexposure)
    else if ( BlendMode == 2 )
    {
        gl_FragColor = clamp((src + dst) - (src * dst), 0.0, 1.0);
        gl_FragColor.w = 1.0;
    }
    // Softlight blending (light result, no overexposure)
    else if ( BlendMode == 3)
    {
        src = (src * 0.5) + 0.5;
        
        gl_FragColor.xyz = vec3((src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x))),
                                (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y))),
                                (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z))));
        gl_FragColor.w = 1.0;
    }
}