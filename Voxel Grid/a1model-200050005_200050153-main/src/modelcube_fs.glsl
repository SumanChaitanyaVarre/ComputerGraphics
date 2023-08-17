//cursor_fs
#version 330

in vec3 oColor;

out vec4 frag_colour;


void main () 
{
  frag_colour = vec4 (oColor, 1.0);
}