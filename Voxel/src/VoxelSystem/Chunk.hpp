#pragma once

#include "Voxel.hpp"
#include "../Rendering/InstancedCubeRenderer.hpp"

#include <vector>;

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
	struct Index
	{
		Index(glm::ivec3 index);
		glm::ivec3 index;
	};
	Chunk(int x, int y);
	void draw(InstancedCubeRenderer* renderer);
	void deactivateHiddenVoxels();
	void createMesh();
	std::vector<Vertex> getMeshVertices();
	std::vector<float> getTestMesh();
	std::vector<Index> getMeshIndices();

	static int gridDimensions;
	static int voxelSize;
	Voxel*** voxels;
private:
	bool voxelIsCovered(int x, int y, int z);
	bool voxelIsInFrustrum(float x, float y, float z, PerspectiveCamera* camera);

	std::vector<glm::ivec3> activeVoxels;
	glm::vec2 coord;
	std::vector<Index> meshIndices;
	std::vector<Vertex> meshVertices;
	std::vector<float> testMesh;
};