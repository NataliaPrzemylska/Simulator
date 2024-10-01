#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Application/Application.h"
#include <iostream>
#include <exception>
#include "Bench.h"

int main() {
	try {
		(*Renderer::Application::Get()).run();
	}
	catch (std::runtime_error e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}