#pragma once
#include <glm\glm.hpp>
//#include "Mesh.h"

class Mesh;

class Object
{
public:
	Object();
	~Object();

	void InitObject(Object *parent, Mesh *mesh);

	void SetParent(Object *parent);
	Object* GetParent();

	void SetMesh(Mesh *mesh);
	Mesh* GetMesh();

	glm::mat4 GetModelMatrix();
	glm::mat4 GetNormalMatrix();

	glm::mat4 GetPosMatrix();
	glm::mat4 GetRotMatrix();
	glm::mat4 GetScaleMatrix();

	virtual void Update(float dt); //Metodo preparado para ser overrided, así se pueden crear nuevas clases de objetos con comportamientos especiales

	virtual void RenderUpdate(float dt);

	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	void SetEulerRotation(glm::vec3 eulerAngles); //Usar roll, pitch y yaw

	//Para matrices modelo con transformaciones complejas (sesgado, rotar alrededor de un punto, etc)
	void UseAlternativeModelMat(bool use);
	void SetAlternativeModelMat(glm::mat4 modelMat); 

	bool SetActive(bool active);
	bool IsActive();

private:
	Object* m_parent; //¿Crear herencia entre objetos para heredar transformaciones?
	Mesh* m_linkedMesh;

	bool m_active; //Si no está activo el objeto sigue en memoria, pero no se dibuja ni actualiza

	glm::vec3 m_globalPosition;
	glm::vec3 m_globalScale;
	glm::vec3 m_eulerAngles;
	
	glm::mat4 m_posMat;
	glm::mat4 m_scaleMat;
	glm::mat4 m_rotMat;

	glm::mat4 m_modelMat;
	glm::mat4 m_altModelMat;
	glm::mat4 m_normalMat;

	bool m_useAltMat = false;
	bool m_updateMatrix = false;

	void UpdateMatrix();
};

