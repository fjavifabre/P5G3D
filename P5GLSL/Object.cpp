#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>


Object::Object()
{
}


Object::~Object()
{
}

void Object::InitObject()
{
	m_modelMat = glm::mat4(1.0f);
	m_normalMat = glm::inverse(glm::transpose(m_modelMat));
}

glm::mat4 Object::GetModelMatrix()
{
	return m_modelMat;
}

glm::mat4 Object::GetNormalMatrix()
{
	return m_normalMat;
}


glm::mat4 Object::GetPosMatrix()
{
	return m_posMat;
}

glm::mat4 Object::GetRotMatrix()
{
	return m_rotMat;
}

glm::mat4 Object::GetScaleMatrix()
{
	return m_scaleMat;
}

//Ejecuta en bucle de lógica
void Object::Update(float dt) 
{
	if (!m_active)
		return;

	if (!m_useAltMat)
	{
		m_modelMat = glm::mat4(1.0f);
		m_modelMat = glm::translate(m_modelMat, m_globalPosition);
		m_modelMat = glm::rotate(m_modelMat, m_eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_modelMat = glm::rotate(m_modelMat, m_eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMat = glm::rotate(m_modelMat, m_eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMat = glm::scale(m_modelMat, m_globalScale);
	}
	else
	{
		m_modelMat = m_altModelMat;
	}

	m_normalMat = glm::inverse(glm::transpose(m_modelMat));
}

void Object::SetPosition(glm::vec3 position)
{
	m_globalPosition = position;
}

void Object::SetScale(glm::vec3 scale)
{
	m_globalScale = scale;
}

void Object::SetEulerRotation(glm::vec3 eulerAngles) //Usar rol, pitch y yaw
{
	m_eulerAngles = eulerAngles;
}


//Para matrices modelo con transformaciones complejas (sesgado, rotar alrededor de un punto, etc)
void Object::UseAlternativeModelMat(bool use)
{
	m_useAltMat = use;
}

void Object::SetAlternativeModelMat(glm::mat4 modelMat)
{
	m_altModelMat = modelMat;
}


bool Object::SetActive(bool active)
{
	m_active = active;
	return m_active;
}

bool Object::IsActive()
{
	return m_active;
}
