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
	Mesh(std::vector<float> vertices, std::vector<int> indices, GLuint shaderProgrammID);
	Mesh(std::vector<Chunk::Vertex> vertices, std::vector<Chunk::Index> indices, GLuint shaderProgrammID);
	void draw(PerspectiveCamera* camera);
private:
	int vertexCount;
	int indexCount;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint shaderProgramID;
};