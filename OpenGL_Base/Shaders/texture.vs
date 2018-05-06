#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 var_color;

//fragment shader will accept this output variable as input
out vec2 TexCoord;
out vec4 varColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	varColor =  vec4(1.0, 1.0, clamp(var_color.z, 0.0, 1.0), 1.0);
}