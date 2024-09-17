#include "Renderer.h"
#include <iostream>
#include <exception>
namespace Renderer {
	void Renderer::run() {
		throw std::runtime_error("??");
	}
	void Renderer::Renderer::init()
	{
		m_Device.init();

	}
	void Renderer::cleanUp()
	{
		m_Device.cleanUp();
	}
	void Renderer::drawFrame()
	{
		uint32_t imageIndex;
		vkWaitForFences(m_Device.m_LogicalDevice, 1, &m_Device.m_InFlightFence, VK_TRUE, UINT64_MAX);
		vkAcquireNextImageKHR(m_Device.m_LogicalDevice, m_Device.m_SwapChain.m_NativeSwapChain, UINT64_MAX, m_Device.m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		vkResetFences(m_Device.m_LogicalDevice, 1, &m_Device.m_InFlightFence);

		vkResetCommandBuffer(m_Device.m_GraphicsPipeline.m_CommandBuffer, 0);
		m_Device.m_GraphicsPipeline.recordCommandBuffer(m_Device, m_Device.m_GraphicsPipeline.m_CommandBuffer, imageIndex);
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_Device.m_ImageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_Device.m_GraphicsPipeline.m_CommandBuffer;

		VkSemaphore signalSemaphores[] = { m_Device.m_RenderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit((*m_Device.m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, m_Device.m_InFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}


		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { m_Device.m_SwapChain.m_NativeSwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional
		vkQueuePresentKHR((*m_Device.m_PresentQueue.GetNativeQueue().get()), &presentInfo);
	}
}