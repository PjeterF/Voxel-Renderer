#include "ChunkManager.hpp"
#include <cmath>
#include "../../Externals/FastNoiseLite.h"
#include "../Utility/Utility.hpp"

ChunkManager::ChunkManager(int viewDistance, GLuint shadowMapShaderID, GLuint meshShaderID)
{
    this->meshRenderer = new MeshRenderer(shadowMapShaderID, meshShaderID);
}

void ChunkManager::draw(PerspectiveCamera* camera)
{
    meshRenderer->meshes.clear();
    for (auto chunk : chunks)
    {
        glm::vec3 point = glm::vec3
        (
            (chunk.second->getCoordinates().x + 0.5f) * Chunk::chunkResolution,
            (chunk.second->getCoordinates().y + 0.5f) * Chunk::chunkResolution,
            0
        );

        if (camera->isAABBInHalfspace(point.x, point.y, point.z, Chunk::chunkResolution, Chunk::chunkResolution, Chunk::chunkHeight))
            meshRenderer->meshes.push_back(chunk.second->getMesh()); 
    }
    //std::cout << "Loaded chunks: " << chunks.size() << " | Chunks sent to GPU: " << meshRenderer->meshes.size() << "\n";
    meshRenderer->draw(camera, lightDirection);
}

bool ChunkManager::addChunk(Chunk* newChunk)
{
    int index = utility::pairing::integerPair(newChunk->getCoordinates().x, newChunk->getCoordinates().y);

    if (chunks.find(index) != chunks.end())
        return false;

    chunks.insert({ index, newChunk });
    
    return true;
}

void ChunkManager::generateChunk(int x, int y, int seed)
{
    if (chunks.find(utility::pairing::integerPair(x, y)) != chunks.end())
        return;

    Chunk* newChunk = new Chunk(x, y);

    generateTerrain(newChunk, 5, seed);
    placeTrees(newChunk, 10);

    addChunk(newChunk);
}

void ChunkManager::generateChunk2(int x, int y, int seed)
{
    if (chunks.find(utility::pairing::integerPair(x, y)) != chunks.end())
        return;

    Chunk* newChunk = new Chunk(x, y);

    generateTerrain(newChunk, 8, seed);

    addChunk(newChunk);
}

void ChunkManager::generateMeshesForAllChunks()
{
    meshRenderer->meshes.clear();
    for (auto chunk : chunks)
    {
        chunk.second->createMesh(meshRenderer->getMeshShaderID(), meshRenderer->getShadowMapShaderID(), this);
        meshRenderer->meshes.push_back(chunk.second->getMesh());
    }
}

bool ChunkManager::isVoxelAir(int chunkX, int chunkY, int localX, int localY, int localZ)
{
    auto it_chunk = chunks.find(utility::pairing::integerPair(chunkX, chunkY));

    if (it_chunk == chunks.end())
        return false;

    if ((*it_chunk).second->voxels[localX][localY][localZ].type == Voxel::AIR)
        return true;
    else
        return false;
}

Voxel* ChunkManager::getVoxel(PositionInfo& posInfo)
{
    if (posInfo.localVoxelCoord.x == -1 || posInfo.localVoxelCoord.y == -1 || posInfo.localVoxelCoord.z == -1)
        return nullptr;

    auto it = chunks.find(utility::pairing::integerPair(posInfo.chunkCoord.x, posInfo.chunkCoord.y));
    if (it == chunks.end())
        return nullptr;

    return &(*it).second->voxels[posInfo.localVoxelCoord.x][posInfo.localVoxelCoord.y][posInfo.localVoxelCoord.z];
}

Chunk* ChunkManager::getChunk(PositionInfo& posInfo)
{
    if (posInfo.localVoxelCoord.x == -1 || posInfo.localVoxelCoord.y == -1 || posInfo.localVoxelCoord.z == -1)
        return nullptr;

    auto it = chunks.find(utility::pairing::integerPair(posInfo.chunkCoord.x, posInfo.chunkCoord.y));
    if (it == chunks.end())
        return nullptr;

    return (*it).second;
}

ChunkManager::PositionInfo ChunkManager::castRayFromCamera(PerspectiveCamera* camera)
{
    glm::vec3 rayLocation = camera->getPosition();

    int maxIterations = 300;
    float step = 0.5f;
    for (int i = 0; i < maxIterations; i++)
    {
        rayLocation = rayLocation + step * camera->getDirection();
        PositionInfo posInfo = worldCoordinatesToLocal(rayLocation);

        auto currentChunk = chunks.find(utility::pairing::integerPair(posInfo.chunkCoord.x, posInfo.chunkCoord.y));
        if(currentChunk==chunks.end())
           return PositionInfo(-1, -1, -1, -1, -1);
        else if(posInfo.localVoxelCoord.z<0 || posInfo.localVoxelCoord.z >= Chunk::chunkHeight)
            return PositionInfo(-1, -1, -1, -1, -1);

        Voxel& currentVoxel = (*currentChunk).second->voxels[posInfo.localVoxelCoord.x][posInfo.localVoxelCoord.y][posInfo.localVoxelCoord.z];
        if (currentVoxel.type != Voxel::AIR)
            return PositionInfo(posInfo.localVoxelCoord.x, posInfo.localVoxelCoord.y, posInfo.localVoxelCoord.z, posInfo.chunkCoord.x, posInfo.chunkCoord.y);
    }
    return PositionInfo(-1, -1, -1, -1, -1);
}

