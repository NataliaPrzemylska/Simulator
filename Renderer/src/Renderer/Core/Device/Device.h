#pragma once
#include "Renderer/Core/Queue/Queue.h"
#include <vector>
#include <memory>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include "Renderer/Core/Queue/Queue.h"

namespace Renderer{
#ifndef NDEBUG
	const bool enableValidationLayers = true;
#else 
	const bool enableValidationLayers = false;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

uint32_t RateDevice(VkPhysicalDevice device);

class Device
{
public:
	Device() : m_VulkanInstance(), m_Surface(), m_PhysicalDevice(), m_LogicalDevice() {};
	~Device() = default;
	void init(GLFWwindow* window);
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
private:
	const std::vector<const char*> m_ValidationLayers = { 
		"VK_LAYER_KHRONOS_validation"
	};
	VkInstance m_VulkanInstance;
	std::unique_ptr<VkDebugUtilsMessengerEXT> m_DebugMessenger = std::make_unique<VkDebugUtilsMessengerEXT>();
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_LogicalDevice;
	Queue m_GraphicsQueue;
	VkSurfaceKHR m_Surface;
private:
	VKAPI_ATTR static VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	};
};
}