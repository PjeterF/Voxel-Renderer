#include "Application.hpp"
#include "inputCallbacks.hpp"

Application::Application(int windowWidth, int windowHeight, std::string title)
{
	srand(time(0));

	wndWidth = windowWidth;
	wndHeight = windowHeight;
	this->title = title;

	glfwInit();
	window = glfwCreateWindow(wndWidth, wndHeight, this->title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, windowWidth, windowHeight);

	setUpCallbacks(window);

	ShaderProgram* cubeShad = new ShaderProgram("src/Shaders/cube.vert", "src/Shaders/cube.frag");
	camera = new PerspectiveCamera(0, 0, 0, 45, 0.1, 100, windowWidth, windowWidth);
	glfwSetWindowUserPointer(window, camera);
	cubeRenderer = new CubeRenderer(camera, cubeShad->getId());

	camera->setPosition(20, 20, 20);

	ShaderProgram* instancedCudeShad = new ShaderProgram("src/Shaders/instancedCube.vert", "src/Shaders/instancedCube.frag");
	instancedCubeRenderer = new InstancedCubeRenderer(camera, instancedCudeShad->getId());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

void Application::run()
{
	FastNoiseLite noise;
	noise.SetSeed(200);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	Chunk::gridDimensions = 100;

	std::vector<float> noiseResult;
	for (int i = 0; i < Chunk::gridDimensions; i++)
	{
		for (int j = 0; j < Chunk::gridDimensions; j++)
		{
			noiseResult.push_back(noise.GetNoise((float)i, (float)j));
		}
	}

	Chunk c= Chunk(0, 0);
	for (int i = 0; i < c.gridDimensions; i++)
	{
		for (int j = 0; j < c.gridDimensions; j++)
		{
			for (int k = 0; k < c.gridDimensions; k++)
			{
				if (k > noiseResult[i + c.gridDimensions * j] * c.gridDimensions/5)
				{
					c.voxels[i][j][k].active = false;;
				}
				int r = rand() % 256;
				c.voxels[i][j][k].color = glm::vec4(0, (float)r / 512+0.5, 0, 1);

				r = rand() % 256;
				if (r > 200)
					c.voxels[i][j][k].color = glm::vec4(0.54, 0.26, 0.07, 1);

				if (c.voxels[i][j][k].active == false && k < 5 && k !=0)
				{
					c.voxels[i][j][k].active = true;
					c.voxels[i][j][k].color = glm::vec4(0, 0.4, (float)r / 256+0.5, 0.7);
				}

			}
		}
	}
	c.deactivateHiddenVoxels();
	Chunk c2 = Chunk(0, 1);
	for (int i = 0; i < c2.gridDimensions; i++)
	{
		for (int j = 0; j < c2.gridDimensions; j++)
		{
			for (int k = 0; k < c2.gridDimensions; k++)
			{
				if (k > noiseResult[i + c2.gridDimensions * j] * c2.gridDimensions)
				{
					c2.voxels[i][j][k].active = false;;
				}
				int r = rand() % 256;
				c2.voxels[i][j][k].color = glm::vec4(1, (float)r / 300, (float)r / 600, 1);
			}
		}
	}
	c2.deactivateHiddenVoxels();

	while (!glfwWindowShouldClose(window))
	{
		camera->lookAtFront();

		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		c.draw(instancedCubeRenderer);
		c2.draw(instancedCubeRenderer);

		instancedCubeRenderer->drawInstances();

		//std::cout << (int)camera->getPosition().x << ", " << (int)camera->getPosition().y << ", " << (int)camera->getPosition().z << std::endl;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
