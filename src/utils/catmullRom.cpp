#include "utils/catmullRom.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\spline.hpp"


CatmullRom::CatmullRom(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD)
{
	_A = pointA;
	_B = pointB;
	_C = pointC;
	_D = pointD;
}


glm::vec3 CatmullRom::interpolate(float t)
{
	return glm::catmullRom(_A, _B, _C, _D, t);
	/*return 0.5f *((2.0f * _B) +
		(-_A + _C) * t +
		(2.0f * _A - 5.0f * _B + 4.0f * _C - _D) * t*t +
		(-_A + 3.0f * _B - 3.0f * _C + _D) * t*t*t);*/
}

CatmullRom::~CatmullRom()
{
}
