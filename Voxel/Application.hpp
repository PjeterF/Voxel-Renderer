#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "src/GUI/DebugMenu.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cmath>
#include <random>
#include <chrono>

#include "Externals/FastNoiseLite.h"

#include "src/VoxelSystem/Chunk.hpp"
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/Rendering/CubeRenderer.hpp"
#include "src/Rendering/InstancedCubeRenderer.hpp"
#include "src/Rendering/Mesh.hpp"

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
	InstancedCubeRenderer* instancedCubeRenderer;
	PerspectiveCamera* camera;

	DebugMenu* debugMenu;
};