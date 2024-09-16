#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include <vector>
namespace Renderer {
    class Device;

    //Helpers
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    


	class SwapChain
	{
	public:
        void create(const Device& device);
        void cleanUp(VkDevice device);
	private:
        VkSwapchainKHR m_NativeSwapChain;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;

        VkFormat m_ImageFormat;
        VkExtent2D m_Extent;
	};
}