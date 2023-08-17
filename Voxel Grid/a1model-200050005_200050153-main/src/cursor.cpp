#include "cursor.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define QUAD(a, b, c, d) a, b, c, a, c, d

const glm::mat4 Cursor::getModelMatrix()
{

 glm::vec3 transvec = { -(float)m_res / 2,-(float)m_res / 2,-(float)m_res / 2 };
 return glm::translate(glm::mat4(1.0f), (m_position + transvec) / (float)m_res * 2.0f);
}

void Cursor::setupVAO()
{
  
 //TODO make this dependant on resolution
  glm::vec3 vertex_positions[] = {
    glm::vec3(-0.1, -0.1, 2.1),
    glm::vec3(-0.1, 2.1, 2.1),
    glm::vec3(2.1, 2.1, 2.1),
    glm::vec3(2.1, -0.1, 2.1),
    glm::vec3(-0.1, -0.1, -0.1),
    glm::vec3(-0.1, 2.1, -0.1),
    glm::vec3(2.1, 2.1, -0.1),
    glm::vec3(2.1, -0.1, -0.1)
  };
 
  for (int i =0;i<8;i++)
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

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ibo);
  glBindVertexArray(m_vao);
    
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), &vertex_positions, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), &vertex_indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

const void Cursor::draw()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Cursor::inputHandle(Event &e)
{
  int key = e.m_key;
  int action = e.m_action;
  int mods = e.m_mods;
  if (key == GLFW_KEY_X && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_position.x = ((int)m_position.x + 1) % m_res;
    else if (mods == GLFW_MOD_SHIFT)
      m_position.x = ((int)m_position.x - 1 + m_res) % m_res;
    else if (mods == GLFW_MOD_ALT)
    {
      m_position.x = ((int)m_position.x + 1) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
    else if (mods == (GLFW_MOD_ALT | GLFW_MOD_SHIFT))
    {
      m_position.x = ((int)m_position.x - 1 + m_res) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
  }
  else if (key == GLFW_KEY_Y && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_position.y = ((int)m_position.y + 1) % m_res;
    else if (mods == GLFW_MOD_SHIFT)
      m_position.y = ((int)m_position.y - 1 + m_res) % m_res;
    else if (mods == GLFW_MOD_ALT)
    {
      m_position.y = ((int)m_position.y + 1) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
    else if (mods == (GLFW_MOD_ALT | GLFW_MOD_SHIFT))
    {
      m_position.y = ((int)m_position.y - 1 + m_res) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
  }
  else if (key == GLFW_KEY_Z && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_position.z = ((int)m_position.z + 1) % m_res;
    else if (mods == GLFW_MOD_SHIFT)
      m_position.z = ((int)m_position.z - 1 + m_res) % m_res;
    else if (mods == GLFW_MOD_ALT)
    {
      m_position.z = ((int)m_position.z + 1) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
    else if (mods == (GLFW_MOD_ALT | GLFW_MOD_SHIFT))
    {
      m_position.z = ((int)m_position.z - 1 + m_res) % m_res;
      m_model_to_edit.addCube(m_position, m_draw_color);
    }
  }
  else if(key == GLFW_KEY_P && action == GLFW_PRESS)
  {
    m_model_to_edit.addCube(m_position, m_draw_color);
  }
  else if (key == GLFW_KEY_D && action == GLFW_PRESS)
  {
    m_model_to_edit.deleteCube(m_position);
  }
  else if (key == GLFW_KEY_C && action == GLFW_PRESS)
  {
    changeColor();
  }
}

void Cursor::changeColor()
{
  std::cout << "Enter color" << std::endl;
  
  std::cout << "R: " << std::endl;
  while(!(std::cin >> m_draw_color.r && m_draw_color.r <= 1.0f && m_draw_color.r >= 0.0f))
  {
    std::cout << "invalid value. Try again" << std::endl;
    std::cout << "R: " << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::cout << "G: " << std::endl;
  while(!(std::cin >> m_draw_color.g && m_draw_color.g <= 1.0f && m_draw_color.g >= 0.0f))
  {
    std::cout << "invalid value. Try again" << std::endl;
    std::cout << "G: " << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::cout << "B: " << std::endl;
  while(!(std::cin >> m_draw_color.b && m_draw_color.b <= 1.0f && m_draw_color.b >= 0.0f))
  {
    std::cout << "invalid value. Try again" << std::endl;
    std::cout << "B: " << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}