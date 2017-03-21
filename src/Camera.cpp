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


void Camera::SetViewMatrix(glm::mat4 view)
{
	m_altModelMat = view;

	m_modelMat = view;
}

glm::mat4* Camera::GetView()
{
	return GetModelMatrix(); //TODO no entiendo esto de aqui realmente
}