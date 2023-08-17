#ifndef _BIKE_HPP_
#define _BIKE_HPP_

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "hierarchy_node.hpp"

class Bike
{
private:

public:
 HNode *bnode[16];
 Bike(HNode* parent, Shader& shader);
 void render(glm::mat4& viewProjMatrix);
 HNode* getRootNode() { return bnode[0]; };
};
#endif
