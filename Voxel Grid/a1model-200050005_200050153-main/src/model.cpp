#include "model.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

#include <fstream>

#define QUAD(a, b, c, d) a, b, c, a, c, d

//res = Voxel Resolution
Model::Model(unsigned int res) :
  m_cubes(std::vector<VoxelCube>(res * res * res)),
  m_isfilled(std::vector<bool>(res * res * res, false)),
  m_res(res)
{}

bool Model::addCube(glm::vec3& coords, glm::vec3& color)
{
  int index = coords.x * pow(m_res, 2) + coords.y * (double)m_res + coords.z;

  if (index >= m_isfilled.size())
    return false;

  m_cubes[index].m_position = coords;
  m_cubes[index].m_color = color;
  m_isfilled[index] = true;

  glBindBuffer(GL_ARRAY_BUFFER, m_colorBO);
  glBufferSubData(GL_ARRAY_BUFFER, index* sizeof(glm::vec3), sizeof(glm::vec3), &color);
  glBindBuffer(GL_ARRAY_BUFFER, m_flagBO);
  GLint x = 1;
  glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(GLint), sizeof(GLint), &x);
  return true;
}

void Model::deleteCube(glm::vec3& coords)
{
  int index = coords.x * pow(m_res, 2) + coords.y * (double)m_res + coords.z;
  m_isfilled[index] = false;
  glBindBuffer(GL_ARRAY_BUFFER, m_flagBO);
  GLint x = 0;
  glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(GLint), sizeof(GLint), &x);
}

const void Model::draw(glm::mat4 projViewMatrix, Shader shader)
{
  shader.use();
  //unsigned int size = m_res * m_res * m_res;
  //for (int i = 0; i < size; i++)
  //{
  //  if (m_isfilled[i])
  //  {
  //    glm::mat4 translation = glm::translate(glm::mat4(1.0f),
  //      m_cubes[i].m_position / (float)m_res * 2.0f);
  //    shader.setUniformMat4("uModelViewProjectMatrix", projViewMatrix * translation);
  //    shader.setUniformVec3("u_color", m_cubes[i].m_color);

  //    //TODO error handling
  //    glBindVertexArray(m_vao);
  //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  //  }
  //}

  shader.setUniformMat4("uModelViewProjectMatrix", projViewMatrix);
  glBindVertexArray(m_vao);
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, m_res*m_res*m_res);
}

void Model::inputHandle(const Event& e)
{
  if (e.m_key == GLFW_KEY_S && e.m_action == GLFW_PRESS)
  {
    if (m_filename == "")
    {
      std::string filename;
      std::cout << "enter model name:" << std::endl;
      std::cin >> filename;
      #ifdef WIN32
      m_filename = "./../repo/data/" + filename + ".raw";
      #else
      m_filename = "./data/" + filename + ".raw";
      #endif // WIN32

    }

    saveModel();
  }
  else if (e.m_key == GLFW_KEY_K && e.m_action == GLFW_PRESS)
  {
    std::cout << "enter file path:" << std::endl;
    std::cin >> m_filename;
    resetModel();
    loadModel();
  }
  else if (e.m_key == GLFW_KEY_R && e.m_action == GLFW_PRESS)
  {
    // int access = 0;
    // std::cout << "Enter 1 to process your reset or 0 to cancel" << std::endl;
    // std::cin >> access;
    // if (access == 1){
    //   resetModel();
    // }
    // else if (access == 0){
    //   std::cout << "Failed resetting" << std::endl;
    // } 
    // else{
    //   std::cout << "Invalid input" << std::endl;
    // }
    resetModel();
  }
}


void Model::saveModel()
{
  std::ofstream outFile(m_filename);
  for (int i = 0; i < m_isfilled.size(); i++)
  {
    if(m_isfilled[i])
      outFile  
      << (int)m_cubes[i].m_position.x << " " 
      << (int)m_cubes[i].m_position.y << " "
      << (int)m_cubes[i].m_position.z << " "
      << m_cubes[i].m_color.r << " "
      << m_cubes[i].m_color.g << " "
      << m_cubes[i].m_color.b << "\n";
  }
}

void Model::loadModel()
{
  std::ifstream infile(m_filename);
  float x, y, z, r, g, b;

  bool index_out_range_flag = false;
  while (infile >> x >> y >> z >> r >> g >> b)
  {
    glm::vec3 position = { x,y,z };
    glm::vec3 color = { r,g,b };
    if (addCube(position, color))
      continue;
    else
      index_out_range_flag = true;
  }
  if (index_out_range_flag)
    std::cout << "Warning: model does not fit in current voxel grid. Only part of model loaded" << std::endl;
}

void Model::resetModel()
{
  for (auto x : m_isfilled)
    x = false;
  
  std::vector<GLint> flags(m_res * m_res * m_res, 0);
  glBindBuffer(GL_ARRAY_BUFFER, m_flagBO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(GLint) * m_res * m_res * m_res,
   &flags[0],
   GL_STATIC_DRAW);
}

void Model::setupVAO()
{
  glm::vec3 vertex_positions[] = {
    glm::vec3(0.0, 0.0, 2.0),
    glm::vec3(0.0, 2.0, 2.0),
    glm::vec3(2.0, 2.0, 2.0),
    glm::vec3(2.0, 0.0, 2.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 2.0, 0.0),
    glm::vec3(2.0, 2.0, 0.0),
    glm::vec3(2.0, 0.0, 0.0)
  };

  for (int i = 0; i < 8; i++)
  {
   vertex_positions[i].x /= m_res;
   vertex_positions[i].y /= m_res;
   vertex_positions[i].z /= m_res;
  }

  unsigned int vertex_indices[36] = {
    QUAD(1, 0, 3, 2),
    QUAD(2, 3, 7, 6),
    QUAD(3, 0, 4, 7),
    QUAD(6, 5, 1, 2),
    QUAD(4, 5, 6, 7),
    QUAD(5, 4, 0, 1)
  };

  std::vector<glm::vec3> offsets(m_res*m_res*m_res, glm::vec3(0.0f,0.0f,0.0f));
  std::vector<glm::vec3> colors(m_res*m_res*m_res, glm::vec3(0.0f,0.0f,1.0f));
  std::vector<GLint> flags(m_res*m_res*m_res, 0);

  for (int i = 0; i < m_res; i++)
  {
   for (int j = 0; j < m_res; j++)
   {
    for (int k = 0; k < m_res; k++)
    {
     int index = i * m_res * m_res + j * m_res + k;
     offsets[index].x = 2*(float)i / m_res - 1;
     offsets[index].y = 2*(float)j / m_res - 1;
     offsets[index].z = 2*(float)k / m_res - 1;
    }
   }
  }


  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ibo);
  glGenBuffers(1, &m_offsetBO);
  glGenBuffers(1, &m_colorBO);
  glGenBuffers(1, &m_flagBO);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), &vertex_positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), &vertex_indices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, m_offsetBO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(glm::vec3) * m_res * m_res * m_res,
   &offsets[0],
   GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, m_colorBO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(glm::vec3)*m_res * m_res*m_res,
   &colors[0],
   GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribDivisor(2, 1);

  glBindBuffer(GL_ARRAY_BUFFER, m_flagBO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(GLint) * m_res * m_res * m_res,
   &flags[0],
   GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(3, 1, GL_INT, 0, NULL);
  glVertexAttribDivisor(3, 1);

}