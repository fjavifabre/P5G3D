#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	void SetDirection(glm::vec3 position);
};

