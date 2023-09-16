#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class PerspectiveCamera
{
public:
	struct Plane
	{
		glm::vec3 normal = glm::vec3(0, 0, 0);
		glm::vec3 point = glm::vec3(0, 0, 0);
	};
	enum DIRECTION
	{
		LEFT, RIGHT, UP, DOWN, AHEAD, BACKWARDS
	};
	PerspectiveCamera(float x, float y, float z, float fov, float nearPlane, float farPlane, float width, float height);
	glm::mat4 getProjection();
	glm::mat4 getView();
	void setPosition(float x, float y, float z);
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	void setDirection(float x, float y, float z);
	void lookAt(float x, float y, float z);
	void lookAtFront();
	void turn(DIRECTION direction, float degrees);
	void move(DIRECTION direction, float distance);
	const std::vector<Plane>& getPlanes();
private:
	void calculateMatrices();
	void calculateViewMatrix();
	void calculateProjectionMatrix();
	void calculatePlanes();

	glm::vec3 position;
	glm::vec3 frontDirection = glm::vec3(1, 0, 0);
	glm::vec3 upDirection = glm::vec3(0, 0, 1);
	glm::vec3 rightDirection = glm::vec3(0, -1, 0);
	glm::vec3 lookAtPoint = glm::vec3(0, 0, 0);
	float fov;
	float nearPlane, farPlane;
	float width, height;

	glm::mat4 projection;
	glm::mat4 view;

	std::vector<Plane> planes;
};