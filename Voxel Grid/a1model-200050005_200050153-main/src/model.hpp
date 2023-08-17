#pragma once

#include "shader.hpp"
#include "event.hpp"
#include <string>
#include <vector>
#include "glm/glm.hpp"

struct VoxelCube
{
  glm::vec3 m_position;
  glm::vec3 m_color;
};


class Model
{
private:
  GLuint m_vao = 0, m_vbo = 0, m_ibo = 0, m_colorBO=0, m_offsetBO=0, m_flagBO=0;

  std::string m_filename="";

  std::vector<VoxelCube> m_cubes;
  std::vector<bool> m_isfilled;
  unsigned int m_res;
public:
  Model(unsigned int VoxelResolution);
  //returns true if success
  bool addCube(glm::vec3& coordinates, glm::vec3& color);
  void deleteCube(glm::vec3& coordinates);

  void setupVAO();

  const void draw(glm::mat4 projViewMatrix, Shader shader);
  void inputHandle(const Event& e);
  void saveModel();
  void resetModel();
  void loadModel();


};

