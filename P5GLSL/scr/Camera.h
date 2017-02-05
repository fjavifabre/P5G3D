#pragma once
#include "Object.h"

//!  Camera class. 
/*!
Class used to control a camera. Contains essential functions like position, rotation and matrix projections
*/

class Camera :
	public Object
{
public:
	//! Class constructor.
	/*!
	Camera class constructor.
	*/
	Camera();

	//! Class destructor.
	/*!
	Camera class destructor.
	*/
	~Camera();

	//! Perspective projection.
	/*!
	Sets the camera with a perspective projection matrix.
		\param fovy Field of view
		\param aspect Aspect ratio
		\param near Near plane
		\param far Far plane
	*/
	void SetPerspProjection(float fovy, float aspect, float near, float far);


	//! Orthographic projection.
	/*!
	Sets the camera with an orthographic projection matrix.
	\param left Left camera frustum plane
	\param right Right camera frustum plane
	\param bottom Bottom camera frustum plane
	\param top Top camera frustum plane
	\param near Near plane
	\param far Far plane
	*/
	void SetOrthoProjection(float left, float right, float bottom, float top, float near, float far);
	
	void SetViewMatrix(glm::mat4 view);

	//! Returns the projection matrix.
	/*!
	Returns the selected camera projection matrix (perspective or orthographic).
	*/
	glm::mat4* GetProjection();

	//! Returns the view matrix.
	/*!
	Returns the camera view matrix.
	*/
	glm::mat4* GetView();


private:

	//! Camera projection matrix.
	/*!
	Camera projection matrix.
	*/
	glm::mat4 m_projection;
};

