#include "Chunk.hpp"
#include "Voxel.hpp"

int Chunk::gridDimensions = 16;
int Chunk::voxelSize = 1;

Chunk::Chunk(int x, int y)
{
	this->coord = glm::vec2(x, y);

	voxels = new Voxel**[gridDimensions];
	for (int i = 0; i < gridDimensions; i++)
	{
		voxels[i] = new Voxel*[gridDimensions];
		for (int j = 0; j < gridDimensions; j++)
		{
			voxels[i][j] = new Voxel[gridDimensions];
		}
	}
}

void Chunk::draw(InstancedCubeRenderer* renderer)
{
	int offsetX = gridDimensions * coord.x;
	int offsetY = gridDimensions * coord.y;

	for (int i = 0; i < gridDimensions; i++)
	{
		for (int j = 0; j < gridDimensions; j++)
		{
			for (int k = 0; k < gridDimensions; k++)
			{
				if (voxels[i][j][k].active)
				{
					/*if (voxelIsInFrustrum(i + offsetX, j + offsetY, k, renderer->getCamera()))
					{
						renderer->commisionInstance
						(
							i + offsetX, j + offsetY, k,
							voxelSize,
							voxels[i][j][k].color.x, voxels[i][j][k].color.y, voxels[i][j][k].color.z, voxels[i][j][k].color.w
						);
					}*/
					renderer->commisionInstance
					(
						i + offsetX, j + offsetY, k,
						voxelSize,
						voxels[i][j][k].color.x, voxels[i][j][k].color.y, voxels[i][j][k].color.z, voxels[i][j][k].color.w
					);
				}
			}
		}
	}
}

void Chunk::deactivateHiddenVoxels()
{
	std::vector<glm::ivec3> hiddenVoxels;
	for (int i = 0; i < gridDimensions; i++)
	{
		for (int j = 0; j < gridDimensions; j++)
		{
			for (int k = 0; k < gridDimensions; k++)
			{
				if (voxelIsCovered(i ,j, k))
				{
					hiddenVoxels.push_back(glm::vec3(i, j, k));
				}
			}
		}
	}
	for (auto coord : hiddenVoxels)
	{
		voxels[(int)coord.x][coord.y][coord.z].active = false;;
	}
}

bool Chunk::voxelIsCovered(int x, int y, int z)
{
	if (x == 0 || x == gridDimensions - 1)
		return false;
	if (y == 0 || y == gridDimensions - 1)
		return false;
	if (z == 0 || z == gridDimensions - 1)
		return false;

	int count = 0;
	if (voxels[x - 1][y][z].active)
		count++;
	if (voxels[x + 1][y][z].active)
		count++;
	if (voxels[x][y - 1][z].active)
		count++;
	if (voxels[x][y + 1][z].active)
		count++;
	if (voxels[x][y][z - 1].active)
		count++;
	if (voxels[x][y][z + 1].active)
		count++;

	if (count == 6)
		return true;
	else
		return false;
}

bool Chunk::voxelIsInFrustrum(float x, float y, float z, PerspectiveCamera* camera)
{
	for (auto plane : camera->getPlanes())
	{
		float distance = glm::dot(plane.normal, glm::vec3(x, y, z) - plane.point);
		if (distance < 0)
			return false;
	}
	return true;
}

