uniform mat4 uP;
uniform mat4 uV;
uniform mat4 uM;
uniform vec3 uView;
uniform sampler2D uSampler;

varying vec4 vPos;
varying vec4 vNor;
varying vec2 vTexCoord;

void main() {
    // Add textures
    vec4 tColor = texture2D(uSampler, vTexCoord);
    
    // Create directional lighting
    vec3 viewDir = normalize(uView - (uV * uM * vPos).xyz);
    vec3 lColor = vec3(1.0, 1.0, 1.0);
    vec3 lDirection = vec3(-1.0, -1.0, 0.0);
    float aIntensity = 1.0;
    float dIntensity = max(0.0, dot(normalize(vNor.xyz), lDirection));
    
    // Shade the textures
    gl_FragColor = tColor * vec4(lColor * (aIntensity + dIntensity), 1.0);
}