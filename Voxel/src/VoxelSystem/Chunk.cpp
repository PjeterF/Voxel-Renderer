#include "Chunk.hpp"

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	voxels = new Voxel**[z];
	for (int i = 0; i < z; i++)
	{
		voxels[i] = new Voxel*[y];
		for (int j = 0; j < y; j++)
		{
			voxels[i][j] = new Voxel[x];
		}
	}
}
