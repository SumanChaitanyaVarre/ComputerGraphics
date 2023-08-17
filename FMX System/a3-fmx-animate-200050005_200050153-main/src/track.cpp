#include "track.hpp"
#include "window.hpp"
#include <fstream>
#define M_PI 3.14159265358979323846 /* pi */
#include "glm/gtx/rotate_vector.hpp"

Track::Track(std::string file_path)
{
  m_file = file_path;
}

void Track::setupVAO()
{
  // TODO specify size here
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texcoords;
  std::vector<unsigned int> indices;

  unsigned int N = m_n_subdivs; // no of quads in a row
  for (unsigned int i = 0; i < m_points.size() - 1; i++)
  {
    auto dir = glm::rotateY(glm::vec3(0.2f, 0.0f, 0.0f), m_points[i].turn);
    glm::vec3 u1 = m_points[i].position + dir;
    glm::vec3 v1 = m_points[i].position - dir;
    dir = glm::rotateY(glm::vec3(0.2f, 0.0f, 0.0f), m_points[i + 1].turn);
    glm::vec3 u2 = m_points[i + 1].position + dir;
    glm::vec3 v2 = m_points[i + 1].position - dir;

    for (unsigned int j = 0; j <= N; j++)
    {
      float f1 = (float)j / (float)N;
      auto p1 = u1 * f1 + u2 * (1 - f1);
      auto p2 = v1 * f1 + v2 * (1 - f1);
      for (unsigned int k = 0; k <= N; k++)
      {
        float f2 = (float)k / (float)N;
        positions.push_back(p1 * f2 + p2 * (1 - f2));
        normals.push_back(glm::normalize(glm::cross(dir, (u1-u2))));
        texcoords.push_back(glm::vec2(f2, f1));
      }
    }
  }
  for (unsigned int i = 0; i < m_points.size() - 1; i++)
  {
    for (unsigned int j = 0; j < N; j++)
    {
      for (unsigned int k = 0; k < N; k++)
      {
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * j + k);
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * (j + 1) + k);
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * j + k + 1);
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * j + k + 1);
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * (j + 1) + k);
        indices.push_back((N + 1) * (N + 1) * i + (N + 1) * (j + 1) + k + 1);
      }
    }
  }

  glDeleteVertexArrays(1, &m_vao);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  //positions
  glGenBuffers(1, &pos_BO);
  glBindBuffer(GL_ARRAY_BUFFER, pos_BO);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(glm::vec3) * (N + 1) * (N + 1) * m_points.size(),
      &positions[0],
      GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  //normals
  glGenBuffers(1, &normal_BO);
  glBindBuffer(GL_ARRAY_BUFFER, normal_BO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(glm::vec3) * (N + 1) * (N + 1) * m_points.size(),
   &normals[0],
   GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  //texcoords
  glGenBuffers(1, &texcoord_BO);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_BO);
  glBufferData(
   GL_ARRAY_BUFFER,
   sizeof(glm::vec2) * (N + 1) * (N + 1) * m_points.size(),
   &texcoords[0],
   GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  //indices
  glGenBuffers(1, &m_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(GLuint) * 6 * N * N * (m_points.size() - 1),
      &indices[0],
      GL_STATIC_DRAW);
}

void Track::draw(Shader &shader)
{
  shader.use();

  glBindVertexArray(m_vao);
  glDrawElements(
   GL_TRIANGLES,
   6 * m_n_subdivs * m_n_subdivs * (m_points.size() - 1),
   GL_UNSIGNED_INT,
   0
  );
}

void Track::inputHandle(Event &e)
{
  int key = e.m_key;
  int action = e.m_action;
  int mods = e.m_mods;
  if (key == GLFW_KEY_N && action == GLFW_PRESS)
  {
    auto point = m_points[m_points.size() - 1];
    m_points.push_back(point);
    m_index_editpoint++;
    setupVAO();
  }
  else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    saveToFile();
  else if (key == GLFW_KEY_L && action == GLFW_PRESS)
  {
    loadFromFile();
    setupVAO();
  }
  else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    m_index_editpoint = (m_index_editpoint + 1) % m_points.size();
  else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    m_index_editpoint = (m_index_editpoint - 1 + m_points.size()) % m_points.size();

  else if (key == GLFW_KEY_X && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_points[m_index_editpoint].position.x += 0.1;
    else if (mods == GLFW_MOD_SHIFT)
      m_points[m_index_editpoint].position.x -= 0.1;
    setupVAO();
  }
  else if (key == GLFW_KEY_Y && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_points[m_index_editpoint].position.y += 0.1;
    else if (mods == GLFW_MOD_SHIFT)
      m_points[m_index_editpoint].position.y -= 0.1;
    setupVAO();
  }
  else if (key == GLFW_KEY_Z && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_points[m_index_editpoint].position.z += 0.1;
    else if (mods == GLFW_MOD_SHIFT)
      m_points[m_index_editpoint].position.z -= 0.1;
    setupVAO();
  }
  else if (key == GLFW_KEY_T && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_points[m_index_editpoint].turn += 0.1;
    else if (mods == GLFW_MOD_SHIFT)
      m_points[m_index_editpoint].turn -= 0.1;
    setupVAO();
  }
  else if (key == GLFW_KEY_P && action != GLFW_RELEASE)
  {
    if (mods == 0)
      m_points[m_index_editpoint].twist += 0.1;
    else if (mods == GLFW_MOD_SHIFT)
      m_points[m_index_editpoint].twist -= 0.1;
    setupVAO();
  }
}

// void Track::drawPoints(Shader& shader)
//{
//  shader.use();
//  glBindVertexArray(m_vao);
//  glDrawArrays(GL_POINTS, 0, 2 * m_points.size());
// }

void Track::loadFromFile()
{
  std::ifstream infile(m_file);
  if (!infile.is_open())
    throw std::runtime_error("Cannot find file: " + m_file);
  float x, y, z, turn, twist;
  m_points.clear();
  while (infile >> x >> y >> z >> turn >> twist)
  {
    glm::vec3 position = {x, y, z};
    turn *= M_PI / 180;
    twist *= M_PI / 180;
    m_points.push_back({position, turn, twist});
  }
  m_index_editpoint = m_points.size() - 1;
}

void Track::saveToFile()
{
  std::ofstream outFile(m_file);
  for (int i = 0; i < m_points.size(); i++)
  {
    outFile
        << m_points[i].position.x << " "
        << m_points[i].position.y << " "
        << m_points[i].position.z << " "
        << m_points[i].turn * (180 / M_PI) << " "
        << m_points[i].twist * (180 / M_PI) << "\n";
  }
}

glm::mat4 Track::getModelMatrix()
{
 return glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
}
