#pragma once

#include "Voxel.hpp"
#include "../Rendering/InstancedCubeRenderer.hpp"

#include <vector>;

class Chunk
{
public:
	struct Vox
	{
		Voxel* voxel;
		glm::vec3 indices;
	};
	Chunk(int x, int y);
	void draw(InstancedCubeRenderer* renderer);
	void deactivateHiddenVoxels();

	static int gridDimensions;
	static int voxelSize;
	Voxel*** voxels;
private:
	bool voxelIsCovered(int x, int y, int z);
	bool voxelIsInFrustrum(float x, float y, float z, PerspectiveCamera* camera);
	std::vector<Vox> activeVoxels;
	glm::vec2 coord;
};