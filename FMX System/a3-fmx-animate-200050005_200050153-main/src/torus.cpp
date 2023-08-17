#include "torus.hpp"


Torus::Torus(
 float minor_radius,
 float major_radius,
 int minor_segments,
 int major_segments,
 glm::vec3 color) :
 minor_segments(minor_segments),
 major_segments(major_segments)
{
 m_color = glm::vec4(color, 1.0);

 std::vector<glm::vec3> positions(minor_segments * major_segments);
 std::vector<glm::vec3> normals(minor_segments * major_segments);

 for (int major_idx = 0; major_idx < major_segments; major_idx++)
 {
  float major_angle = glm::radians(360 * major_idx / (float)major_segments);
  for (int minor_idx = 0; minor_idx < minor_segments; minor_idx++)
  {
   int vert_idx = major_idx * major_segments + minor_idx;
   float minor_angle = glm::radians(360 * minor_idx / (float)minor_segments);

   positions[vert_idx].x = (major_radius + minor_radius * cos(minor_angle)) * cos(major_angle);
   positions[vert_idx].y = (major_radius + minor_radius * cos(minor_angle)) * sin(major_angle);
   positions[vert_idx].z = minor_radius * sin(minor_angle);

   normals[vert_idx].x = cos(minor_angle) * cos(major_angle);
   normals[vert_idx].y = cos(minor_angle) * sin(major_angle);
   normals[vert_idx].z = sin(minor_angle);
  }
 }
 std::vector<unsigned int> indices(6 * minor_segments * major_segments);
 for (int major_idx = 0; major_idx < major_segments; major_idx++)
 {
  for (int minor_idx = 0; minor_idx < minor_segments; minor_idx++)
  {
   int face_idx = major_idx * major_segments + minor_idx;
   int vert1 = major_idx * major_segments + minor_idx;
   int vert2 = major_idx * major_segments + (minor_idx + 1) % minor_segments;
   int vert3 = ((major_idx + 1) % major_segments) * major_segments + minor_idx;
   int vert4 = ((major_idx + 1) % major_segments) * major_segments + (minor_idx + 1) % minor_segments;
   indices[face_idx * 6 + 0] = vert1;
   indices[face_idx * 6 + 1] = vert2;
   indices[face_idx * 6 + 2] = vert4;
   indices[face_idx * 6 + 3] = vert4;
   indices[face_idx * 6 + 4] = vert3;
   indices[face_idx * 6 + 5] = vert1;
  }
 }

 glGenVertexArrays(1, &m_vao);

 glGenBuffers(1, &pos_BO);
 glGenBuffers(1, &normal_BO);
 glGenBuffers(1, &index_BO);

 glBindVertexArray(m_vao);
 //positions
 glBindBuffer(GL_ARRAY_BUFFER, pos_BO);
 glBufferData(
  GL_ARRAY_BUFFER,
  sizeof(glm::vec3)*positions.size(),
  &positions[0],
  GL_STATIC_DRAW
 );
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
 //normals
 glBindBuffer(GL_ARRAY_BUFFER, normal_BO);
 glBufferData(
  GL_ARRAY_BUFFER,
  sizeof(glm::vec3) * normals.size(),
  &normals[0],
  GL_STATIC_DRAW
 );
 glEnableVertexAttribArray(1);
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
 //indices
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_BO);
 glBufferData(
  GL_ELEMENT_ARRAY_BUFFER,
  sizeof(unsigned int)*indices.size(),
  &indices[0],
  GL_STATIC_DRAW
 );

}

void Torus::render(glm::mat4 &modelviewprojmatrix, glm::mat4 modelMatrix) const
{
 m_shader->use();
 m_shader->setUniformMat4("uProjViewModelMatrix", modelviewprojmatrix);
 m_shader->setUniformMat4("uModelMatrix", modelMatrix);
 m_shader->setUniformVec4("uColor", m_color);
 m_shader->setUniformInt("uColTex", m_texture_slot);
 m_shader->setUniformInt("uIsTextured", m_is_textured);

 glBindVertexArray(m_vao);
 glDrawElements(
  GL_TRIANGLES,
  6 * minor_segments * major_segments,
  GL_UNSIGNED_INT,
  0
 );
}