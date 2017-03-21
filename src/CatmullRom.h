#pragma once

#include <glm\glm.hpp>

class CatmullRom
{
public:
	CatmullRom(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD);
	~CatmullRom();

	glm::vec3 interpolate(float t);

private:
	glm::vec3 _A;
	glm::vec3 _B;
	glm::vec3 _C;
	glm::vec3 _D;
};

