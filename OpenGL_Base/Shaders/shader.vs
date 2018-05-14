#version 330 core
layout (location = 0) in vec3 vertexCoord; //vertex position attribute. Gets the complete vertex of each triangle
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 var_color;

//fragment shader will accept this output variable as input
out vec2 TexCoord;
out vec4 varColor;

uniform float heightScale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
	gl_Position = projection * view * model * vec4(vertexCoord, 1.0f);

	//set the coords of the texture
	TexCoord = aTexCoord;   

	//set the color of each pixel in the height map based on the heightScale value 
	float g = (vertexCoord.y - 0)/(heightScale - 0); 
	varColor = vec4(g, g, g, 1.0);
}