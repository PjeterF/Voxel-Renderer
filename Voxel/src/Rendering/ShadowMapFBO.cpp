#include "ShadowMapFBO.hpp"

ShadowMapFBO::ShadowMapFBO(int width, int height)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

	glViewport(0, 0, width, height);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error in framebuffer Error code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMapFBO::~ShadowMapFBO()
{
	glDeleteFramebuffers(1, &shadowMapFBO);
	glDeleteTextures(1, &shadowMap);
}

void ShadowMapFBO::bindBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMapFBO);
}

void ShadowMapFBO::unbindBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void ShadowMapFBO::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

void ShadowMapFBO::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
