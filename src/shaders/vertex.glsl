#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 transform;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}