//cursor_vs
#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec3 vCol;
layout (location = 3) in int vFlag;
// uniform vec3 uTranslation;
//uniform int res;

uniform mat4 uModelViewProjectMatrix;

out vec3 oColor;

void main () 
{
  // vec3 pos = vp + uTranslation;

  oColor = vCol;
  if(vFlag == 1)
    gl_Position = uModelViewProjectMatrix * vec4 (vPos + offset, 1.0);
  else
    gl_Position = vec4(2.0,2.0,2.0,1.0);
}