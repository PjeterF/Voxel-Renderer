#pragma once

#include "../OpenGL/VertexBuffer.hpp"
#include "../OpenGL/ElementBuffer.hpp"
#include "../OpenGL/VertexArray.hpp"
#include "../OpenGL/ShaderProgram.hpp"

#include "PerspectiveCamera.hpp"

#include <glm/gtc/type_ptr.hpp>

class CubeRenderer
{
public:
	CubeRenderer(PerspectiveCamera* camera, GLuint shaderProgramID);
	void draw(float x, float y, float z, float size, float r, float g, float b, float a);
	void commisionInstance(float x, float y, float z, float size, float r, float g, float b, float a);
private:
	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	PerspectiveCamera* camera;
	GLuint shaderProgramID;
};