#version 330

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexcoord;

uniform mat4 uProjViewModelMatrix;
uniform mat4 uModelMatrix;

out vec3 fWorldPos;
out vec3 fNormal;
out vec2 fTexcoord;

void main (void)
{
	fWorldPos = vec3(uModelMatrix * vec4(vPosition, 1.0));
	fNormal = normalize(mat3(transpose(inverse(uModelMatrix))) * vNormal);
	fTexcoord = vTexcoord;
	gl_Position = uProjViewModelMatrix * vec4(vPosition,1);
}
