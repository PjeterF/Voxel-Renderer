#include "DebugMenu.hpp"

DebugMenu::DebugMenu(PerspectiveCamera* camera)
{
	this->camera = camera;
}

void DebugMenu::render()
{
	ImGui::Begin("DebugMenu");

	float camPos[3]= { camera->getPosition().x, camera->getPosition().y, camera->getPosition().z };
	if (ImGui::InputFloat3("Camera position", camPos))
	{
		camera->setPosition(camPos[0], camPos[1], camPos[2]);
	}
	float camDir[3] = { camera->getDirection().x, camera->getDirection().y, camera->getDirection().z };
	if (ImGui::InputFloat3("Camera direction", camDir))
	{
		camera->setDirection(camDir[0], camDir[1], camDir[2]);
	}
	ImGui::InputFloat("FOV", &camera->fov);
	ImGui::InputFloat("NearPlane", &camera->nearPlane);
	ImGui::InputFloat("FarPlane", &camera->farPlane);

	ImGui::End();
}
