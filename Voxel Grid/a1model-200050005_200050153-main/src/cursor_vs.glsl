//cursor_vs
#version 330

in vec3 vp;
// uniform vec3 uTranslation;
uniform mat4 uModelViewProjectMatrix;

void main () 
{
  // vec3 pos = vp + uTranslation;
  gl_Position = uModelViewProjectMatrix * vec4 (vp, 1.0);
}