#pragma once
#include <glm\glm.hpp>
#include <gl/freeglut.h> 

//!  Light class. 
/*!
Base class for lights. All the lights must inherit from this class.
*/

class Light
{
public:

	//! Class constructor.
	/*!
	Light class constructor.
	*/
	Light();

	//! Class destructor.
	/*!
	Light class destructor.
	*/
	~Light();


	//¿Cambiar shader para utilizar componente especular distinta o juntarlo todo en solo color de la luz?

	//! Ambient color.
	/*!
	Ambient color of the light.
	*/
	glm::vec3 AmbientColor; // Intesidad ambiental

	//! Diffuse color.
	/*!
	Diffuse color of the light.
	*/
	glm::vec3 DiffuseColor; // Intensidad difusa

	//pos.w: 0 -> direccional, 1 -> puntual, focal
	//cosCutOff == -1 -> direccional y puntal , != focal


	//! Light position.
	/*!
	Returns the position of the light.
	*/
	glm::vec4* GetPosition();
	//! Light direction.
	/*!
	Returns the direction of the light.
	*/
	glm::vec4* GetDirection();
	//! Light atternuation.
	/*!
	Returns the atternuation of the light.
	*/
	glm::vec3* GetAttenuation();
	//! Light cosine cut off.
	/*!
	Returns the cosine of the cut off of the light. If it is a directional or a spot light returns -1.
	*/
	GLfloat GetCosCutOff();
	//! Light spot exponent.
	/*!
	Returns the spot exponent of the light. If it is a directional or a spot light returns 0.
	*/
	GLfloat GetSpotExponent();

protected:
	//! Light postion.
	/*!
	Light position.
	*/
	glm::vec4 m_pos; // Posición
	//! Light direction.
	/*!
	Light direction.
	*/
	glm::vec4 m_dir; // Dirección
	//! Light attenuation.
	/*!
	Light attenuation.
	*/
	glm::vec3 m_c; // Atenuación
	//! Light cosine cut off.
	/*!
	Light cosine cut off.
	*/
	GLfloat m_cosCutOff;
	//! Light spot exponent.
	/*!
	Light spot exponent.
	*/
	GLfloat m_spotExponent;

};

