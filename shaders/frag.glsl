// Object information
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

/// Gets the Gaussian value in the first dimension.
float Gaussian (float x, float deviation)
{
    return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

void main ()
{
    vec4 src = texture2D(uSampler1, vTexCoord); // glowmap
    
    // rendered scene w/ directional lighting
    vec4 tColor = texture2D(uSampler0, vTexCoord);
    vec3 viewDir = normalize(uView - (uV * uM * vPos).xyz);
    vec3 lColor = vec3(1.0, 1.0, 1.0);
    vec3 lDirection = vec3(-1.0, 1.0, 0.278);
    float aIntensity = 0.5;
    float dIntensity = max(0.0, dot(normalize(vNor.xyz), lDirection));
    vec4 dst = tColor * vec4(lColor * (aIntensity + dIntensity), uTrans);
    
    // blur shading gaussian filter
    float halfBlur = float(BlurAmount) * 0.5;
    vec4 colour = vec4(0.0);
    float deviation = halfBlur * 0.35;
    deviation *= deviation;
    float strength = 1.0 - BlurStrength;
    
    // Additive blending (strong result, high overexposure)
    if ( BlendMode == 1 ) {
        dst = texture2D(uSampler0, vTexCoord);
        gl_FragColor = min((src + dst) * BloomAmount, 1.0);
    }
    // Screen blending (mild result, medium overexposure)
    else if ( BlendMode == 2 ) {
        dst = texture2D(uSampler0, vTexCoord);
        gl_FragColor = clamp(((src + dst) - (src * dst)) * BloomAmount, 0.0, 1.0);
        gl_FragColor.w = 1.0;
    }
    // Softlight blending (light result, no overexposure)
    else if ( BlendMode == 3) {
        dst = texture2D(uSampler0, vTexCoord);
        src = (src * 0.5) + 0.5;
        
        gl_FragColor.xyz = vec3((src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x))),
                                (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y))),
                                (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z))));
        gl_FragColor.w = 1.0;
    }
    // No bloom added; render normally
    else {
        gl_FragColor = dst;
    }
    
    // Blur the scene with horizontal blur
    if ( BlurMode == 1) {
        for (int i = 0; i < 10; ++i)
        {
            if ( i >= BlurAmount )
                break;
            float offset = float(i) - halfBlur;
            vec4 h_blr = texture2D(uSampler0, vTexCoord + vec2(offset * (1.0/148.0) * BlurScale, 0.0)) * Gaussian(offset * strength, deviation);
            colour += h_blr;
        }
        gl_FragColor = clamp(colour, 0.0, 1.0);
        gl_FragColor.w = 1.0;
    }
    // Blur the scene with vertical blur
    if ( BlurMode == 2) {
        for (int i = 0; i < 10; ++i)
        {
            if ( i >= BlurAmount )
                break;
            float offset = float(i) - halfBlur;
            vec4 v_blr = texture2D(uSampler0, vTexCoord + vec2(0.0, offset * (1.0/148.0) * BlurScale)) * Gaussian(offset * strength, deviation);
            colour += v_blr;
        }
        gl_FragColor = clamp(colour, 0.0, 1.0);
        gl_FragColor.w = 1.0;
    }
}