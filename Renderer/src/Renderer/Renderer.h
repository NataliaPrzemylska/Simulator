#pragma once
#include "Core/Device/Device.h"
namespace Renderer {
	class Renderer {
	public:
		void run();
		void init();
		void cleanUp();
		void drawFrame();
	private:
		Device m_Device;

	};
}