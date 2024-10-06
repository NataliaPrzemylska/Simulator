#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Application/Application.h"
#include <iostream>
#include <exception>
#include "Bench.h"
#include "Camera/Camera.h"

int main() {
	try {
		std::shared_ptr<Scene::Camera> sceneCamera = std::make_shared<Scene::Camera>();
		Renderer::Application::Get()->setCamera(sceneCamera.get());
		Renderer::Application::Get()->run();
	}
	catch (std::runtime_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}