#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>


Object::Object()
{
}


Object::~Object()
{
}

void Object::InitObject(Object* parent = nullptr, Mesh* mesh = nullptr)
{
	m_parent = parent;
	m_linkedMesh = mesh;
	m_updateMatrix = true;
}

void Object::SetParent(Object* parent)
{
	m_parent = parent;
}

Object* Object::GetParent()
{
	return m_parent;
}

void Object::SetMesh(Mesh* mesh)
{
	m_linkedMesh = mesh;
}

Mesh* Object::GetMesh()
{
	return m_linkedMesh;
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
	if (m_updateMatrix)
	{
		m_updateMatrix = false;
		UpdateModelMatrix();
	}
}

void Object::RenderUpdate(float dt)
{

}

void Object::SetPosition(glm::vec3 position)
{
	m_globalPosition = position;
	m_updateMatrix = true;
}

void Object::SetScale(glm::vec3 scale)
{
	m_globalScale = scale;
	m_updateMatrix = true;
}

void Object::SetEulerRotation(glm::vec3 eulerAngles) //Usar roll, pitch y yaw
{
	m_eulerAngles = eulerAngles;
	m_updateMatrix = true;
}

//Para matrices modelo con transformaciones complejas (sesgado, rotar alrededor de un punto, etc)
void Object::UseAlternativeModelMat(bool use)
{
	m_useAltMat = use;
	m_updateMatrix = true;
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

void Object::UpdateModelMatrix()
{
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
