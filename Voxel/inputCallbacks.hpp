#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Rendering/PerspectiveCamera.hpp"

static bool cameraLocked = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

void setUpCallbacks(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	PerspectiveCamera* camera = (PerspectiveCamera *)glfwGetWindowUserPointer(window);
	static float step = 0.5;
	static float degree = 5;

	if (mods == GLFW_MOD_ALT)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cameraLocked = true;
	}	
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cameraLocked = false;
	}

	

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
	}
	if (mods == GLFW_MOD_CONTROL)
	{
		camera->move(PerspectiveCamera::DOWN, step);
	}
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (cameraLocked)
		return;

	static bool first=true;
	static int prev_x = -100;
	static int prev_y = -100;

	int wndWidth, wndHeight;
	glfwGetWindowSize(window, &wndWidth, &wndHeight);
	ypos = wndHeight - ypos;

	PerspectiveCamera* camera = (PerspectiveCamera*)glfwGetWindowUserPointer(window);

	if (first)
	{
		prev_x = xpos;
		prev_y = ypos;
		first = false;
		return;
	}

	static float sensitivity = 0.03;

	if (prev_x < xpos)
		camera->turn(PerspectiveCamera::RIGHT, abs(prev_x - xpos) * sensitivity);
	else
		camera->turn(PerspectiveCamera::LEFT, abs(prev_x - xpos) * sensitivity);

	if (prev_y < ypos)
		camera->turn(PerspectiveCamera::UP, abs(prev_y - ypos) * sensitivity);
	else
		camera->turn(PerspectiveCamera::DOWN, abs(prev_y - ypos) * sensitivity);

	prev_x = xpos;
	prev_y = ypos;
}