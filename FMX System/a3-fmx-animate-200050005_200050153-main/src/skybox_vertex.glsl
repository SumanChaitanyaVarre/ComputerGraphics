#version 330 core

in vec3 vPosition;

out vec3 fTexcoord;

uniform mat4 uProjViewMatrix;

void main()
{
    fTexcoord = vPosition;
    vec4 pos = uProjViewMatrix * vec4(vPosition, 1.0);
    gl_Position = pos.xyww;
}
