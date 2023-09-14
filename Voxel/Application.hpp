#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cmath>

#include "src/VoxelSystem/Chunk.hpp"
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/Rendering/CubeRenderer.hpp"

class Application
{
public:
	Application(int windowWidth, int windowHeight, std::string title);
	void run();
private:
	GLFWwindow* window;
	int wndWidth;
	int wndHeight;
	std::string title;

	CubeRenderer* cubeRenderer;
	PerspectiveCamera* camera;
};