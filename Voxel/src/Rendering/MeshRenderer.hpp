#pragma once

#include "Mesh.hpp"
#include "ShadowMapFBO.hpp"

class MeshRenderer
{
public:
	MeshRenderer(GLuint shadowMapShaderID, GLuint meshShaderID);
	void performShadowPass(glm::vec3 lightPosition, glm::vec3 lightLookAt);
	void draw(PerspectiveCamera* camera, glm::vec3 lightDirection);

	GLuint getShadowMapShaderID();
	GLuint getMeshShaderID();
	
	std::vector<Mesh*> meshes;
private:
	ShadowMapFBO* shadowMapFBO;
	glm::mat4 lightTransform=glm::mat4(1.0f);

	GLuint shadowMapShaderID;
	GLuint meshShaderID;

	float texelSize;
	int shadowMapResolution = 4000;
	float ortho[6] = { -250, 250, -250, 250, 1, 700 };
};