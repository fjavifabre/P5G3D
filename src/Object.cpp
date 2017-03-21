#include "Object.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

Object::Object()
{
	m_globalPosition = glm::vec3(0.0f);
	m_globalScale = glm::vec3(1.0f);
	m_eulerAngles = glm::vec3(0.0f);
}


Object::~Object()
{
}

void Object::InitObject(char * name, Object* parent = nullptr, Mesh *mesh = nullptr)
{
	m_name = name;
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

glm::mat4* Object::GetModelMatrix()
{
	if (m_useAltMat)
		return &m_altModelMat;
	else
		return &m_modelMat;
}

//Ejecuta en bucle de lógica
void Object::Update(float dt) 
{
	if (!m_active)
		return;
	if (m_updateMatrix)
	{
		m_updateMatrix = false;
		UpdateMatrix();
	}
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

void Object::UpdateMatrix()
{
	m_modelMat = glm::mat4(1.0f);

	if (m_parent != nullptr)
		m_modelMat = *m_parent->GetModelMatrix();

	if (!m_useAltMat)
	{
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
}
