#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerspectiveCamera
{
public:
	PerspectiveCamera(float x, float y, float z, float fov, float nearPlane, float farPlane, float width, float height);
	glm::mat4 getProjection();
	glm::mat4 getView();
	void setPosition(float x, float y, float z);
	glm::vec3 getPosition();
	void lookAt(float x, float y, float z);
private:
	void calculateMatrices();
	void calculateViewMatrix();
	void calculateProjectionMatrix();

	glm::vec3 position;
	glm::vec3 lookAtPoint = glm::vec3(0, 0, 0);
	float fov;
	float nearPlane, farPlane;
	float width, height;

	glm::mat4 projection;
	glm::mat4 view;
};