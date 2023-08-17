#include "sphere.hpp"

double PI = 3.14159265;
int tri_idx = 0;
int wire_idx = 0;

Sphere::Sphere(float radius, int Lats, int Longs, glm::vec3 color):
 m_n_lattitude(Lats), m_n_longitude(Longs)
{
 m_color = glm::vec4(color, 1);

 std::vector<glm::vec3> positions(Lats*Longs+1);
 std::vector<glm::vec3> normals(Lats*Longs+1);
 positions[Lats * Longs] = glm::vec3(0, 0, radius);
 normals[Lats * Longs] = glm::vec3(0, 0, 1);

 for (int i = 0; i < Lats; i++)
 {
  for (int j = 0; j < Longs; j++)
  {
   float lat = glm::radians(i*90/(float)Lats);
   float lng = glm::radians(j*360/(float)Longs);
   positions[i * Longs + j].x = radius * cos(lat) * cos(lng);
   positions[i * Longs + j].y = radius * cos(lat) * sin(lng);
   positions[i * Longs + j].z = radius * sin(lat);

   normals[i * Longs + j].x = cos(lat) * cos(lng);
   normals[i * Longs + j].y = cos(lat) * sin(lng);
   normals[i * Longs + j].z = sin(lat);
  }
 }
 std::vector<unsigned int> indices(6*(Lats-1)*Longs + 3*Longs);
 for (int i = 0; i < Lats-1; i++)
 {
  for (int j = 0; j < Longs; j++)
  {
   indices[6 * (i * Longs + j) + 0] = i * Longs + j;
   indices[6 * (i * Longs + j) + 1] = (i+1) * Longs + j;
   indices[6 * (i * Longs + j) + 2] = i * Longs + (j + 1) % Longs;
   indices[6 * (i * Longs + j) + 3] = (i + 1) * Longs + j + 1;
   indices[6 * (i * Longs + j) + 4] = i * Longs + (j + 1) % Longs;
   indices[6 * (i * Longs + j) + 5] = (i+1) * Longs + j;
  }
 }
 for (int j = 0; j < Longs; j++)
 {
  indices[6 * ((Lats - 1) * Longs) + 3 * j + 0] = (Lats - 1) * Longs + j;
  indices[6 * ((Lats - 1) * Longs) + 3 * j + 1] = Lats * Longs;
  indices[6 * ((Lats - 1) * Longs) + 3 * j + 2] = (Lats - 1) * Longs + (j + 1) % Longs;
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
  sizeof(glm::vec3) * positions.size(),
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
  sizeof(unsigned int) * indices.size(),
  &indices[0],
  GL_STATIC_DRAW
 );
}

void Sphere::render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const
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
  (6 * (m_n_lattitude-1) * m_n_longitude) + (3 * m_n_longitude),
  GL_UNSIGNED_INT,
  0
 );
}