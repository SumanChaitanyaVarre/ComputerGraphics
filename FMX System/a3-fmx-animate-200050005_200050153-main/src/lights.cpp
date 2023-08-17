#include <iostream>
#include <iomanip>

#include "lights.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"


Light::Light(std::string name, glm::vec3 position):
	name(name), position(position)
{}
Light::Light(std::string name, glm::vec3 position, glm::vec3 rotation):
	name(name), position(position), rotation(rotation)
{}

void Light::inputHandle(Event& e)
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
		inc_tx(0.1);
	else if (key == GLFW_KEY_H && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tx(-0.1);
	else if (key == GLFW_KEY_J && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ty(0.1);
	else if (key == GLFW_KEY_J && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_ty(-0.1);
	else if (key == GLFW_KEY_K && mods != GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tz(0.1);
	else if (key == GLFW_KEY_K && mods == GLFW_MOD_SHIFT && action != GLFW_RELEASE)
		inc_tz(-0.1);

	if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
		on = 1 - on;

}

void Light::inc_rx(float deg) { rotation.x += deg; }

void Light::inc_ry(float deg) { rotation.y += deg; }

void Light::inc_rz(float deg) { rotation.z += deg; }

void Light::inc_tx(float val) { position.x += val; }

void Light::inc_ty(float val) { position.y += val; }

void Light::inc_tz(float val) { position.z += val; }

void Light::printData()
{
	std::cout << "Entity Name: " << std::left << std::setw(15);
	std::cout << name << std::left << std::setw(12);
	
	if (on)
		std::cout << "ON";
	else
		std::cout << "OFF";
	std::cout	<< " N key to change | '/' key to switch ON/OFF" << std::endl;

	std::cout << "Trans: "
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << position.x
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << position.y
		<< std::left << std::setw(14) << std::fixed << std::setprecision(4) << position.z
		<< "H,J,K+(shift) to change" << std::endl;

	std::cout << "Rot:   "
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << rotation.x
		<< std::left << std::setw(10) << std::fixed << std::setprecision(4) << rotation.y
		<< std::left << std::setw(14) << std::fixed << std::setprecision(4) << rotation.z
		<< "U,I,O+(shift) to change" << std::endl;
}

glm::vec3 HeadLight::getPosition()
{
	auto transMat = glm::mat4(1.0f);
	if (m_parent != NULL)
		transMat = m_parent->getTransformationMatrix();

	return glm::vec3(transMat* glm::vec4(position, 1));
}

glm::vec3 HeadLight::getDirection()
{
	auto trans = glm::mat4(1.0f);

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 direction = glm::vec3(trans * glm::vec4(0, -1, 0, 1));

	auto transMat = glm::mat4(1.0f);
	if (m_parent != NULL)
		transMat = m_parent->getTransformationMatrix();

	return glm::normalize(glm::vec3(transMat * glm::vec4(direction, 0)));
}

glm::vec3 FollowLight::getDirection()
{
	auto target_pos = glm::vec3(0,0,0);
	if(m_target!=NULL)
		target_pos = glm::vec3(m_target->getTransformationMatrix() * glm::vec4(0, 0, 0, 1));

	return glm::normalize(target_pos-position);
}

float FollowLight::getInnerCutoff()
{
	auto target_pos = glm::vec3(0, 0, 0);
	if (m_target != NULL)
		target_pos = glm::vec3(m_target->getTransformationMatrix() * glm::vec4(0, 0, 0, 1));

	float x = glm::distance(target_pos, position);
	return sqrt(1 - 0.05 / (x * x));
}

float FollowLight::getOuterCutoff()
{
	auto target_pos = glm::vec3(0, 0, 0);
	if (m_target != NULL)
		target_pos = glm::vec3(m_target->getTransformationMatrix() * glm::vec4(0, 0, 0, 1));

	float x = glm::distance(target_pos, position);
	return sqrt(1 - 0.1 / (x * x));
}

glm::vec3 SpotLight::getDirection()
{
	auto trans = glm::mat4(1.0f);

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	return glm::vec3(trans * glm::vec4(0, -1, 0, 1));
}
