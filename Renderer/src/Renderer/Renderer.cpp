#include "Renderer.h"
#include <iostream>
#include <exception>
namespace Renderer {
	void Renderer::run() {
		throw std::runtime_error("??");
	}
	void Renderer::Renderer::init()
	{
		m_Device.init();

	}
	void Renderer::cleanUp()
	{
		m_Device.cleanUp();
	}
}