#include "hierarchy_node.hpp"
#include <iomanip>


HNode::HNode(HNode *parent, Mesh *mesh, std::string name):
	m_parent(parent), m_mesh(mesh), m_name(name)
{

	if (parent != NULL)
		parent->add_child(this);
	update_matrices();
}

void HNode::update_matrices()
{
	m_transformation = glm::mat4(1.0f);


	if(m_parent!=NULL)
		m_transformation = m_parent->getTransformationMatrix();

	m_transformation = glm::translate(m_transformation, m_trans);
	m_transformation = glm::rotate(m_transformation, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_transformation = glm::rotate(m_transformation, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_transformation = glm::rotate(m_transformation, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_transformation = glm::scale(m_transformation, glm::vec3(m_scale));

	for (auto child : m_children)
	{
		child->update_matrices();
	}
}

void HNode::add_child(HNode *child)
{
	m_children.push_back(child);
	child->update_matrices();
}

void HNode::set_scale(float scale)
{
	m_scale = scale;
	update_matrices();
}

void HNode::change_parameters(glm::vec3 trans, glm::vec3 rot)
{
	m_rot = rot;
	m_trans = trans;
	update_matrices();
}

void HNode::render(const glm::mat4& viewProjMatrix)
{
	if (m_mesh == NULL)
		return;
	
	auto modelMat = getTransformationMatrix();
	auto modelViewProjMat = viewProjMatrix * modelMat;
	m_mesh->render(modelViewProjMat, modelMat);
}

void HNode::render_tree(const glm::mat4& viewProjMatrix)
{
	render(viewProjMatrix);
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->render_tree(viewProjMatrix);
	}
}

void HNode::inc_rx(float deg)
{
	m_rot.x += deg;
	update_matrices();
}

void HNode::inc_ry(float deg)
{
	m_rot.y += deg;
	update_matrices();
}

void HNode::inc_rz(float deg)
{
	m_rot.z += deg;
	update_matrices();
}

void HNode::inc_tx(float val)
{
	m_trans.x += val;
	update_matrices();
}

void HNode::inc_ty(float val)
{
	m_trans.y += val;
	update_matrices();
}

void HNode::inc_tz(float val)
{
	m_trans.z += val;
	update_matrices();
}

void HNode::inputHandle(Event& e)
{
	int key = e.m_key;
	int action = e.m_action;
	int mods = e.m_mods;

	if (key == GLFW_KEY_U && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_rx(1);
	else if (key == GLFW_KEY_U && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_rx(-1);
	else if (key == GLFW_KEY_I && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ry(1);
	else if (key == GLFW_KEY_I && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ry(-1);
	else if (key == GLFW_KEY_O && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_rz(1);
	else if (key == GLFW_KEY_O && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_rz(-1);
	else if (key == GLFW_KEY_H && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tx(0.1 * m_scale);
	else if (key == GLFW_KEY_H && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tx(-0.1 * m_scale);
	else if (key == GLFW_KEY_J && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ty(0.1 * m_scale);
	else if (key == GLFW_KEY_J && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ty(-0.1 * m_scale);
	else if (key == GLFW_KEY_K && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tz(0.1 * m_scale);
	else if (key == GLFW_KEY_K && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tz(-0.1 * m_scale);
}

void HNode::printData()
{
	std::cout << "Entity Name: " << std::left << std::setw(27);
	std::cout <<	m_name << " (<,) and (>.) keys to change  " << std::endl;
	
	std::cout << "Trans: "
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << m_trans.x
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << m_trans.y
		<< std::left << std::setw(14) << std::fixed << std::setprecision(4) << m_trans.z
		<< "H,J,K+(shift) to change" << std::endl;

	std::cout << "Rot:   "
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << m_rot.x
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << m_rot.y
		<< std::left << std::setw(14) << std::fixed << std::setprecision(4) << m_rot.z
		<< "U,I,O+(shift) to change" <<std::endl;
}

void HNode::set_parent(HNode* parent)
{
	m_parent = parent;
	parent->add_child(this);
	update_matrices();
}