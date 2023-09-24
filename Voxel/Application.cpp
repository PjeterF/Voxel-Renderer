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
	camera = new PerspectiveCamera(0, 0, 0, 45, 0.5, 500, windowWidth, windowWidth);
	glfwSetWindowUserPointer(window, camera);
	cubeRenderer = new CubeRenderer(camera, cubeShad->getId());

	camera->setPosition(20, 20, 20);

	ShaderProgram* instancedCudeShad = new ShaderProgram("src/Shaders/instancedCube.vert", "src/Shaders/instancedCube.frag");
	instancedCubeRenderer = new InstancedCubeRenderer(camera, instancedCudeShad->getId());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 300 es");

	debugMenu = new DebugMenu(camera);
}

void Application::run()
{
	FastNoiseLite noise;
	//noise.SetSeed(102);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	Chunk::chunkResolution = 16*16;

	std::vector<float> noiseResult;
	for (int i = 0; i < Chunk::chunkResolution; i++)
	{
		for (int j = 0; j < Chunk::chunkResolution; j++)
		{
			noiseResult.push_back(noise.GetNoise((float)i, (float)j));
		}
	}

	ShaderProgram* meshShad = new ShaderProgram("src/Shaders/mesh.vert", "src/Shaders/mesh.frag");
	ShaderProgram* shadowShad = new ShaderProgram("src/Shaders/shadowMap.vert", "src/Shaders/shadowMap.frag");

	Chunk c = Chunk(0, 0);
	for (int i = 0; i < c.chunkResolution; i++)
	{
		for (int j = 0; j < c.chunkResolution; j++)
		{
			for (int k = 0; k < c.chunkResolution; k++)
			{
				if (k > noise.GetNoise((float)i, (float)j) * c.chunkResolution/20 && k!=0)
				{
					c.voxels[i][j][k].type = Voxel::AIR;
				}
				else
				{
					c.voxels[i][j][k].type = Voxel::SOLID;

					int r = rand() % 256;
					if (r > 250)
						c.voxels[i][j][k].color = 0.5f * glm::vec4(0.54, 0.26, 0.07, 1);
					else
						c.voxels[i][j][k].color = glm::vec4(0, 0.2 * ((float)r / 512 + 0.5), 0, 1);
				}
			}
		}
	}
	for (int n = 0; n < 100; n++)
	{
		int rx = rand() % Chunk::chunkResolution;
		int ry = rand() % Chunk::chunkResolution;

		int height = 5;
		for (int z = 0; z < Chunk::chunkResolution; z++)
		{
			if (c.voxels[rx][ry][z].type == Voxel::AIR)
			{
				c.voxels[rx][ry][z].type = Voxel::SOLID;
				c.voxels[rx][ry][z].color = glm::vec4(0.3, 0.3, 0.3, 1);
				if (--height == 0)
					break;
			}
		}
	}
	c.createMesh(meshShad->getId(), shadowShad->getId());

	Chunk c2(0, 1);
	for (int i = 0; i < c2.chunkResolution; i++)
	{
		for (int j = 0; j < c2.chunkResolution; j++)
		{
			for (int k = 0; k < c2.chunkResolution; k++)
			{
				if (k > noise.GetNoise((float)i, (float)j + c2.chunkResolution) * c2.chunkResolution / 20 && k != 0)
				{
					c2.voxels[i][j][k].type = Voxel::AIR;
				}
				else
				{
					c2.voxels[i][j][k].type = Voxel::SOLID;

					int r = rand() % 256;
					if (r > 250)
						c2.voxels[i][j][k].color = 0.5f * glm::vec4(0.54, 0.26, 0.07, 1);
					else
						c2.voxels[i][j][k].color = glm::vec4(0, 0.2 * ((float)r / 512 + 0.5), 0, 1);
				}
			}
		}
	}
	c2.createMesh(meshShad->getId(), shadowShad->getId());

	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		camera->lookAtFront();
		
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float T = 10;
		float sine = sin(glfwGetTime() / T);
		float cosine = cos(glfwGetTime() / T);

		static int i = 0;
		c.draw(camera, glm::vec3(sine, cosine, -0.2), glm::vec2(c.getCoordinates().x * c.chunkResolution, c.getCoordinates().y * c.chunkResolution));
		c2.draw(camera, glm::vec3(sine, cosine, -0.2), glm::vec2(c2.getCoordinates().x * c2.chunkResolution, c2.getCoordinates().y * c2.chunkResolution));

		debugMenu->render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

		auto timeEnd = std::chrono::high_resolution_clock::now();

		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();

		glfwSetWindowTitle(window, std::to_string(frameDuration).c_str());
	}
}
