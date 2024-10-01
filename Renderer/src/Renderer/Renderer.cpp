#include "Renderer.h"
#include <iostream>
#include <exception>
#include <Application/Application.h>
#include "Bench.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
namespace Renderer {
// Setup
	void Renderer::init()
	{
		m_Device.init();
		initCommandPool();
		m_SwapChain.create();
		createRenderPass();
		m_SwapChain.createFrameBuffers();
		m_ResourceManager.init();
		m_GraphicsPipeline.init();
		m_ResourceManager.m_FrameManager.init(&m_ResourceManager);
		//initImGui();
	}

	void Renderer::initCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = Application::Get()->getDevice().m_GraphicsQueue.m_QueueFamilyIndices.graphicsFamily.value();
		if (vkCreateCommandPool(Application::Get()->getNativeDevice(), &poolInfo, nullptr, &m_PoolForOneTimeOperations) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool! :(");
		}
	}

	void Renderer::createRenderPass()
	{

		// Render pass utilties
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChain.m_ImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		// Render pass
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_Device.m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	// ImGui Helper
	static void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

	void Renderer::initImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(Application::Get()->getGLFWwindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_Device.m_VulkanInstance;
		init_info.PhysicalDevice = m_Device.m_PhysicalDevice;
		init_info.Device = m_Device.m_LogicalDevice;	
		init_info.QueueFamily = m_Device.m_GraphicsQueue.m_QueueFamilyIndices.graphicsFamily.value();
		init_info.Queue = *m_Device.m_GraphicsQueue.m_VulkanQueue.get();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = VK_NULL_HANDLE;
		init_info.RenderPass = m_RenderPass;
		init_info.Subpass = 0;
		init_info.MinImageCount = 2;
		init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = nullptr;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info);

	}

// Clenup
	void Renderer::cleanUp()
	{
		vkDestroyCommandPool(Application::Get()->getNativeDevice(), m_PoolForOneTimeOperations, nullptr);
		m_ResourceManager.m_FrameManager.cleanUp();
		m_ResourceManager.cleanUp();
		m_GraphicsPipeline.cleanUp();
		m_SwapChain.cleanUp();
		m_Device.cleanUp();
	}

// Basic functions
	void Renderer::onRender()
	{
		m_ResourceManager.m_FrameManager.drawFrame();
	}

	void Renderer::onImGuiRender()
	{
	}


	// Command buffers
	VkCommandBuffer Renderer::BeginOneTimeOperationBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_PoolForOneTimeOperations;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(Application::Get()->getNativeDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void Renderer::EndAndSubmitOneTimeOperationBuffer(VkCommandBuffer& cb)
	{
		vkEndCommandBuffer(cb);
		Application::Get()->getDevice().m_GraphicsQueue.submitToQueue(cb);
		vkFreeCommandBuffers(Application::Get()->getNativeDevice(), m_PoolForOneTimeOperations, 1, &cb);
	}
}