#pragma once
#include "Light.h"

//!  SpotLight class. 
/*!
Class used to define a point light.
*/
class SpotLight :
	public Light
{
public:

	//! Class constructor.
	/*!
	SpotLight class constructor.
	*/
	SpotLight();

	//! Class destructor.
	/*!
	SpotLight class destructor.
	*/
	~SpotLight();

	//! Position setter.
	/*!
	\param position 3D vector to set as new postion.
	*/
	void SetPosition(glm::vec3 position);

	//! Direction setter.
	/*!
	\param direction 3D vector to set as new direction.
	*/
	void SetDirection(glm::vec3 direction);

	//! Attenuation setter.
	/*!
	\param attenuation 3D vector to set as new attenuation.
	*/
	void SetAttenuation(glm::vec3 attenuation);

	//! Cosine CutOff setter.
	/*!
	\param cosCutOff value to set as new Cosine Cutoff.
	*/
	void SetCosCutOff(GLfloat cosCutOff);

	//! Exponent of cone attenuation setter.
	/*!

	Sets the exponent of the attenuation in the cone of the spot light.

	\param spotExponent value to set as new exponent of cone attenuation.
	*/
	void SetSpotExponent(GLfloat spotExponent);
};

