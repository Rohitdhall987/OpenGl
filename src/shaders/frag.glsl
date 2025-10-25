#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

void main()
{
    vec2 center = vec2(0.5, 0.5);

    float dist = distance(vTexCoord, center);

    float opacity = 0.0;

    float radius = 0.25;

    if (dist < radius)
        opacity = 1.0;
        

    FragColor = vec4(1.0, 0.5, 0.2, opacity);
} 