#pragma once

#include <glm/vec4.hpp>

class Voxel
{
public:
	enum TYPE
	{
		AIR = 0,
		RED, GREEN, BLUE
	};
	Voxel(Voxel::TYPE type = AIR, bool active = true);
	~Voxel();

	Voxel::TYPE type;
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	bool active;
private:

};