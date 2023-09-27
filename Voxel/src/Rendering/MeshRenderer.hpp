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
	float ortho[6] = { -500, 500, -500, 500, 50, 1200 };

	GLuint shadowMapShaderID;
	GLuint meshShaderID;
};