#version 330 core
layout (location = 0) in vec3 vertexCoord;

uniform mat4 lightSpaceMatrix;
uniform mat4 model; 

void main()
{
	//lightSpaceMatrix is used to transform vertices to light space...
	//and transform the world-space vertex position to light space
    gl_Position = lightSpaceMatrix * model * vec4(vertexCoord, 1.0);
}