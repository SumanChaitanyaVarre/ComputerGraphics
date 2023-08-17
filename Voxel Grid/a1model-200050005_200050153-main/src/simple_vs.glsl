//simple_vs
#version 330

in vec3 vp;
uniform mat4 uModelViewProjectMatrix;

void main () 
{
  gl_Position = uModelViewProjectMatrix * vec4 (vp, 1.0);
}