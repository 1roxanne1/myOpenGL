#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 outColor;
void main()
{
	gl_Position = vec4(position, 1.0);
	outColor = color;
};

#shader fragment
#version 330 core
in vec3 outColor;
out vec4 color;

void main()
{
	color = vec4(outColor, 1.0f);
};