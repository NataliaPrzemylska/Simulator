#include "Renderer.h"
#include <iostream>
#include <exception>
#include <Application/Application.h>
namespace Renderer {
// Setup
	void Renderer::init()
	{
		m_Device.init();
		m_SwapChain.create();
		createRenderPass();
		m_GraphicsPipeline.init();
		m_FrameManager.init();
		m_SwapChain.createFrameBuffers();

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

// Clenup
	void Renderer::cleanUp()
	{
		m_FrameManager.cleanUp();
		m_SwapChain.cleanUp();
		m_GraphicsPipeline.cleanUp();
		m_Device.cleanUp();
	}

// Basic functions
	void Renderer::drawFrame()
	{
		uint32_t imageIndex;
		vkWaitForFences(m_Device.m_LogicalDevice, 1, &m_FrameManager.m_InFlightFence, VK_TRUE, UINT64_MAX);
		vkAcquireNextImageKHR(m_Device.m_LogicalDevice, m_SwapChain.m_NativeSwapChain, UINT64_MAX, m_FrameManager.m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		vkResetFences(m_Device.m_LogicalDevice, 1, &m_FrameManager.m_InFlightFence);

		vkResetCommandBuffer(m_FrameManager.m_CommandBuffer, 0);
		m_FrameManager.recordCommandBuffer(m_FrameManager.m_CommandBuffer, imageIndex);
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_FrameManager.m_ImageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_FrameManager.m_CommandBuffer;

		VkSemaphore signalSemaphores[] = { m_FrameManager.m_RenderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit((*m_Device.m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, m_FrameManager.m_InFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}


		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain.m_NativeSwapChain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR((*m_Device.m_PresentQueue.GetNativeQueue().get()), &presentInfo);
	}

}