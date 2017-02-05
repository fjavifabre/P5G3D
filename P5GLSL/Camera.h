#pragma once
#include "Object.h"


class Camera :
	public Object
{
public:
	Camera();
	~Camera();

	void SetPerspProjection(float fovy, float aspect, float near, float far);
	void SetOrthoProjection(float left, float right, float bottom, float top, float near, float far);
	glm::mat4* GetProjection();
	glm::mat4* GetView();


private:
	glm::mat4 m_projection;
};

