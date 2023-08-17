#include <GL/glew.h>

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "mesh.hpp"

class Sphere: public Mesh
{
 int m_n_lattitude, m_n_longitude;
public:
 Sphere(float radius, int Lats, int Longs, glm::vec3 color);
 void render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const;
};