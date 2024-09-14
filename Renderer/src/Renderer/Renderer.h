#pragma once
#include "Core/Device/Device.h"
namespace Renderer {
	class Renderer {
	public:
		void run();
		void init();
		void cleanUp();
	private:
		Device m_Device;
	};
}