#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../Rendering/PerspectiveCamera.hpp"

class DebugMenu
{
public:
	DebugMenu(PerspectiveCamera* camera);
	void render();

	PerspectiveCamera* camera;
private:
};