Chunk* ChunkManager::getChunk(float x, int y)
{
    auto it = chunks.find(utility::pairing::integerPair(x, y));
    if (it == chunks.end())
        return nullptr;
    else
        return (*it).second;
}

bool ChunkManager::createMesh(int x, int y)
{
    Chunk* chunk = getChunk(x, y);
    if (chunk == nullptr)
    {
        return false;
    }
    else
    {
        chunk->createMesh(meshRenderer->getMeshShaderID(), meshRenderer->getShadowMapShaderID(), this);
        return true;
    }
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

int ChunkManager::numberOfVoxelNeighbors(PositionInfo posInfo)
{
    if (getVoxel(posInfo) == nullptr)
        return 0;

    int n = 0;
    Voxel* vox;
    PositionInfo pos;

    if (posInfo.localVoxelCoord.x == 0)
    {
        pos = posInfo;
        pos.chunkCoord.x--;
        pos.localVoxelCoord.x = Chunk::chunkResolution - 1;
        vox = getVoxel(pos);
        if (vox != nullptr)
        {
            if (vox->type != Voxel::AIR)
                n++;
        }
    }
    else
    {
        pos = posInfo;
        pos.localVoxelCoord.x--;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }
    if (posInfo.localVoxelCoord.x == Chunk::chunkResolution - 1)
    {
        pos = posInfo;
        pos.chunkCoord.x++;
        pos.localVoxelCoord.x = 0;
        vox = getVoxel(pos);
        if (vox != nullptr)
        {
            if (vox->type != Voxel::AIR)
                n++;
        }
    }
    else
    {
        pos = posInfo;
        pos.localVoxelCoord.x++;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }
    if (posInfo.localVoxelCoord.y == 0)
    {
        pos = posInfo;
        pos.chunkCoord.y--;
        pos.localVoxelCoord.y = Chunk::chunkResolution - 1;
        vox = getVoxel(pos);
        if (vox != nullptr)
        {
            if (vox->type != Voxel::AIR)
                n++;
        }
    }
    else
    {
        pos = posInfo;
        pos.localVoxelCoord.y--;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }
    if (posInfo.localVoxelCoord.y == Chunk::chunkResolution - 1)
    {
        pos = posInfo;
        pos.chunkCoord.y++;
        pos.localVoxelCoord.y = 0;
        vox = getVoxel(pos);
        if (vox != nullptr)
        {
            if (vox->type != Voxel::AIR)
                n++;
        }
    }
    else
    {
        pos = posInfo;
        pos.localVoxelCoord.y++;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }
    if (posInfo.localVoxelCoord.z != 0)
    {
        pos = posInfo;
        pos.localVoxelCoord.z++;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }
    if (posInfo.localVoxelCoord.z != Chunk::chunkHeight - 1)
    {
        pos = posInfo;
        pos.localVoxelCoord.z--;
        vox = getVoxel(pos);
        if (vox->type != Voxel::AIR)
            n++;
    }

    return n;
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
            for (int k = 0; k < Chunk::chunkHeight; k++)
            {
                if (k > noise.GetNoise((float)(i + chunk->getCoordinates().x * Chunk::chunkResolution), (float)(j + chunk->getCoordinates().y * Chunk::chunkResolution)) * Chunk::chunkHeight / dampening && k > 3)
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
        for (int z = 0; z < Chunk::chunkHeight; z++)
        {
            if (chunk->voxels[randCoord.x][randCoord.y][z].type == Voxel::AIR)
            {
                chunk->voxels[randCoord.x][randCoord.y][z].type = Voxel::SOLID;
                chunk->voxels[randCoord.x][randCoord.y][z].color = 0.8f * glm::vec4(0.4, 0.25, 0.12, 1);
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
            for (int z = prev_z; z < Chunk::chunkHeight; z++)
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

void ChunkManager::removeVoxel(Chunk* chunk, int x, int y, int z)
{
    glm::vec2 chunkCoord = chunk->getCoordinates();
    if (x == 0 && y == 0)
    {
        Chunk* c = this->getChunk(chunkCoord.x - 1, chunkCoord.y - 1);
        c->createMesh(this->meshRenderer->getMeshShaderID(), this->meshRenderer->getShadowMapShaderID(), this);
    }
    if (x == 0 && y == Chunk::chunkResolution-1)
    {

    }
    if (x == Chunk::chunkResolution - 1 && y == 0)
    {

    }
    if (x == Chunk::chunkResolution - 1 && y == Chunk::chunkResolution - 1)
    {

    }

    chunk->createMesh(this->meshRenderer->getMeshShaderID(), this->meshRenderer->getShadowMapShaderID(), this);
    chunk->voxels[x][y][z].type = Voxel::AIR;
}

ChunkManager::PositionInfo::PositionInfo(int x, int y, int z, int cx, int cy)
{
    localVoxelCoord = { x, y, z };
    chunkCoord = { cx, cy };
}

ChunkManager::PositionInfo::PositionInfo()
{
}
