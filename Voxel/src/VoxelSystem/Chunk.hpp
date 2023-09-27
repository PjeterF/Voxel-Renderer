#pragma once

#include "Voxel.hpp"
#include "../Rendering/InstancedCubeRenderer.hpp"

#include <vector>;

class Mesh;
class ChunkManager;

class Chunk
{
public:
	struct Vertex
	{
		Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal);
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
	};
	struct TriangleIndices
	{
		TriangleIndices(glm::ivec3 index);
		glm::ivec3 index;
	};
	Chunk(int x, int y);
	void draw(PerspectiveCamera* camera, glm::vec3 lightDirection, glm::vec2 chunkOffset);
	void deactivateHiddenVoxels();
	void createMesh(GLuint meshShader, GLuint shadowMapShader, ChunkManager* manager);
	Mesh* getMesh();
	glm::vec2 getCoordinates();
	std::vector<Vertex> getMeshVertices();
	std::vector<TriangleIndices> getMeshIndices();

	static int chunkResolution;
	static int chunkHeight;
	static int voxelSize;
	Voxel*** voxels;
private:
	bool voxelIsCovered(int x, int y, int z);
	bool voxelIsAir(int x, int y, int z);

	std::vector<glm::ivec3> activeVoxels;
	glm::vec2 coord;
	std::vector<TriangleIndices> meshIndices;
	std::vector<Vertex> meshVertices;
	Mesh* mesh=nullptr;
};