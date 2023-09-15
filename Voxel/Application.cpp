#include "Application.hpp"
#include <random>


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

	ShaderProgram* cubeShad = new ShaderProgram("src/Shaders/cube.vert", "src/Shaders/cube.frag");
	camera = new PerspectiveCamera(0, 0, 0, 45, 0.1, 100, windowWidth, windowWidth);
	cubeRenderer = new CubeRenderer(camera, cubeShad->getId());

	camera->setPosition(20, 20, 20);

	ShaderProgram* instancedCudeShad = new ShaderProgram("src/Shaders/instancedCube.vert", "src/Shaders/instancedCube.frag");
	instancedCubeRenderer = new InstancedCubeRenderer(camera, instancedCudeShad->getId());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
}

void Application::run()
{
	Chunk c= Chunk();

	for (int i = 0; i < c.size; i++)
	{
		for (int j = 0; j < c.size; j++)
		{
			for (int k = 0; k < c.size; k++)
			{
				int r = rand() % 3;
				if (r==0)
				{
					c.voxels[i][j][k].type = Voxel::TYPE::RED;
				}
				else if (r==1)
				{
					c.voxels[i][j][k].type = Voxel::TYPE::GREEN;
				}
				else if (r==2)
				{
					c.voxels[i][j][k].type = Voxel::TYPE::BLUE;
				}

			}
		}
	}

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 10; k++)
				{
					instancedCubeRenderer->commisionInstance(i, j, k, 0.5, (float)i / 10, (float)j / 10, (float)k / 10, 1);
				}

			}
		}*/
		c.draw(instancedCubeRenderer);

		instancedCubeRenderer->drawInstances();

		double scale = sin(glfwGetTime());
		double scale2 = cos(glfwGetTime());
		camera->lookAt(5, 5, 5);
		camera->setPosition(10*scale2, 10 * scale, 20 * scale);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
