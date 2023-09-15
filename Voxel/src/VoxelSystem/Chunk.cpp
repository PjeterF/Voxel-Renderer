#include "Chunk.hpp"
#include "Voxel.hpp"

int Chunk::size = 16;

Chunk::Chunk()
{
	voxels = new Voxel**[size];
	for (int i = 0; i < size; i++)
	{
		voxels[i] = new Voxel*[size];
		for (int j = 0; j < size; j++)
		{
			voxels[i][j] = new Voxel[size];
		}
	}
}

void Chunk::draw(InstancedCubeRenderer* renderer)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				switch (voxels[i][j][k].type)
				{
				case Voxel::TYPE::RED:
				{
					renderer->commisionInstance(i, j, k, 1, 1, 0, 0, 1);
				}
				break;
				case Voxel::TYPE::GREEN:
				{
					renderer->commisionInstance(i, j, k, 1, 0, 1, 0, 1);
				}
				break;
				case Voxel::TYPE::BLUE:
				{
					renderer->commisionInstance(i, j, k, 1, 1, 0, 1, 1);
				}
				break;
				default:
					break;
				}
			}
		}
	}
}
