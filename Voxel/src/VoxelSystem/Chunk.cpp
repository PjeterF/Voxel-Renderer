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
					hiddenVoxels.push_back(glm::ivec3(i, j, k));
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

void Chunk::createMesh()
{
	meshVertices.clear();
	meshIndices.clear();
	for (int i = 0; i < gridDimensions; i++)
	{
		for (int j = 0; j < gridDimensions; j++)
		{
			for (int k = 0; k < gridDimensions; k++)
			{
				if (voxels[i][j][k].type != Voxel::AIR)
				{
					//Check top
					if (k + 1 < gridDimensions)
					{
						if (voxels[i][j][k + 1].type == Voxel::AIR)
						{
							meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j+1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i+1, j+1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i+1, j, k + 1), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
					//Check bottom
					if (k - 1 > 0)
					{
						if (voxels[i][j][k - 1].type == Voxel::AIR)
						{
							meshVertices.push_back(Vertex(glm::vec3(i, j, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
					//Check right
					if (j + 1 < gridDimensions)
					{
						if (voxels[i][j+1][k].type == Voxel::AIR)
						{
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
					//Check left
					if (j - 1 > 0)
					{
						if (voxels[i][j - 1][k].type == Voxel::AIR)
						{

							meshVertices.push_back(Vertex(glm::vec3(i, j, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
					//Check front
					if (i + 1 < gridDimensions)
					{
						if (voxels[i+1][j][k].type == Voxel::AIR)
						{
							meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
					//Check back
					if (i - 1 > 0)
					{
						if (voxels[i - 1][j][k].type == Voxel::AIR)
						{
							meshVertices.push_back(Vertex(glm::vec3(i, j, k), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k + 1), voxels[i][j][k].color));
							meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k), voxels[i][j][k].color));

							glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
							glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

							meshIndices.push_back(index1);
							meshIndices.push_back(index2);
						}
					}
				}
			}
		}
	}
	testMesh.clear();
	for (auto vertex : meshVertices)
	{
		testMesh.push_back(vertex.position.x);
		testMesh.push_back(vertex.position.y);
		testMesh.push_back(vertex.position.z);

		testMesh.push_back(vertex.color.x);
		testMesh.push_back(vertex.color.y);
		testMesh.push_back(vertex.color.z);
		testMesh.push_back(vertex.color.w);
	}
}

std::vector<Chunk::Vertex> Chunk::getMeshVertices()
{
	return meshVertices;
}

std::vector<float> Chunk::getTestMesh()
{
	return testMesh;
}

std::vector<Chunk::Index> Chunk::getMeshIndices()
{
	return meshIndices;
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

Chunk::Vertex::Vertex(glm::vec3 position, glm::vec4 color)
{
	this->position = position;
	this->color = color;
}

Chunk::Index::Index(glm::ivec3 index)
{
	this->index = index;
}
