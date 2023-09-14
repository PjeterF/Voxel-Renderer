#pragma once

#include "Voxel.hpp"

#include <vector>;

class Chunk
{
public:
	Chunk(int x, int y, int z);
private:
	Voxel*** voxels;
	int x, y, z;
};