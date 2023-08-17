#ifndef _RIDER_HPP_
#define _RIDER_HPP_

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "hierarchy_node.hpp"

class Rider
{
private:
public:
 HNode *rnode[21];
 Rider(HNode *parent, Shader& shader);
 void render(glm::mat4& viewProjMatrix);
 void setParent(HNode* parent);
};


#endif
