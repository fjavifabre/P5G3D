#include "PointLight.h"


PointLight::PointLight() : Light()
{

	m_cosCutOff = -1.0;
	m_pos = glm::vec4(1.0);
}


PointLight::~PointLight()
{
}

void PointLight::SetPosition(glm::vec3 position)
{
	m_pos.x = position.x;
	m_pos.y = position.y;
	m_pos.z = position.z;
	m_pos.w = 1.0;
}

void PointLight::SetAttenuation(glm::vec3 attenuation)
{
	m_c = attenuation;
}
