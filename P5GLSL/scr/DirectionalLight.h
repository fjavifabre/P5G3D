#pragma once
#include "Light.h"

//!  DirectionalLight class. 
/*!
Class used to define a directional light.
*/

class DirectionalLight :
	public Light
{
public:

	//! Class constructor.
	/*!
	DirectionalLight class constructor.
	*/
	DirectionalLight();

	//! Class destructor.
	/*!
	DirectionalLight class destructor.
	*/
	~DirectionalLight();

	//! Direction setter.
	/*!
	\param direction 3D vector to set as new direction.
	*/
	void SetDirection(glm::vec3 position);
};

