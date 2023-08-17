#ifndef _TRACK_HPP_
#define _TRACK_HPP_


#include <GL/glew.h>

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "event.hpp"
#include "shader.hpp"

struct ControlPoints
{
 glm::vec3 position;
 float turn;
 float twist;
};
class Track
{
 GLuint m_vao = 0, pos_BO = 0, normal_BO = 0, texcoord_BO = 0, m_ibo = 0;
 std::vector<ControlPoints> m_points;
 unsigned int m_index_editpoint=0;
 unsigned int m_n_subdivs=5;
 float m_scale=1.0;
 std::string m_file;

public:
 Track(std::string file_path);
 void setupVAO();
 //void drawPoints(Shader& shader);
 void draw(Shader &shader);
 void inputHandle(Event& e);
 void loadFromFile();
 void saveToFile();
 void setScale(float scale) { m_scale = scale; };
 glm::mat4 getModelMatrix();
};
#endif