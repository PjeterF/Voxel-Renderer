#include "Mesh.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLES3/gl3.h>

#include "ShadowMapFBO.hpp"

Mesh::Mesh(std::vector<float> vertices, std::vector<int> indices, GLuint meshShader)
{
	this->meshShader = meshShader;
	vertexCount = vertices.size();
	indexCount = indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(int),
		&indices[0],
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)0);
	glEnableVertexAttribArray(0);									
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);									
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Chunk::Vertex> vertices, std::vector<Chunk::TriangleIndices> indices, GLuint meshShader, GLuint shadowShader)
{
	this->meshShader = meshShader;
	this->shadowShader = shadowShader;
	vertexCount = vertices.size();
	indexCount = indices.size()*3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Chunk::Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(Chunk::TriangleIndices),
		&indices[0],
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Chunk::Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Chunk::Vertex), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Chunk::Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::draw(PerspectiveCamera* camera, glm::vec3 lightPosition, glm::vec3 lightLookAtPoint, glm::vec2 chunkOffset)
{
	ShadowMapFBO shadowFramebuffer(2500, 2500);

	static float ortho[6] = { -200, 200, -200, 200, 50, 1200 };

	shadowFramebuffer.bindBuffer();
	glUseProgram(shadowShader);

	glm::mat4 lightProjection = glm::ortho(ortho[0], ortho[1], ortho[2], ortho[3], ortho[4], ortho[5]);
	glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAtPoint, glm::vec3(0, 0, 1));
	glm::mat4 lightTransform = lightProjection * lightView;

	unsigned int lightProjectionLocationShadowMap = glGetUniformLocation(shadowShader, "lightTransform");
	glUniformMatrix4fv(lightProjectionLocationShadowMap, 1, GL_FALSE, glm::value_ptr(lightTransform));


	// Draw Shadow
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shadowFramebuffer.unbindBuffer();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glViewport(0, 0, 1000, 1000);

	glUseProgram(meshShader);

	unsigned int lightProjectionLocation = glGetUniformLocation(meshShader, "lightTransform");
	glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(biasMatrix*lightTransform));

	unsigned int projectionLocation = glGetUniformLocation(meshShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

	unsigned int viewLocation = glGetUniformLocation(meshShader, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->getView()));

	unsigned int positionOffset = glGetUniformLocation(meshShader, "positionOffset");
	glUniform2f(positionOffset, chunkOffset.x, chunkOffset.y);

	// Draw Meshes
	glActiveTexture(GL_TEXTURE0);
	shadowFramebuffer.bindTexture();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	shadowFramebuffer.unbindTexture();
	glBindVertexArray(0);

	glUseProgram(0);
}
