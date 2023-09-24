#pragma once

#include "../OpenGL/VertexBuffer.hpp"
#include "../OpenGL/ElementBuffer.hpp"
#include "../OpenGL/VertexArray.hpp"
#include "../OpenGL/ShaderProgram.hpp"

#include"PerspectiveCamera.hpp"
#include "../VoxelSystem/Chunk.hpp"

#include <vector>

class Mesh
{
public:
	Mesh(std::vector<float> vertices, std::vector<int> indices, GLuint meshShader);
	Mesh(std::vector<Chunk::Vertex> vertices, std::vector<Chunk::Index> indices, GLuint meshShader, GLuint shadowShader);
	void draw(PerspectiveCamera* camera, glm::vec3 lightPosition, glm::vec3 lightLookAtPoint, glm::vec2 chunkOffset);
	//void draw(PerspectiveCamera* camera, glm::vec3 lightPosition, glm::vec3 lightLookAtPoint, glm::vec2 chunkOffset);
private:
	int vertexCount;
	int indexCount;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint meshShader;
	GLuint shadowShader;
};