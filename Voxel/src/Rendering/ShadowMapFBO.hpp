#pragma once

#include <gladES/glad.h>
#include <iostream>

class ShadowMapFBO
{
public:
	ShadowMapFBO(int width, int height);
	~ShadowMapFBO();
	void bindBuffer();
	void unbindBuffer();

	void bindTexture();
	void unbindTexture();
private:
	unsigned int shadowMap;
	unsigned int shadowMapFBO;

	int width, height;
};