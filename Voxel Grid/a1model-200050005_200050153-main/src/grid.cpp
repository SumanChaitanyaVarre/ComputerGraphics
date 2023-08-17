#include "grid.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Grid::setupVAO()
{
  std::vector<Line> points(3 * (m_n_subdivs + 1) * (m_n_subdivs + 1));
  int count = -1;
  Line line;
  for (int y = 0; y <= m_n_subdivs; y++) {
    for (int z = 0; z <= m_n_subdivs; z++) {
      line.start = glm::vec3(-0.5, (float)y / m_n_subdivs - 0.5, (float)z / m_n_subdivs - 0.5);
      line.end = glm::vec3(0.5, (float)y / m_n_subdivs - 0.5, (float)z / m_n_subdivs - 0.5);
      points[++count] = line;
    }
  }
  for (int x = 0; x <= m_n_subdivs; x++) {
    for (int z = 0; z <= m_n_subdivs; z++) {
      line.start = glm::vec3((float)x / m_n_subdivs - 0.5, -0.5, (float)z / m_n_subdivs - 0.5);
      line.end = glm::vec3((float)x / m_n_subdivs - 0.5, 0.5, (float)z / m_n_subdivs - 0.5);
      points[++count] = line;
    }
  }
  for (int x = 0; x <= m_n_subdivs; x++) {
    for (int y = 0; y <= m_n_subdivs; y++) {
      line.start = glm::vec3((float)x / m_n_subdivs - 0.5, (float)y / m_n_subdivs - 0.5, 0.5);
      line.end = glm::vec3((float)x / m_n_subdivs - 0.5, (float)y / m_n_subdivs - 0.5, -0.5);
      points[++count] = line;
    }
  }
  for (int i = 0; i < points.size(); i++){
    points[i].start *= 2;
    points[i].end *= 2;
  }

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Line), points.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Grid::bindVAO()
{
  glBindVertexArray(m_vao);
}

void Grid::draw()
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_LINES, 0, 6 * (m_n_subdivs + 1) * (m_n_subdivs + 1));
}

void Grid::inputHandle(Event& e)
{

}