#pragma once
#include "Core/Device/Device.h"
#include "Renderer/Core/SwapChain/SwapChain.h"
#include "Renderer/Core/Resources/Frame/FrameManager.h"
#include "Renderer/Core/Resources/ResourceManager.h"
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
		void onRender();
		void onImGuiRender();

		VkDevice& GetNativeDevice() { return m_Device.getNativeDevice(); };
		Device& GetDevice() { return m_Device; };
		ResourceManager& GetResourceManager() { return m_ResourceManager; };
		FrameManager& GeFrameManager() { return m_FrameManager; }
	public:
		VkRenderPass m_RenderPass;
		bool m_FrameBufferResize = false;
	private:
		Device m_Device;
		SwapChain m_SwapChain;
		GraphicsPipeline m_GraphicsPipeline;
		FrameManager m_FrameManager;
		ResourceManager m_ResourceManager;
	private:
		void createRenderPass();
		void initImGui();
	};
}