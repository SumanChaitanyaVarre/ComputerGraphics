#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <iostream>
#include "mesh.hpp"

class Torus : public Mesh
{
 int minor_segments, major_segments;
public:
 Torus(
  float minor_radius,
  float major_radius,
  int minor_segments, 
  int major_segments, 
  glm::vec3 color
 );
 void render(glm::mat4 &modelviewprojmatrix, glm::mat4 modelMatrix) const override;
};