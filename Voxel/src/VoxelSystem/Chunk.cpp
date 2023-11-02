#include "Chunk.hpp"
#include "Voxel.hpp"

#include "../Rendering/Mesh.hpp"
#include "ChunkManager.hpp"
#include "../Utility/Utility.hpp"

int Chunk::chunkResolution = 32;
int Chunk::chunkHeight = 32*4;
int Chunk::voxelSize = 1;

Chunk::Chunk(int x, int y)
{
	this->coord = glm::vec2(x, y);

	voxels = new Voxel**[chunkResolution];
	for (int i = 0; i < chunkResolution; i++)
	{
		voxels[i] = new Voxel*[chunkResolution];
		for (int j = 0; j < chunkResolution; j++)
		{
			voxels[i][j] = new Voxel[chunkHeight];
		}
	}
}

void Chunk::draw(PerspectiveCamera* camera, glm::vec3 lightDirection, glm::vec2 chunkOffset)
{
	if (mesh == nullptr)
		return;
	
	lightDirection = glm::normalize(lightDirection);

	float halfResolution= chunkResolution / 2;
	glm::vec3 lightPosition = glm::vec3(halfResolution, halfResolution, halfResolution) - 1.5f * halfResolution * glm::vec3(lightDirection);
	glm::vec3 lightLookAt = lightPosition + lightDirection;

	mesh->draw(camera, lightPosition, lightLookAt, chunkOffset);
}

void Chunk::deactivateHiddenVoxels()
{
	std::vector<glm::ivec3> hiddenVoxels;
	for (int i = 0; i < chunkResolution; i++)
	{
		for (int j = 0; j < chunkResolution; j++)
		{
			for (int k = 0; k < chunkResolution; k++)
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
	if (x == 0 || x == chunkResolution - 1)
		return false;
	if (y == 0 || y == chunkResolution - 1)
		return false;
	if (z == 0 || z == chunkResolution - 1)
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

void Chunk::createMesh(GLuint meshShader, GLuint shadowMapShader, ChunkManager* manager)
{
	meshVertices.clear();
	meshIndices.clear();

	glm::vec3 offset(coord.x * Chunk::chunkResolution, coord.y * Chunk::chunkResolution, 0);

	for (int i = 0; i < chunkResolution; i++)
	{
		for (int j = 0; j < chunkResolution; j++)
		{
			for (int k = 0; k < chunkHeight; k++)
			{
				bool faces[6] = { false, false, false, false, false, false }; // top botton right left front back
				if (!isVoxelAir(i, j, k))
				{
					//Check top
					if (k + 1 < chunkHeight)
					{
						if (isVoxelAir(i, j, k + 1))
							faces[0] = true;
					}

					//Check bottom
					if (k - 1 >= 0)
					{
						if (isVoxelAir(i, j, k-1))
							faces[1] = true;
					}

					//Check right
					if (j + 1 < chunkResolution)
					{
						if (isVoxelAir(i, j+1, k))
							faces[2] = true;
					}
					else if (manager->isVoxelAir(coord.x, coord.y + 1, i, 0, k))
						faces[2] = true;

					//Check left
					if (j - 1 >= 0)
					{
						if (isVoxelAir(i, j-1, k))
							faces[3] = true;
					}
					else if (manager->isVoxelAir(coord.x, coord.y - 1, i, chunkResolution - 1, k))
						faces[3] = true;

					//Check front
					if (i + 1 < chunkResolution)
					{
						if (isVoxelAir(i+1, j, k))
							faces[4] = true;
					}
					else if (manager->isVoxelAir(coord.x + 1, coord.y, 0, j, k))
						faces[4] = true;

					//Check back
					if (i - 1 >= 0)
					{
						if (isVoxelAir(i-1, j, k))
							faces[5] = true;
					}
					else if (manager->isVoxelAir(coord.x - 1, coord.y, chunkResolution - 1, j, k))
						faces[5] = true;

					if (faces[0])
					{
						meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 0, 1)));
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 0, 1)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 0, 1)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 0, 1)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
					if (faces[1])
					{
						meshVertices.push_back(Vertex(glm::vec3(i, j, k) + offset, voxels[i][j][k].color, glm::vec3(0, 0, -1)));
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(0, 0, -1)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(0, 0, -1)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k) + offset, voxels[i][j][k].color, glm::vec3(0, 0, -1)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
					if (faces[2])
					{
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(0, 1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, 1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(0, 1, 0)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
					if (faces[3])
					{
						meshVertices.push_back(Vertex(glm::vec3(i, j, k) + offset, voxels[i][j][k].color, glm::vec3(0, -1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, -1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(0, -1, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k) + offset, voxels[i][j][k].color, glm::vec3(0, -1, 0)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
					if (faces[4])
					{
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k) + offset, voxels[i][j][k].color, glm::vec3(1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i + 1, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(1, 0, 0)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
					if (faces[5])
					{
						meshVertices.push_back(Vertex(glm::vec3(i, j, k) + offset, voxels[i][j][k].color, glm::vec3(-1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i, j, k + 1) + offset, voxels[i][j][k].color, glm::vec3(-1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k + 1) + offset, voxels[i][j][k].color, glm::vec3(-1, 0, 0)));
						meshVertices.push_back(Vertex(glm::vec3(i, j + 1, k) + offset, voxels[i][j][k].color, glm::vec3(-1, 0, 0)));

						glm::ivec3 index1 = glm::ivec3(0, 1, 2) + (int)(2 * meshIndices.size());
						glm::ivec3 index2 = glm::ivec3(0, 2, 3) + (int)(2 * meshIndices.size());

						meshIndices.push_back(index1);
						meshIndices.push_back(index2);
					}
				}
			}
		}
	}

	delete mesh;
	mesh = new Mesh(meshVertices, meshIndices, meshShader, shadowMapShader);
}

Mesh* Chunk::getMesh()
{
	return mesh;
}

glm::vec2 Chunk::getCoordinates()
{
	return coord;
}

std::vector<Chunk::Vertex> Chunk::getMeshVertices()
{
	return meshVertices;
}

std::vector<Chunk::TriangleIndices> Chunk::getMeshIndices()
{
	return meshIndices;
}

bool Chunk::isVoxelAir(int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < chunkResolution && y < chunkResolution && z < chunkHeight)
	{
		if (voxels[x][y][z].type == Voxel::AIR)
			return true;
	}
	return false;
}

std::vector<glm::ivec3> Chunk::voxelVertices(int i, int j, int k)
{
	std::vector<glm::ivec3> vertices;

	vertices.push_back({ i, j, k });
	vertices.push_back({ i, j+1, k });
	vertices.push_back({ i+1, j+1, k });
	vertices.push_back({ i+1, j, k });
	vertices.push_back({ i, j, k+1 });
	vertices.push_back({ i, j + 1, k+1 });
	vertices.push_back({ i + 1, j + 1, k+1 });
	vertices.push_back({ i + 1, j, k+1 });

	return vertices;
}

bool Chunk::isVertexInMap(int i, int j, int k, std::unordered_map<int, int>& map)
{
	auto it = map.find(utility::pairing::cantorTriple(i, j, k));
	if (it == map.end())
		return true;
	else
		return false;
}

Chunk::Vertex::Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal)
{
	this->position = position;
	this->color = color;
	this->normal = normal;
}

Chunk::TriangleIndices::TriangleIndices(glm::ivec3 index)
{
	this->index = index;
}
