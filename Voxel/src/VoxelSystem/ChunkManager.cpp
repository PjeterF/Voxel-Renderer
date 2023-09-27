#include "ChunkManager.hpp"
#include <cmath>
#include "../../Externals/FastNoiseLite.h"

int ChunkManager::mapToNatural(int n)
{
    if (n >= 0)
        return 2 * n;
    else
        return -2 * n - 1;
}

ChunkManager::ChunkManager(int viewDistance, GLuint shadowMapShaderID, GLuint meshShaderID)
{
    this->meshRenderer = new MeshRenderer(shadowMapShaderID, meshShaderID);
}

void ChunkManager::draw(PerspectiveCamera* camera)
{
    meshRenderer->meshes.clear();
    for (auto chunk : chunks)
    {
        meshRenderer->meshes.push_back(chunk.second->getMesh());
    }
    meshRenderer->draw(camera, { -1, -1, -0.4 });
}

bool ChunkManager::addChunk(Chunk* newChunk)
{
    int index = cantorPair(newChunk->getCoordinates().x, newChunk->getCoordinates().y);

    if (chunks.find(index) != chunks.end())
        return false;

    chunks.insert({ index, newChunk });
    
    return true;
}

void ChunkManager::generateChunk(int x, int y, int seed)
{
    if (chunks.find(cantorPair(x, y)) != chunks.end())
        return;

    Chunk* newChunk = new Chunk(x, y);

    generateTerrain(newChunk, 2, seed);
    placeTrees(newChunk, 10);

    addChunk(newChunk);
}

void ChunkManager::generateMeshesForAllChunks()
{
    for (auto chunk : chunks)
    {
        chunk.second->createMesh(meshRenderer->getMeshShaderID(), meshRenderer->getShadowMapShaderID(), this);
        meshRenderer->meshes.push_back(chunk.second->getMesh());
    }
}

bool ChunkManager::isVoxelAir(int chunkX, int chunkY, int localX, int localY, int localZ)
{
    auto it_chunk = chunks.find(cantorPair(chunkX, chunkY));

    if (it_chunk == chunks.end())
        return false;

    if ((*it_chunk).second->voxels[localX][localY][localZ].type == Voxel::AIR)
        return true;
    else
        return false;
}

ChunkManager::PositionInfo ChunkManager::raycastInstersection(PerspectiveCamera* camera)
{
    glm::vec3 rayLocation = camera->getPosition();

    int maxIterations = 1000;
    float step = 0.5f;
    for (int i = 0; i < maxIterations; i++)
    {
        rayLocation = rayLocation + step * camera->getDirection();
        PositionInfo posInfo = worldCoordinatesToLocal(rayLocation);

        auto currentChunk = chunks.find(cantorPair(posInfo.chunkCoord.x, posInfo.chunkCoord.y));
        if(currentChunk==chunks.end())
           return PositionInfo(-1, -1, -1, -1, -1);
        else if(posInfo.localVoxelCoord.z<0 || posInfo.localVoxelCoord.z >= Chunk::chunkResolution)
            return PositionInfo(-1, -1, -1, -1, -1);

        Voxel& currentVoxel = (*currentChunk).second->voxels[posInfo.localVoxelCoord.x][posInfo.localVoxelCoord.y][posInfo.localVoxelCoord.z];
        if (currentVoxel.type != Voxel::AIR)
        {
            currentVoxel.color = glm::vec4(1, 0, 0, 1);
            currentVoxel.type = Voxel::AIR;
            (*currentChunk).second->createMesh(meshRenderer->getMeshShaderID(), meshRenderer->getShadowMapShaderID(), this);
            return PositionInfo(posInfo.localVoxelCoord.x, posInfo.localVoxelCoord.y, posInfo.localVoxelCoord.z, posInfo.chunkCoord.x, posInfo.chunkCoord.y);
        }
    }
    return PositionInfo(-1, -1, -1, -1, -1);
}

Chunk* ChunkManager::getChunk(float worldCoordX, int worldCoordY)
{
    int x = worldCoordX / Chunk::chunkResolution;
    int y = worldCoordY / Chunk::chunkResolution;

    auto it = chunks.find(cantorPair(x, y));
    if (it == chunks.end())
        return nullptr;
    else
        return (*it).second;
}

