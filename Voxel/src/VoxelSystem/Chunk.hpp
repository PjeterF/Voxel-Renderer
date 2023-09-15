#pragma once

#include "Voxel.hpp"
#include "../Rendering/InstancedCubeRenderer.hpp"

#include <vector>;

class Chunk
{
public:
	Chunk();
	void draw(InstancedCubeRenderer* renderer);
	static int size;
	Voxel*** voxels;
private:

};