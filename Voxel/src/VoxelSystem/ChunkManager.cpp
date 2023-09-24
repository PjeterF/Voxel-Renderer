#include "ChunkManager.hpp"
#include <cmath>

int ChunkManager::mapToNatural(int n)
{
    if (n > 0)
        return 2 * n;
    else
        return -2 * n - 1;
}

ChunkManager::ChunkManager(int viewDistance)
{
    
}

void ChunkManager::draw(PerspectiveCamera* camera)
{
    for (auto chunk : chunks)
    {
       // chunk.second->draw(camera, glm::vec3(1, 1, -0.3));
    }
}

bool ChunkManager::addChunk(Chunk* newChunk)
{
    int index = cantorPair(newChunk->getCoordinates().x, newChunk->getCoordinates().y);

    if (chunks.find(index) != chunks.end())
        return false;

    chunks.insert({ index, newChunk });
    return true;
}

int ChunkManager::cantorPair(int a, int b)
{
    a = mapToNatural(a);
    b = mapToNatural(b);

    return (a + b) * (a + b + 1) / 2 + b;
}
