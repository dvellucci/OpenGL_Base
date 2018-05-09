#version 330 core
out vec4 FragColor;

//in is used for input variables
in vec4 varColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform samplerCube cubemap;

void main()
{
	//applies the texture to the object
	FragColor = texture(texture1, TexCoord);
	
	//sets the color of the object 
	//FragColor = varColor;
}