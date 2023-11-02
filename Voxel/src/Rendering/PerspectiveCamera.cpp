#include "PerspectiveCamera.hpp"
#include <cmath>
#include <iostream>

PerspectiveCamera::PerspectiveCamera(float x, float y, float z, float fov, float nearPlane, float farPlane, float width, float height)
{
	this->position = glm::vec3(x, y, z);
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->width = width;
	this->height = height;

	for (int i = 0; i < 6; i++)
		planes.push_back(Plane());
	calculateMatrices();
	calculatePlanes();
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
	calculatePlanes();
}

glm::vec3 PerspectiveCamera::getPosition()
{
	return position;
}

glm::vec3 PerspectiveCamera::getDirection()
{
	return frontDirection;
}

void PerspectiveCamera::setDirection(float x, float y, float z)
{
	frontDirection = glm::normalize(glm::vec3(x, y, z));
}

void PerspectiveCamera::lookAt(float x, float y, float z)
{
	lookAtPoint = glm::vec3(x, y, z);
	calculateViewMatrix();
}

void PerspectiveCamera::lookAtFront()
{
	glm::vec3 vec = position + frontDirection;
	lookAt(vec.x, vec.y, vec.z);
}

void PerspectiveCamera::turn(DIRECTION direction, float degrees)
{
	switch (direction)
	{
	case PerspectiveCamera::LEFT:
	{
		glm::vec4 vec;
		glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), this->upDirection);

		vec = rotMatrix * glm::vec4(frontDirection, 1);
		frontDirection = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
		rightDirection = glm::normalize(glm::cross(frontDirection, upDirection));
	}
	break;
	case PerspectiveCamera::RIGHT:
	{
		glm::vec4 vec;
		glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), -glm::radians(degrees), this->upDirection);

		vec = rotMatrix * glm::vec4(frontDirection, 1);
		frontDirection = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
		rightDirection = glm::normalize(glm::cross(frontDirection, upDirection));
	}
	break;
	case PerspectiveCamera::UP:
	{
		glm::vec4 vec;
		glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), this->rightDirection);

		vec = rotMatrix * glm::vec4(frontDirection, 1);
		frontDirection = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
	}
	break;
	case PerspectiveCamera::DOWN:
	{
		glm::vec4 vec;
		glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), -glm::radians(degrees), this->rightDirection);

		vec = rotMatrix * glm::vec4(frontDirection, 1);
		frontDirection = glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
	}
	break;
	}
	lookAtFront();
}

void PerspectiveCamera::move(DIRECTION direction, float distance)
{
	switch (direction)
	{
	case PerspectiveCamera::LEFT:
	{
		position = position - rightDirection * distance;
	}
	break;
	case PerspectiveCamera::RIGHT:
	{
		position = position + rightDirection * distance;
	}
	break;
	case PerspectiveCamera::UP:
	{
		position = position + upDirection * distance;
	}
	break;
	case PerspectiveCamera::DOWN:
	{
		position = position - upDirection * distance;
	}
	break;
	case PerspectiveCamera::AHEAD:
	{
		position = position + frontDirection * distance;
	}
	break;
	case PerspectiveCamera::BACKWARDS:
	{
		position = position - frontDirection * distance;
	}
	break;
	}
}

const std::vector<PerspectiveCamera::Plane>& PerspectiveCamera::getPlanes()
{
	return planes;
}

void PerspectiveCamera::calculateMatrices()
{
	calculateViewMatrix();
	calculateProjectionMatrix();
}

void PerspectiveCamera::calculateViewMatrix()
{
	view = glm::mat4(1.0f);
	view = glm::lookAt(position, lookAtPoint, glm::vec3(0, 0, 1));
	calculatePlanes2();
}

void PerspectiveCamera::calculateProjectionMatrix()
{
	projection = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
	calculatePlanes2();
}

