#pragma once

#include <map>

#include "Chunk.hpp"

class ChunkManager
{
public:
	ChunkManager(int viewDistance);
	void draw(PerspectiveCamera* camera);
	bool addChunk(Chunk* newChunk);
private:
	int mapToNatural(int n);
	int cantorPair(int a, int b);

	int viewDistance = 1;
	glm::vec2 middleChunkCoordinates = glm::vec2(0, 0);
	std::map<int, Chunk*> chunks;
};