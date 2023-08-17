#include "rider.hpp"
#include "cube.hpp"
#include "sphere.hpp"


Rider::Rider(HNode* parent, Shader& shader)
{
 // Creating the hierarchy:
 Cube* hip=new Cube(glm::vec3(2.2, 1.2, 1.0), glm::vec3(-1.1, -0.6, -0.5), glm::vec3(1, 1, 0));
 Cube* body=new Cube(glm::vec3(2.2, 2.5, 1.0), glm::vec3(-1.1, 0.0, -0.5), glm::vec3(1, 1, 0));
 Cube* neck=new Cube(glm::vec3(1.0, 0.4, 0.5), glm::vec3(-0.5, 0.0, -0.25), glm::vec3(0.99, 0.74, 0.67));
 Cube* hand_lower=new Cube(glm::vec3(0.5, -1.6, 0.6), glm::vec3(-0.25, 0.0, -0.3), glm::vec3(0.99, 0.74, 0.67));
 Cube* hand_upper=new Cube(glm::vec3(0.5, -1.6, 0.6), glm::vec3(-0.26, 0.0, -0.32), glm::vec3(0.99, 0.74, 0.67));
 Cube* head=new Cube(glm::vec3(1.4, 1.4, 1.0), glm::vec3(-0.7, 0.0, -0.5), glm::vec3(0.99, 0.74, 0.67));
 Cube* leg_upper=new Cube(glm::vec3(0.75, -1.8, 0.6), glm::vec3(-0.375, 0.0, -0.3), glm::vec3(0.5, 0.78, 0.8));
 Cube* leg_lower=new Cube(glm::vec3(0.75, -1.8, 0.6), glm::vec3(-0.375, 0.0, -0.3), glm::vec3(0.5, 0.78, 0.8));
 Cube* hair=new Cube(glm::vec3(1.4, 0.28, 1.0), glm::vec3(-0.7, 0.0, -0.5), glm::vec3(0.36, 0.25, 0.2));
 Cube* eyes=new Cube(glm::vec3(0.2, 0.2, 0.2), glm::vec3(-0.1, -0.1, -0.1), glm::vec3(0.0, 0.0, 0.0));
 Cube* shoe=new Cube(glm::vec3(0.75, 0.4, 1.2), glm::vec3(-0.375, -0.2, -0.6), glm::vec3(0.0, 0.0, 0.0));
 Cube* glove=new Cube(glm::vec3(0.5, 0.7, 0.48), glm::vec3(-0.25, -0.7, -0.24), glm::vec3(0.0, 0.0, 0.0));
 Cube* mouth=new Cube(glm::vec3(0.8, 0.12, 0.2), glm::vec3(-0.4, -0.06, -0.1), glm::vec3(0.0, 0.0, 0.0));
 Sphere* helmet=new Sphere(1, 6, 16, glm::vec3(1.0, 0.2, 0.2));

 hip->setShader(shader);
 body->setShader(shader);
 neck->setShader(shader);
 hand_upper->setShader(shader);
 hand_lower->setShader(shader);
 head->setShader(shader);
 leg_upper->setShader(shader);
 leg_lower->setShader(shader);
 hair->setShader(shader);
 eyes->setShader(shader);
 shoe->setShader(shader);
 glove->setShader(shader);
 mouth->setShader(shader);
 helmet->setShader(shader);

 body->setTextureSlot(2);
 hip->setTextureSlot(2);

 // hip
 rnode[0] = new HNode(NULL, hip, "hip");
 rnode[0]->change_parameters(glm::vec3(-0.4, 2.0, 0.0), glm::vec3(0.0, 90.0, 0.0));
 // body
 rnode[1] = new HNode(rnode[0], body, "body");
 rnode[1]->change_parameters(glm::vec3(0.0, 0.3, 0.0), glm::vec3(20.0, 0.0, 0.0));
 // neck
 rnode[2] = new HNode(rnode[1], neck, "neck");
 rnode[2]->change_parameters(glm::vec3(0.0, 2.5, 0.0), glm::vec3(0.0, 0.0, 0.0));
 // head
 rnode[3] = new HNode(rnode[2], head, "head");
 rnode[3]->change_parameters(glm::vec3(0.0, 0.4, 0.0), glm::vec3(-20.0, 0.0, 0.0));
 // handleft
 rnode[4] = new HNode(rnode[1], hand_upper, "upper_left_hand");
 rnode[4]->change_parameters(glm::vec3(1.1, 2.5, 0.0), glm::vec3(-60.0, 0.0, 0.0));
 rnode[5] = new HNode(rnode[4], hand_lower, "lower_left_hand");
 rnode[5]->change_parameters(glm::vec3(0.0, -1.6, 0.0), glm::vec3(-60.0, 0.0, 0.0));
 // handright
 rnode[6] = new HNode(rnode[1], hand_upper, "upper_right_hand");
 rnode[6]->change_parameters(glm::vec3(-1.1, 2.5, 0.0), glm::vec3(-60.0, 0.0, 0.0));
 rnode[7] = new HNode(rnode[6], hand_lower, "lower_right_hand");
 rnode[7]->change_parameters(glm::vec3(0.0, -1.6, 0.0), glm::vec3(-60.0, 0.0, 0.0));
 // legleft
 rnode[8] = new HNode(rnode[0], leg_upper, "upper_left_leg");
 rnode[8]->change_parameters(glm::vec3(0.625, -0.05, 0.0), glm::vec3(-70.0, 0.0, 0.0));
 rnode[9] = new HNode(rnode[8], leg_lower, "lower_left_leg");
 rnode[9]->change_parameters(glm::vec3(0.0, -1.5, 0.0), glm::vec3(70.0, 0.0, 0.0));
 // legright
 rnode[10] = new HNode(rnode[0], leg_upper, "upper_right_leg");
 rnode[10]->change_parameters(glm::vec3(-0.625, -0.05, 0.0), glm::vec3(-70.0, 0.0, 0.0));
 rnode[11] = new HNode(rnode[10], leg_lower, "lower_right_leg");
 rnode[11]->change_parameters(glm::vec3(0.0, -1.5, 0.0), glm::vec3(70.0, 0.0, 0.0));
 // hair
 rnode[12] = new HNode(rnode[3], hair, "hair");
 rnode[12]->change_parameters(glm::vec3(0.0, 1.4, 0.0), glm::vec3(0.0, 0.0, 0.0));
 // eyes
 rnode[13] = new HNode(rnode[3], eyes, "eye_left");
 rnode[13]->change_parameters(glm::vec3(0.35, 1.0, 0.52), glm::vec3(0.0, 0.0, 0.0));
 rnode[14] = new HNode(rnode[3], eyes, "eye_right");
 rnode[14]->change_parameters(glm::vec3(-0.35, 1.0, 0.52), glm::vec3(0.0, 0.0, 0.0));
 // shoes
 rnode[15] = new HNode(rnode[9], shoe, "shoe_left");
 rnode[15]->change_parameters(glm::vec3(0.0, -2.0, 0.26), glm::vec3(0.0, 0.0, 0.0));
 rnode[16] = new HNode(rnode[11], shoe, "shoe_right");
 rnode[16]->change_parameters(glm::vec3(0.0, -2.0, 0.26), glm::vec3(0.0, 0.0, 0.0));
 // gloves
 rnode[17] = new HNode(rnode[5], glove, "glove_left");
 rnode[17]->change_parameters(glm::vec3(0.0, -1.5, 0.0), glm::vec3(0.0, 0.0, 0.0));
 rnode[18] = new HNode(rnode[7], glove, "glove_right");
 rnode[18]->change_parameters(glm::vec3(0.0, -1.5, 0.0), glm::vec3(0.0, 0.0, 0.0));
 // mouth
 rnode[19] = new HNode(rnode[3], mouth, "mouth");
 rnode[19]->change_parameters(glm::vec3(0.0, 0.4, 0.52), glm::vec3(0.0, 0.0, 0.0));
 // helmet
 rnode[20] = new HNode(rnode[3], helmet, "helmet");
 rnode[20]->change_parameters(glm::vec3(0.0, 1.2, 0.0), glm::vec3(-90.0, 0.0, 0.0));

 //root_node = rnode1;
 //curr_node = rnode2;
 //rnode[0]->set_scale(0.5);

}

void Rider::render(glm::mat4& viewProjMatrix)
{
 rnode[0]->render_tree(viewProjMatrix);
}

void Rider::setParent(HNode* parent)
{
 rnode[0]->set_parent(parent);
}




