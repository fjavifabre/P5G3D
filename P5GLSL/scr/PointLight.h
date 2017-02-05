#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();
	~PointLight();

	void SetPosition(glm::vec3 position);
	void SetAttenuation(glm::vec3 attenuation);
	
};

