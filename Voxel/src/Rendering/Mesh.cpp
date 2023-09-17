#include "Mesh.hpp"

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

Mesh::Mesh(std::vector<Chunk::Vertex> vertices, std::vector<Chunk::Index> indices, GLuint meshShader)
{
	this->meshShader = meshShader;
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
		indices.size() * sizeof(Chunk::Index),
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

void Mesh::draw(PerspectiveCamera* camera, glm::vec3 lightDirection)
{
	lightDirection = glm::normalize(lightDirection);
	glUseProgram(meshShader);

	unsigned int projectionLocation = glGetUniformLocation(meshShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

	unsigned int viewLocation = glGetUniformLocation(meshShader, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->getView()));
	unsigned int lightDirectionLocation = glGetUniformLocation(meshShader, "lightDirection");
	glUniform3f(lightDirectionLocation, lightDirection.x, lightDirection.y, lightDirection.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}
