#pragma once
#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "event.hpp"
#include <GLFW/glfw3.h>


struct Line
{
  glm::vec3 start;
  glm::vec3 end;
};

class Grid
{
private: 
  GLuint m_vao;
  GLuint m_vbo;
  unsigned int m_n_subdivs;

public:
  Grid(int n):m_n_subdivs(n), m_vao(0), m_vbo(0){}
  
  void inputHandle(Event& e);

  void setupVAO();
  void bindVAO();

  void draw();
};