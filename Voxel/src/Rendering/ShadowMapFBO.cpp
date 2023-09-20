#include "ShadowMapFBO.hpp"

ShadowMapFBO::ShadowMapFBO(int width, int height)
{
	this->width = width;
	this->height = height;

	//FBO creation
	glGenFramebuffers(1, &shadowFBO);

	//Texture creation
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Attatching texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffers(1, GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR in framebuffer\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, width, height);
}

void ShadowMapFBO::unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
