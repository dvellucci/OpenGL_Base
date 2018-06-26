#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 PixelPos;
out vec2 TexCoords;
out vec4 PixelPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{		
	//position of a pixel
    PixelPos = vec3(model * vec4(aPos, 1.0));

	//normal of a surface
    Normal = transpose(inverse(mat3(model))) * aNormal;

    TexCoords = aTexCoords;
    PixelPosLightSpace = lightSpaceMatrix * vec4(PixelPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}