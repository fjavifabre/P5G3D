#pragma once
#include <glm\glm.hpp>
#include <gl/freeglut.h> 

class Light
{
public:
	Light();
	~Light();


	//¿Cambiar shader para utilizar componente especular distinta o juntarlo todo en solo color de la luz?
	glm::vec3 AmbientColor; // Intesidad ambiental
	glm::vec3 DiffuseColor; // Intensidad difusa

	//pos.w: 0 -> direccional, 1 -> puntual, focal
	//cosCutOff == -1 -> direccional y puntal , != focal

	glm::vec4* GetPosition();
	glm::vec4* GetDirection();
	glm::vec3* GetAttenuation();
	GLfloat GetCosCutOff();
	GLfloat GetSpotExponent();

protected:
	//Identificadores
	int uAmb;
	int uDiff;
	int uPos;
	int uDir;
	int uC;
	int uCosCutOff;
	int uSpotExponent;

	//Valores
	glm::vec4 m_pos; // Posición
	glm::vec4 m_dir; // Dirección
	glm::vec3 m_c; // Atenuación
	GLfloat m_cosCutOff;
	GLfloat m_spotExponent;

};

