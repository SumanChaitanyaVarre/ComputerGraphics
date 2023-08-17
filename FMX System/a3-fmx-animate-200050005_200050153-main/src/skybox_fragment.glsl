#version 330 core

in vec3 fTexcoord;

uniform samplerCube uCubemap;

out vec4 color;
void main()
{
	color = texture(uCubemap, fTexcoord);
} 