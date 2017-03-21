#pragma once
#include "Light.h"

//!  PointLight class. 
/*!
Class used to define a point light. 
*/
class PointLight :
	public Light
{

public:

	//! Class constructor.
	/*!
	PointLight class constructor.
	*/
	PointLight();

	//! Class destructor.
	/*!
	PoijntLight class destructor.
	*/
	~PointLight();

	//! Position setter.
	/*!
	\param position 3D vector to set as new postion.
	*/
	void SetPosition(glm::vec3 position);

	//! Attenuation setter.
	/*!
	\param attenuation 3D vector to set as new attenuation.
	*/
	void SetAttenuation(glm::vec3 attenuation);
	
};

