#include "PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(float x, float y, float z, float fov, float nearPlane, float farPlane, float width, float height)
{
	this->position = glm::vec3(x, y, z);
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->width = width;
	this->height = height;

	calculateMatrices();
}

glm::mat4 PerspectiveCamera::getProjection()
{
	return projection;
}

glm::mat4 PerspectiveCamera::getView()
{
	return view;
}

void PerspectiveCamera::setPosition(float x, float y, float z)
{
	this->position = glm::vec3(x, y, z);
	calculateViewMatrix();
}

glm::vec3 PerspectiveCamera::getPosition()
{
	return position;
}

void PerspectiveCamera::lookAt(float x, float y, float z)
{
	lookAtPoint = glm::vec3(x, y, z);
	calculateViewMatrix();
}

void PerspectiveCamera::calculateMatrices()
{
	calculateViewMatrix();
	calculateProjectionMatrix();
}

void PerspectiveCamera::calculateViewMatrix()
{
	view = glm::mat4(1.0f);
	view = glm::lookAt(position, lookAtPoint, glm::vec3(0, 1, 0));
}

void PerspectiveCamera::calculateProjectionMatrix()
{
	projection = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
}
