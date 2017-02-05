#pragma once
#include "Light.h"
class SpotLight :
	public Light
{
public:
	SpotLight();
	~SpotLight();

	void SetPosition(glm::vec3 position);
	void SetDirection(glm::vec3 direction);
	void SetAttenuation(glm::vec3 attenuation);

	void SetCosCutOff(GLfloat cosCutOff);
	void SetSpotExponent(GLfloat spotExponent);
};

