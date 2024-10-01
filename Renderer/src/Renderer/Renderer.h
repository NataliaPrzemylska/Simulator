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
		FrameManager& GeFrameManager() { return m_ResourceManager.m_FrameManager; }
		VkExtent2D& GetSwapChainExtent() { return m_SwapChain.m_Extent; }
	public:
		bool m_FrameBufferResize = false;
		VkRenderPass m_RenderPass;
	private:
		Device m_Device;
		SwapChain m_SwapChain;
		GraphicsPipeline m_GraphicsPipeline;
		ResourceManager m_ResourceManager;
		VkCommandPool m_PoolForOneTimeOperations;
	public:
		VkCommandBuffer BeginOneTimeOperationBuffer(); // Graphics queue family
		void EndAndSubmitOneTimeOperationBuffer(VkCommandBuffer& cb); // Submit to graphics queue
	private:
		void initCommandPool();
		void createRenderPass();
		void initImGui();
	};
}