#version 330 core
out vec4 FragColor;

//in is used for input variables
in vec4 varColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = texture(texture1, TexCoord);
	//FragColor = varColor;
}