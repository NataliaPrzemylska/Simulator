#pragma once
#include "Core/Device/Device.h"
#include "Renderer/Core/SwapChain/SwapChain.h"
#include "Renderer/Core/Resources/ResourceManager.h"
#include "Scene/Scene.h"
namespace Renderer {
	class SwapChain;
	/*struct ImGuiComponents {
		VkAllocationCallbacks* g_Allocator = nullptr;
		VkInstance               g_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice                 g_Device = VK_NULL_HANDLE;
		uint32_t                 g_QueueFamily = (uint32_t)-1;
		VkQueue                  g_Queue = VK_NULL_HANDLE;
		VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

		ImGui_ImplVulkanH_Window g_MainWindowData;
		int                      g_MinImageCount = 2;
		bool                     g_SwapChainRebuild = false;

	};*/
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
		//ImGuiComponents m_ImGuiComponents;
		VkDescriptorPool m_ImGuiDescriptorPool;
	private:
		Device m_Device;
		SwapChain m_SwapChain;
		GraphicsPipeline m_GraphicsPipeline;
		ResourceManager m_ResourceManager;
		VkCommandPool m_PoolForOneTimeOperations;
	public:
		VkCommandBuffer BeginSingleTimeCommands(); // Graphics queue family
		void EndSingleTimeCommands(VkCommandBuffer& cb); // Submit to graphics queue
	private:
		void initCommandPool();
		void createRenderPass();
		void initImGui();
	};
}