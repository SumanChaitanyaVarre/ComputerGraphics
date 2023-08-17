#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "glm/glm.hpp"
#include "event.hpp"
#include "hierarchy_node.hpp"

#include <string>
#include <vector>

class Light
{
protected:
	glm::vec3 position= glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0,0,0);
public:
	float range=10.0f;
	int on=1;
	std::string name="";
	Light(){};
	Light(std::string name, glm::vec3 position);
	Light(std::string name, glm::vec3 position, glm::vec3 rotation);

	void inputHandle(Event& e);
	void inc_rx(float deg);
	void inc_ry(float deg);
	void inc_rz(float deg);
	void inc_tx(float val);
	void inc_ty(float val);
	void inc_tz(float val);
	void printData();

	virtual glm::vec3 getPosition() { return position; };
	virtual glm::vec3 getDirection() { return glm::vec3(1,0,0); };

};


class PointLight:public Light
{
public:
	PointLight():Light(){};
	PointLight(std::string name, glm::vec3 position):
		Light(name, position){}
};

class SpotLight:public Light
{
protected:
public:
	float innerCutoff = 0.9f;
	float outerCutoff = 0.8f;

	SpotLight():Light(){};
	SpotLight(std::string name, glm::vec3 position) :
		Light(name, position){}
	SpotLight(std::string name, glm::vec3 position, glm::vec3 rotation) :
		Light(name, position, rotation){}

	virtual glm::vec3 getDirection() override;
};

struct HeadLight :public SpotLight
{
	HNode* m_parent=NULL;
public:
	HeadLight() :SpotLight() {};
	HeadLight(std::string name, glm::vec3 position, glm::vec3 rotation) :
		SpotLight(name, position, rotation) {};
	void setParent(HNode* parent) { m_parent = parent; };

	virtual glm::vec3 getPosition() override;
	virtual glm::vec3 getDirection() override;
};

struct FollowLight : public SpotLight
{
	HNode* m_target=NULL;
public:
	FollowLight() :SpotLight() {};
	FollowLight(std::string name, glm::vec3 position) :SpotLight(name, position) {};
	void setTarget(HNode* target) { m_target = target; };

	virtual glm::vec3 getDirection() override;
	float getInnerCutoff();
	float getOuterCutoff();
};


#endif // __LIGHTS_H__