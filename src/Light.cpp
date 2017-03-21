#include "Light.h"


Light::Light()
{
	AmbientColor = glm::vec3(0.0); // Intesidad ambiental
	DiffuseColor = glm::vec3(0.7); // Intensidad difusa
	m_pos = glm::vec4(1.0); // Posición
	m_dir = glm::vec4(1.0); // Dirección
	m_c = glm::vec3(1.0); // Atenuación
	m_cosCutOff = 0.0;
	m_spotExponent = 0.0;
}


Light::~Light()
{
}

glm::vec4* Light::GetPosition()
{
	return &m_pos;
}

glm::vec4* Light::GetDirection()
{
	return &m_dir;
}

glm::vec3* Light::GetAttenuation()
{
	return &m_c;
}

GLfloat Light::GetCosCutOff()
{
	return m_cosCutOff;
}

GLfloat Light::GetSpotExponent()
{
	return m_spotExponent;
}