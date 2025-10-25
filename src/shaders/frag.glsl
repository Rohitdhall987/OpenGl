#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec2 center = vec2(0.5, 0.5);

    float dist = distance(vTexCoord, center);

    float opacity = 0.0;

    float radius = 0.25;

    if (dist < radius)
        opacity = 1.0;
        
    vec4 t = texture(ourTexture, vec2(vTexCoord.x,vTexCoord.y+0.125));
    FragColor = vec4(mix(vec3(1),t.rgb,t.a), opacity);
} 