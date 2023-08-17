#ifndef __MESH_HPP__
#define __MESH_HPP__
#include "shader.hpp"

class Mesh
{
protected:
 Shader* m_shader;
 glm::vec4 m_color;
 int m_is_textured=0;
 int m_texture_slot=-1;
 GLuint m_vao = 0, pos_BO = 0, normal_BO = 0, index_BO = 0, texcoord_BO = 0;

public:
 void setShader(Shader &shader) { m_shader = &shader; };
 void setTextureSlot(int texture_slot)
 {
  m_texture_slot = texture_slot;
  m_is_textured = 1;
 };
 virtual void render(glm::mat4 &modelviewprojmatrix, glm::mat4 modelMatrix) const=0;

};






#endif //__MESH_HPP__