ChunkManager::PositionInfo ChunkManager::worldCoordinatesToLocal(glm::vec3 worldCoordinates)
{
    PositionInfo posInfo;

    if (worldCoordinates.x >= 0)
    {
        posInfo.chunkCoord.x = worldCoordinates.x / Chunk::chunkResolution;
        posInfo.localVoxelCoord.x = worldCoordinates.x - posInfo.chunkCoord.x * Chunk::chunkResolution;
    }
    else
    {
        posInfo.chunkCoord.x = worldCoordinates.x / Chunk::chunkResolution-1;
        posInfo.localVoxelCoord.x = Chunk::chunkResolution - (abs(worldCoordinates.x) - abs(posInfo.chunkCoord.x + 1) * Chunk::chunkResolution);
    }
    if (worldCoordinates.y >= 0)
    {
        posInfo.chunkCoord.y = worldCoordinates.y / Chunk::chunkResolution;
        posInfo.localVoxelCoord.y = worldCoordinates.y - posInfo.chunkCoord.y * Chunk::chunkResolution;
    }
    else
    {
        posInfo.chunkCoord.y = worldCoordinates.y / Chunk::chunkResolution - 1;
        posInfo.localVoxelCoord.y = Chunk::chunkResolution - (abs(worldCoordinates.y) - abs(posInfo.chunkCoord.y + 1) * Chunk::chunkResolution);
    }
    posInfo.localVoxelCoord.z = worldCoordinates.z;
    return posInfo;
}

int ChunkManager::cantorPair(int a, int b)
{
    a = mapToNatural(a);
    b = mapToNatural(b);

    return (a + b) * (a + b + 1) / 2 + b;
}

void ChunkManager::generateTerrain(Chunk* chunk, int dampening, int seed)
{
    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
;
    for (int i = 0; i < Chunk::chunkResolution; i++)
    {
        for (int j = 0; j < Chunk::chunkResolution; j++)
        {
            for (int k = 0; k < Chunk::chunkResolution; k++)
            {
                if (k > noise.GetNoise((float)(i + chunk->getCoordinates().x * Chunk::chunkResolution), (float)(j + chunk->getCoordinates().y * Chunk::chunkResolution)) * Chunk::chunkResolution / dampening && k > 3)
                {
                    chunk->voxels[i][j][k].type = Voxel::AIR;
                }
                else
                {
                    chunk->voxels[i][j][k].type = Voxel::SOLID;

                    int r = rand() % 256;
                    if (r > 250)
                        chunk->voxels[i][j][k].color = 0.5f * glm::vec4(0.54, 0.26, 0.07, 1);
                    else
                        chunk->voxels[i][j][k].color = glm::vec4(0, 0.2 * ((float)r / 512 + 0.5), 0, 1);
                }
            }
        }
    }
}

void ChunkManager::placeTrees(Chunk* chunk, int numberOfTrees)
{
    glm::ivec2 randCoord;

    for (int n = 0; n < numberOfTrees; n++)
    {
        randCoord = { rand() % Chunk::chunkResolution, rand() % Chunk::chunkResolution };

        int height = 3 + rand() % 3;
        int prev_z = 0;
        for (int z = 0; z < Chunk::chunkResolution; z++)
        {
            if (chunk->voxels[randCoord.x][randCoord.y][z].type == Voxel::AIR)
            {
                chunk->voxels[randCoord.x][randCoord.y][z].type = Voxel::SOLID;
                chunk->voxels[randCoord.x][randCoord.y][z].color = glm::vec4(0.4, 0.25, 0.12, 1);
                if (--height == 0)
                {
                    prev_z = z;
                    break;
                }
            }
        }
        height = 5;
        if (randCoord.x >= 1 && randCoord.x < Chunk::chunkResolution - 2 && randCoord.y >= 1 && randCoord.y < Chunk::chunkResolution - 2)
        {
            for (int z = prev_z; z < Chunk::chunkResolution; z++)
            {
                for (int x = randCoord.x - 1; x <= randCoord.x + 1; x++)
                {
                    for (int y = randCoord.y - 1; y <= randCoord.y + 1; y++)
                    {
                        chunk->voxels[x][y][z].type = Voxel::SOLID;
                        chunk->voxels[x][y][z].color = (float)(rand() % 100 + 50) / 150 * 0.5f * glm::vec4(0.3, 0.8, 0.3, 1);
                    }
                }
                if (--height == 0)
                {
                    break;
                }
            }
        }
    }
}

ChunkManager::PositionInfo::PositionInfo(int x, int y, int z, int cx, int cy)
{
    localVoxelCoord = { x, y, z };
    chunkCoord = { cx, cy };
}

ChunkManager::PositionInfo::PositionInfo()
{
}
