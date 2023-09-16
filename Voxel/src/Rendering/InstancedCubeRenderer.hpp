#pragma once

#include "../OpenGL/VertexBuffer.hpp"
#include "../OpenGL/ElementBuffer.hpp"
#include "../OpenGL/VertexArray.hpp"
#include "../OpenGL/ShaderProgram.hpp"

#include "PerspectiveCamera.hpp"

#include <glm/gtc/type_ptr.hpp>

class InstancedCubeRenderer
{
public:
	InstancedCubeRenderer(PerspectiveCamera* camera, GLuint shaderProgramID);
	void commisionInstance(float x, float y, float z, float size, float r, float g, float b, float a);
	void drawInstances();
	PerspectiveCamera* getCamera();
private:
	class INSTANCE_DATA
	{
	public:
		INSTANCE_DATA();
		void setBufferSize(int size);
		int getBufferSize();
		void addInstance(float x, float y, float z, float size, float r, float g, float b, float a);
		void reset();
		int size();
		bool empty();

		std::vector<glm::vec3> positions;
		std::vector<float> sizes;
		std::vector<glm::vec4> colors;
	private:
		int currentIndex = 0;
		int bufferSize=1000000;
	};

	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	PerspectiveCamera* camera;
	GLuint shaderProgramID;

	INSTANCE_DATA instanceData;
};