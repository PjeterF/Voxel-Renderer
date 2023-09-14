#include "Application.hpp"


Application::Application(int windowWidth, int windowHeight, std::string title)
{
	wndWidth = windowWidth;
	wndHeight = windowHeight;
	this->title = title;

	glfwInit();
	window = glfwCreateWindow(wndWidth, wndHeight, this->title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, windowWidth, windowHeight);

	ShaderProgram* cubeShad = new ShaderProgram("src/Shaders/cube.vert", "src/Shaders/cube.frag");
	camera = new PerspectiveCamera(4, 4, 4, 45, 0.1, 100, windowWidth, windowWidth);
	cubeRenderer = new CubeRenderer(camera, cubeShad->getId());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeRenderer->draw(0, 0, 0, 1, 1, 0, 0, 1);
		cubeRenderer->draw(0, 0, 0, 0.8, 1, 1, 0, 1);
		cubeRenderer->draw(0, 0, 0, 0.6, 1, 0.5, 0.8, 1);
		cubeRenderer->draw(0, 0, 0, 0.4, 0.5, 0.5, 1, 1);
		cubeRenderer->draw(0, 0, 0, 0.2, 0.3, 0.3, 0.8, 1);

		camera->setPosition(4 * sin(glfwGetTime()), 4 * sin(glfwGetTime()), 4);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