void PerspectiveCamera::calculatePlanes()
{
	float tanOfHalfFov = tan(fov / 2);
	glm::vec4 vec;
	glm::mat4 rotMatrix;

	// Near plane
	planes[0].normal = frontDirection;
	planes[0].point = position + nearPlane * frontDirection;

	// Far plane
	planes[1].normal = -frontDirection;
	planes[1].point = position + farPlane * frontDirection;

	// Left plane
	rotMatrix = glm::rotate(glm::mat4(1.0f), tanOfHalfFov-glm::radians(90.0f), upDirection);
	vec = rotMatrix * glm::vec4(frontDirection, 1);

	planes[2].normal = glm::vec3(vec.x, vec.y, vec.z);
	planes[2].point = position;

	// Right plane
	rotMatrix = glm::rotate(glm::mat4(1.0f), -tanOfHalfFov + glm::radians(90.0f), upDirection);
	vec = rotMatrix * glm::vec4(frontDirection, 1);

	planes[3].normal = glm::vec3(vec.x, vec.y, vec.z);
	planes[3].point = position;

	// Top plane
	rotMatrix = glm::rotate(glm::mat4(1.0f), tanOfHalfFov - glm::radians(90.0f), rightDirection);
	vec = rotMatrix * glm::vec4(upDirection, 1);

	planes[4].normal = glm::vec3(vec.x, vec.y, vec.z);
	planes[4].point = position;

	// Bottom plane
	rotMatrix = glm::rotate(glm::mat4(1.0f), -tanOfHalfFov + glm::radians(90.0f), rightDirection);
	vec = rotMatrix * glm::vec4(upDirection, 1);

	planes[5].normal = glm::vec3(vec.x, vec.y, vec.z);
	planes[5].point = position;
}

void PerspectiveCamera::calculatePlanes2()
{
	glm::mat4 clipMatrix = projection * view;

	planes[0].normal = glm::vec3(clipMatrix[3] + clipMatrix[2]); // Near plane
	planes[1].normal = glm::vec3(clipMatrix[3] - clipMatrix[2]); // Far plane
	planes[2].normal = glm::vec3(clipMatrix[3] + clipMatrix[0]); // Left plane
	planes[3].normal = glm::vec3(clipMatrix[3] - clipMatrix[0]); // Right plane
	planes[4].normal = glm::vec3(clipMatrix[3] - clipMatrix[1]); // Top plane
	planes[5].normal = glm::vec3(clipMatrix[3] + clipMatrix[1]); // Bottom plane

	for (int i = 0; i < 6; ++i)
	{
		planes[i].normal = glm::normalize(planes[i].normal);
	}

	planes[0].point = position + nearPlane * frontDirection;
	planes[1].point = position + farPlane * frontDirection;
	planes[2].point = position;
	planes[3].point = position;
	planes[4].point = position;
	planes[5].point = position;
}

bool PerspectiveCamera::isPointInFrustum(float x, float y, float z)
{
	for (auto& plane : planes)
	{
		if (glm::dot(plane.normal, glm::vec3(x, y, z) - plane.point) < 0)
			return false;	
	}
	return true;
}

bool PerspectiveCamera::isAABBInFrustum(float x, float y, float z, float w_x, float w_y, float w_z)
{
	if (isPointInFrustum(x, y, z))
		return true;
	if (isPointInFrustum(x + w_x, y, z))
		return true;
	if (isPointInFrustum(x, y + w_y, z))
		return true;
	if (isPointInFrustum(x + w_x, y + w_y, z))
		return true;
	if (isPointInFrustum(x, y, z + w_z))
		return true;
	if (isPointInFrustum(x + w_x, y, z + w_z))
		return true;
	if (isPointInFrustum(x, y + w_y, z + w_z))
		return true;
	if (isPointInFrustum(x + w_x, y + w_y, z + w_z))
		return true;

	return false;
}

bool PerspectiveCamera::isPointInHalfspace(float x, float y, float z)
{
	if (glm::dot(frontDirection, glm::vec3(x, y, z) - position) < 0)
		return false;
	else
		return true;
}

bool PerspectiveCamera::isAABBInHalfspace(float x, float y, float z, float w_x, float w_y, float w_z)
{
	if (isPointInHalfspace(x, y, z))
		return true;
	if (isPointInHalfspace(x + w_x, y, z))
		return true;
	if (isPointInHalfspace(x, y + w_y, z))
		return true;
	if (isPointInHalfspace(x + w_x, y + w_y, z))
		return true;
	if (isPointInHalfspace(x, y, z + w_z))
		return true;
	if (isPointInHalfspace(x + w_x, y, z + w_z))
		return true;
	if (isPointInHalfspace(x, y + w_y, z + w_z))
		return true;
	if (isPointInHalfspace(x + w_x, y + w_y, z + w_z))
		return true;

	return false;
}
