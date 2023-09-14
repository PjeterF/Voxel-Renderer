#include "CubeRenderer.hpp"

CubeRenderer::CubeRenderer(PerspectiveCamera* camera, GLuint shaderProgramID)
{
	this->camera = camera;
	this->shaderProgramID = shaderProgramID;

	std::vector<float> vertices=
	{
		-0.5 ,-0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,

		-0.5 ,-0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5
	};
	

	std::vector<int> indices=
	{
    0, 1, 2,
    2, 3, 0,

    1, 5, 6,
    6, 2, 1,

    7, 6, 5,
    5, 4, 7,

    4, 0, 3,
    3, 7, 4,

    3, 2, 6,
    6, 7, 3,

    4, 5, 1,
    1, 0, 4
	};

	

	VAO = new VertexArray;
	VAO->bind();

	VBO = new VertexBuffer(vertices);
	VBO->bind();

	EBO = new ElementBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	VAO->unbind();
}

void CubeRenderer::draw(float x, float y, float z, float size, float r, float g, float b, float a)
{
	glm::mat4 modelTransfrom = glm::mat4(1.0f);
	modelTransfrom = glm::translate(modelTransfrom, glm::vec3(x, y, z));
	modelTransfrom = glm::scale(modelTransfrom, glm::vec3(size, size, size));

	glUseProgram(shaderProgramID);

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelTransfrom));

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->getView()));

	unsigned int colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform4f(colorLocation, r, g, b, a);

	VAO->bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	VAO->unbind();
}

void CubeRenderer::commisionInstance(float x, float y, float z, float size, float r, float g, float b, float a)
{
	positions.push_back(glm::vec3(x, y, z));
	sizes.push_back(size);

}
