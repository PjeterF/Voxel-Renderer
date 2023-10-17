#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Rendering/PerspectiveCamera.hpp"
#include "src/VoxelSystem/ChunkManager.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButttonCallback(GLFWwindow* window, int button, int action, int mods);

struct ObjectPool
{
	PerspectiveCamera* camera=nullptr;
	ChunkManager* manager=nullptr;
};

static bool cameraLocked = false;
bool setCamera = false;
glm::vec3 camDir(0, 0, 0);
static ObjectPool objectpool;

void setUpCallbacks(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButttonCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	PerspectiveCamera* camera = objectpool.camera;
	static float step = 1;
	static float degree = 5;

	switch (key)
	{
	case GLFW_KEY_W:
	{
		camera->move(PerspectiveCamera::AHEAD, step);
	}
	break;
	case GLFW_KEY_S:
	{
		camera->move(PerspectiveCamera::BACKWARDS, step);
	}
	break;
	case GLFW_KEY_A:
	{
		camera->move(PerspectiveCamera::LEFT, step);
	}
	break;
	case GLFW_KEY_D:
	{
		camera->move(PerspectiveCamera::RIGHT, step);
	}
	break;
	case GLFW_KEY_X:
	{
		camera->move(PerspectiveCamera::UP, step);
	}
	break;
	case GLFW_KEY_Z:
	{
		camera->move(PerspectiveCamera::DOWN, step);
	}
	break;
	case GLFW_KEY_I:
	{
		camera->turn(PerspectiveCamera::UP, degree);
	}
	break;
	case GLFW_KEY_K:
	{
		camera->turn(PerspectiveCamera::DOWN, degree);
	}
	break;
	case GLFW_KEY_J:
	{
		camera->turn(PerspectiveCamera::LEFT, degree);
	}
	break;
	case GLFW_KEY_L:
	{
		camera->turn(PerspectiveCamera::RIGHT, degree);
	}
	break;
	case GLFW_KEY_SPACE:
	{
		camera->move(PerspectiveCamera::UP, step);
	}
	break;
	case GLFW_KEY_Q:
	{
		camera->lookAt(0, 0, 0);
	}
	break;
	}

	if (mods == GLFW_MOD_CONTROL)
	{
		camera->move(PerspectiveCamera::DOWN, step);
	}

	if (mods == GLFW_MOD_ALT)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cameraLocked = true;
	}
	else
	{
		camDir = camera->getDirection();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cameraLocked = false;
	}
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	static int prev_x = 0;
	static int prev_y = 0;

	int wndWidth, wndHeight;
	glfwGetWindowSize(window, &wndWidth, &wndHeight);
	ypos = wndHeight - ypos;

	if (cameraLocked)
	{
		prev_x = xpos;
		prev_y = ypos;
		return;
	}

	static float sensitivity = 0.03;
	if (prev_x < xpos)
		objectpool.camera->turn(PerspectiveCamera::RIGHT, abs(prev_x - xpos) * sensitivity);
	else
		objectpool.camera->turn(PerspectiveCamera::LEFT, abs(prev_x - xpos) * sensitivity);

	if (prev_y < ypos)
		objectpool.camera->turn(PerspectiveCamera::UP, abs(prev_y - ypos) * sensitivity);
	else
		objectpool.camera->turn(PerspectiveCamera::DOWN, abs(prev_y - ypos) * sensitivity);

	prev_x = xpos;
	prev_y = ypos;
}

inline void mouseButttonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_1:
	{
		if(action == GLFW_PRESS)
			objectpool.manager->castRayFromCamera(objectpool.camera);
	}
	break;
	default:
		break;
	}
}
