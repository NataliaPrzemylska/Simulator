#include "Application/Application.h"
#include <iostream>
#include <exception>

int main() {
	try {
		(*Renderer::Application::Get()).run();
	}
	catch (std::runtime_error e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}