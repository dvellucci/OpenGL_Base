#version 330 core
//position of skybox faces
layout (location = 0) in vec3 aPos;

//position of the texture 
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}  