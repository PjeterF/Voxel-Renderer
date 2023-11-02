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
	objectpool.camera = camera;
	cubeRenderer = new CubeRenderer(camera, cubeShad->getId());

	camera->setPosition(20, 20, 20);

	ShaderProgram* instancedCudeShad = new ShaderProgram("src/Shaders/instancedCube.vert", "src/Shaders/instancedCube.frag");
	instancedCubeRenderer = new InstancedCubeRenderer(camera, instancedCudeShad->getId());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	Chunk::chunkResolution = 32;
	Chunk::chunkHeight = Chunk::chunkResolution*4;

	ShaderProgram* meshShad = new ShaderProgram("src/Shaders/mesh.vert", "src/Shaders/mesh.frag");
	ShaderProgram* shadowShad = new ShaderProgram("src/Shaders/shadowMap.vert", "src/Shaders/shadowMap.frag");

	chunkManager = new ChunkManager(1, shadowShad->getId(), meshShad->getId());
	objectpool.manager = chunkManager;

	for (int x = -10; x < 10; x++)
	{
		for (int y = -10; y < 10; y++)
		{
			chunkManager->generateChunk(x, y, 102);
		}
	}
	chunkManager->generateMeshesForAllChunks();

	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		camera->lookAtFront();
		
		glClearColor(0.52, 0.8, 0.91, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float T = 10;
		float sine = sin(glfwGetTime() / T);
		float cosine = cos(glfwGetTime() / T);

		chunkManager->draw(camera);

		ImGui::Begin("LightDirection");
		ImGui::InputFloat("x", &chunkManager->lightDirection.x);
		ImGui::InputFloat("y", &chunkManager->lightDirection.y);
		ImGui::InputFloat("z", &chunkManager->lightDirection.z);
		ImGui::End();

		static int i = 0;
		if (i++% 100 == 0)
		{
			//ChunkManager::PositionInfo posInfo; posInfo = chunkManager->raycastInstersection(camera);
			//std::cout << posInfo.localVoxelCoord.x << " " << posInfo.localVoxelCoord.y << " " << posInfo.localVoxelCoord.z << " | " << posInfo.chunkCoord.x << " " << posInfo.chunkCoord.y << "\n";
		}

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
