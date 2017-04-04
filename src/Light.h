#pragma once
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

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
	\return the position of the light.
	*/
	glm::vec4* GetPosition();
	//! Light direction.
	/*!
	\return the direction of the light.
	*/
	glm::vec4* GetDirection();
	//! Light atternuation.
	/*!
	\return the atternuation of the light.
	*/
	glm::vec3* GetAttenuation();
	//! Light cosine cut off.
	/*!
	\return the cosine of the cut off of the light. If it is a directional or a spot light \return -1.
	*/
	GLfloat GetCosCutOff();
	//! Light spot exponent.
	/*!
	\return the spot exponent of the light. If it is a directional or a spot light \return 0.
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

