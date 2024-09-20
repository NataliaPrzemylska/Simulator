#include "FrameManager.h"
#include <iostream>
#include "Renderer/Core/Device/Device.h"
#include "Application/Application.h"

void Renderer::FrameManager::init()
{
	createCommandBuffer();
	CreateSyncObjects();
}

void Renderer::FrameManager::cleanUp()
{
	vkDeviceWaitIdle(Application::Get()->getNativeDevice());
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(Application::Get()->getNativeDevice(), m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(Application::Get()->getNativeDevice(), m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(Application::Get()->getNativeDevice(), m_InFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(Application::Get()->getNativeDevice(), m_CommandPool, nullptr);
}

void Renderer::FrameManager::createCommandBuffer()
{
	Device& device = Application::Get()->getDevice();
	QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(device.m_PhysicalDevice, device.m_Surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	if (vkCreateCommandPool(device.m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool! :(");
	}
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 2;

	if (vkAllocateCommandBuffers(device.m_LogicalDevice, &allocInfo, m_CommandBuffers) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers! :(");
	}
}

void Renderer::FrameManager::drawFrame()
{
	uint32_t imageIndex;
	vkWaitForFences(Application::Get()->getNativeDevice(), 1, &m_InFlightFences[m_CurrentFrameInFlightIndex], VK_TRUE, UINT64_MAX);
	vkAcquireNextImageKHR(Application::Get()->getNativeDevice(), Application::Get()->m_Renderer.m_SwapChain.m_NativeSwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrameInFlightIndex], VK_NULL_HANDLE, &imageIndex);
	vkResetFences(Application::Get()->getNativeDevice(), 1, &m_InFlightFences[m_CurrentFrameInFlightIndex]);

	vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrameInFlightIndex], 0);
	recordCommandBuffer(m_CommandBuffers[m_CurrentFrameInFlightIndex], imageIndex);
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentFrameInFlightIndex];
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrameInFlightIndex];

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrameInFlightIndex];

	if (vkQueueSubmit((*Application::Get()->m_Renderer.GetDevice().m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, m_InFlightFences[m_CurrentFrameInFlightIndex]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}


	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrameInFlightIndex];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &Application::Get()->m_Renderer.m_SwapChain.m_NativeSwapChain;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR((*Application::Get()->m_Renderer.GetDevice().m_PresentQueue.GetNativeQueue().get()), &presentInfo);
	m_CurrentFrameInFlightIndex = (m_CurrentFrameInFlightIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::FrameManager::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = Application::Get()->getRenderer().m_RenderPass;
	renderPassInfo.framebuffer = (Application::Get()->m_Renderer.m_SwapChain).m_SwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = (Application::Get()->m_Renderer.m_SwapChain).m_Extent;
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Application::Get()->m_Renderer.m_GraphicsPipeline.getNativePipeline());
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>((Application::Get()->m_Renderer.m_SwapChain).m_Extent.width);
	viewport.height = static_cast<float>((Application::Get()->m_Renderer.m_SwapChain).m_Extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = (Application::Get()->m_Renderer.m_SwapChain).m_Extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(commandBuffer);
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer! :(");
	}
}


void Renderer::FrameManager::CreateSyncObjects()
{
	VkDevice& device = Application::Get()->getNativeDevice();
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores and/or fences! :(");
		}
	}

}
