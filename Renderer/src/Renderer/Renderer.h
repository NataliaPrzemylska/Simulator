#pragma once
#include "Core/Device/Device.h"
#include "Renderer/Core/SwapChain/SwapChain.h"
#include "Renderer/Core/Drawing/FrameManager.h"
namespace Renderer {
	class SwapChain;
	class Renderer {
	friend class SwapChain;
	friend class Application;
	friend class GraphicsPipeline;
	friend class FrameManager;
	public:
		void init();
		void cleanUp();
		void drawFrame();
		VkDevice& GetNativeDevice() { return m_Device.getNativeDevice(); };
		Device& GetDevice() { return m_Device; };
		VkRenderPass m_RenderPass;

	private:
		Device m_Device;
		SwapChain m_SwapChain;
		GraphicsPipeline m_GraphicsPipeline;
		FrameManager m_FrameManager;

		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;
	private:
		void createRenderPass();
		void CreateSyncObjects();

	};
}