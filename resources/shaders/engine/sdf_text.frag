#version 460 core
precision highp float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 text_color;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
float pxRange = 4.0;

void main()
{
    float strength = 0.5;
/* Invert the strength so that 1.0 becomes bold and 0.0 becomes thin */
    float threshold = 1.0 - strength;

    vec2 msdfUnit = pxRange/vec2(textureSize(tex, 0));
    vec3 s = texture(tex, TexCoords).rgb;
    float sigDist = median(s.r, s.g, s.b) - threshold;
    sigDist *= dot(msdfUnit, 0.5/fwidth(TexCoords));
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    float alpha = smoothstep(0.5, 0.5, opacity); 
    FragColor = mix(vec4(0.0, 0.0, 0.0, 0.0), text_color, alpha);
}
