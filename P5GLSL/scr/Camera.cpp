#include "Camera.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
	m_useAltMat = true;
}


Camera::~Camera()
{
}

void Camera::SetPerspProjection(float fovy, float aspect, float near, float far)
{
	m_projection = glm::perspective(fovy, aspect, near, far);
}

void Camera::SetOrthoProjection(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
}

glm::mat4* Camera::GetProjection()
{
	return &m_projection;
}

glm::mat4* Camera::GetView()
{
	return GetModelMatrix();
}