#pragma once
#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "event.hpp"
#include "model.hpp"
#include <GLFW/glfw3.h>
#include <functional>


class Cursor
{
private:
  GLuint m_vao, m_vbo, m_ibo;
  glm::vec3 m_position;
  glm::vec3 m_draw_color;
  unsigned int m_res;

  Model& m_model_to_edit;
  //std::function<void(glm::vec3 pos, glm::vec3 col)> m_add_cube;

public:

  Cursor(unsigned int res, Model& model_to_edit) :
    m_vao(0),
    m_vbo(0),
    m_ibo(0), 
    m_position({ 0.0,0.0,0.0 }),
    m_draw_color({ 1.0,0.0,0.0 }),
    m_res(res),
    m_model_to_edit(model_to_edit)
  {}
  //void setAddCubeFunction(std::function<void(glm::vec3 pos, glm::vec3 col)> func)
  //{
  //  m_add_cube = func;
  //}
  void setupVAO();
  //void cube_vertices();
  const glm::mat4 getModelMatrix();
  void changeColor();
  void inputHandle(Event& e);
  //void quad(int a, int b, int c, int d);
  const void draw();
};