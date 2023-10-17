#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(GLuint shadowMapShaderID, GLuint meshShaderID)
{
	this->meshShaderID = meshShaderID;
	this->shadowMapShaderID = shadowMapShaderID;
	shadowMapFBO = new ShadowMapFBO(2500, 2500);
}

void MeshRenderer::performShadowPass(glm::vec3 lightPosition, glm::vec3 lightLookAt)
{
	shadowMapFBO = new ShadowMapFBO(1000, 1000);

	shadowMapFBO->bindBuffer();
	glUseProgram(shadowMapShaderID);

	glm::mat4 lightProjection = glm::ortho(ortho[0], ortho[1], ortho[2], ortho[3], ortho[4], ortho[5]);
	glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAt, glm::vec3(0, 0, 1));
	lightTransform = lightProjection * lightView;

	unsigned int lightProjectionLocationShadowMap = glGetUniformLocation(shadowMapShaderID, "lightTransform");
	glUniformMatrix4fv(lightProjectionLocationShadowMap, 1, GL_FALSE, glm::value_ptr(lightTransform));


	// Draw Shadow
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	for (auto mesh : meshes)
	{
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
	}
	//glDisable(GL_CULL_FACE);
	
	glBindVertexArray(0);
	shadowMapFBO->unbindBuffer();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void MeshRenderer::draw(PerspectiveCamera* camera, glm::vec3 lightDirection)
{
	glm::vec3 lightPos =
	{
		camera->getPosition().x + (camera->getDirection().x - lightDirection.x) * camera->farPlane / 2,
		camera->getPosition().y + (camera->getDirection().y - lightDirection.y) * camera->farPlane / 2,
		70
	};

	performShadowPass(lightPos, lightPos+lightDirection);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glViewport(0, 0, 1000, 1000);

	glUseProgram(meshShaderID);

	unsigned int lightProjectionLocation = glGetUniformLocation(meshShaderID, "lightTransform");
	glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(biasMatrix * lightTransform));

	unsigned int projectionLocation = glGetUniformLocation(meshShaderID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

	unsigned int viewLocation = glGetUniformLocation(meshShaderID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->getView()));

	unsigned int positionOffset = glGetUniformLocation(meshShaderID, "positionOffset");
	glUniform2f(positionOffset, 0.0f, 0.0f);

	// Draw Meshes
	glActiveTexture(GL_TEXTURE0);
	shadowMapFBO->bindTexture();

	for (auto mesh : meshes)
	{
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	shadowMapFBO->unbindTexture();
	glBindVertexArray(0);

	glUseProgram(0);

	delete shadowMapFBO;
}

GLuint MeshRenderer::getShadowMapShaderID()
{
	return shadowMapShaderID;
}

GLuint MeshRenderer::getMeshShaderID()
{
	return meshShaderID;
}
