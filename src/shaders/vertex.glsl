#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 MVP;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}