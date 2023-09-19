#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 view_pos;
uniform vec4 color;
uniform bool use_color;
uniform bool flip;
uniform sampler2D tex;

void main()
{    
    // and the diffuse per-fragment color
    vec4 cur_color = vec4(1);
    if (!use_color)
    {
        if (flip)
            cur_color = texture(tex, vec2(1.0 - TexCoords.x, TexCoords.y));
        else
            cur_color = texture(tex, TexCoords);
        if (cur_color.a == 0.0)
            discard;
        cur_color.rgb = pow(cur_color.rgb, vec3(2.2));
    }
    else
    {
        cur_color.rgb = color.rgb;
    }
    FragColor = cur_color;
}  