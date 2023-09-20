#pragma once

#include <gladES/glad.h>
#include <iostream>

class ShadowMapFBO
{
public:
	ShadowMapFBO(int width, int height);
	void bindForWriting();
	//void bindForReading();
	void unbind();
private:
	unsigned int shadowMap;
	unsigned int shadowFBO;

	int width, height;
};