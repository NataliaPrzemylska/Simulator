#pragma once

#include "Renderer/Core/Queue/Queue.h"
#include "Renderer/Core/SwapChain/SwapChain.h"
#include "Renderer/Core/GraphicsPipeline/GraphicsPipeline.h"
#include <vector>
#include <memory>
#include <iostream>
#include <optional>

#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

namespace Renderer{
#ifndef NDEBUG
	const bool enableValidationLayers = true;
#else 
	const bool enableValidationLayers = false;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

uint32_t RateDevice(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

class Device
{
	friend class SwapChain;
	friend class Shader;
	friend class GraphicsPipeline;
	friend class Renderer;
public:
	Device() : m_VulkanInstance(), m_Surface(), m_PhysicalDevice(), m_LogicalDevice() {};
	~Device() = default;
	void init();
	void cleanUp();
	VkInstance* getVulkanInstance() { return &m_VulkanInstance; }
private:
	void createVulkanInstance();
	void setupDebugMessanger();
	bool checkValidationLayerSupport() const;
	std::vector<const char*> getRequiredExtensions() const;
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface(GLFWwindow* nativeWindow);
	void CreateSyncObjects();
private:
	const std::vector<const char*> m_ValidationLayers = { 
		"VK_LAYER_KHRONOS_validation"
	};
	std::unique_ptr<VkDebugUtilsMessengerEXT> m_DebugMessenger = std::make_unique<VkDebugUtilsMessengerEXT>();
	VkInstance m_VulkanInstance;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_LogicalDevice;
	VkSurfaceKHR m_Surface;
	VkSemaphore m_ImageAvailableSemaphore;
	VkSemaphore m_RenderFinishedSemaphore;
	VkFence m_InFlightFence;

	Queue m_GraphicsQueue;
	Queue m_PresentQueue;
	SwapChain m_SwapChain;
	GraphicsPipeline m_GraphicsPipeline;
private:
	VKAPI_ATTR static VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	};

};
}