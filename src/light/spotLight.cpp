#include "light/spotLight.h"


SpotLight::SpotLight() : Light()
{

	m_pos = glm::vec4(1.0);
}


SpotLight::~SpotLight()
{
}

void SpotLight::SetPosition(glm::vec3 position)
{
	m_pos.x = position.x;
	m_pos.y = position.y;
	m_pos.z = position.z;
	m_pos.w = 1.0;
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	m_dir.x = direction.x;
	m_dir.y = direction.y;
	m_dir.z = direction.z;
	m_dir.w = 0.0;
	m_dir = glm::normalize(m_dir);
}

void SpotLight::SetAttenuation(glm::vec3 attenuation)
{
	m_c = attenuation;
}

void SpotLight::SetCosCutOff(GLfloat cosCutOff)
{
	m_cosCutOff = cosCutOff;
}

void SpotLight::SetSpotExponent(GLfloat spotExponent)
{
	m_spotExponent = spotExponent;
}
