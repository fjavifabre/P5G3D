#include "DirectionalLight.h"


DirectionalLight::DirectionalLight() : Light()
{

	m_cosCutOff = -1.0;
	m_pos = glm::vec4(1.0);
	m_pos.w = 0.0;
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_pos.x = direction.x;
	m_pos.y = direction.y;
	m_pos.z = direction.z;

	m_dir.x = -direction.x;
	m_dir.y = -direction.y;
	m_dir.z = -direction.z;
	m_dir.w = 0.0;
	m_dir = glm::normalize(m_dir);
}
