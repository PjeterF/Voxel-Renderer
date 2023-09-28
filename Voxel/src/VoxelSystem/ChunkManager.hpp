#pragma once

#include <map>

#include "Chunk.hpp"
#include "../Rendering/MeshRenderer.hpp"

class ChunkManager
{
public:
	struct PositionInfo
	{
		PositionInfo(int x, int y, int z, int cx, int cy);
		PositionInfo();
		glm::ivec3 localVoxelCoord;
		glm::ivec2 chunkCoord;
	};
	ChunkManager(int viewDistance, GLuint shadowMapShaderID, GLuint meshShaderID);
	void draw(PerspectiveCamera* camera);
	bool addChunk(Chunk* newChunk);
	void generateChunk(int x, int y, int seed);
	void generateMeshesForAllChunks();
	bool isVoxelAir(int chunkX, int chunkY, int localX, int localY, int localZ);

	PositionInfo raycastInstersection(PerspectiveCamera* camera);
	Chunk* getChunk(float worldCoordX, int worldCoordY);
	PositionInfo worldCoordinatesToLocal(glm::vec3 worldCoordinates);

	glm::vec3 lightDirection = glm::vec3(1, 1, -0.5);
private:

	int mapToNatural(int n);
	int cantorPair(int a, int b);

	void generateTerrain(Chunk* chunk, int dampening, int seed);
	void placeTrees(Chunk* chunk, int n);

	int viewDistance = 1;
	glm::vec2 middleChunkCoordinates = glm::vec2(0, 0);
	std::map<int, Chunk*> chunks;
	MeshRenderer* meshRenderer;
};