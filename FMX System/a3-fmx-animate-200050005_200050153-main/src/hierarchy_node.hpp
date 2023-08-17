#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "mesh.hpp"
#include "event.hpp"




class HNode {

	Mesh* m_mesh;
	std::string m_name;

	float m_scale = 1.0f;
	glm::vec3 m_rot = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 m_trans = glm::vec3(0.0, 0.0, 0.0);
		
	std::vector<HNode*> m_children;
	HNode* m_parent;

	glm::mat4 m_transformation;
	void update_matrices();

	public:

	//HNode(HNode*, GLuint, glm::vec4*, glm::vec4*, std::size_t, std::size_t);
	HNode(HNode* parent, Mesh* mesh, std::string name);
	glm::mat4 getTransformationMatrix() { return m_transformation; };
	void printData();
	void set_parent(HNode* parent);
	void add_child(HNode*);
	void set_scale(float scale);

	float get_scale() { return m_scale; };
	glm::vec3 get_rot() { return m_rot; };
	glm::vec3 get_trans() { return m_trans; };
	
	void render(const glm::mat4& viewProjMatrix);
	void change_parameters(glm::vec3 trans, glm::vec3 rot);
	void render_tree(const glm::mat4& viewProjMatrix);
	void inc_rx(float deg);
	void inc_ry(float deg);
	void inc_rz(float deg);
	void inc_tx(float val);
	void inc_ty(float val);
	void inc_tz(float val);
	void inputHandle(Event &e);

};

glm::mat4* multiply_stack(std::vector <glm::mat4> );

#endif