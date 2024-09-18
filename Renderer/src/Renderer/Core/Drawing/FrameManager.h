#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
namespace Renderer {
	class FrameManager
	{
	public:
		void init();
		void cleanUp();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void createCommandBuffer();
		VkCommandBuffer m_CommandBuffer;
	private:
		VkCommandPool m_CommandPool;

	};
}