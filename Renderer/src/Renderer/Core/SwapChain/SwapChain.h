#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include <vector>
namespace Renderer {
    class Device;
    class Renderer;
    class GraphicsPipeline;
    class FrameManager;

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
        friend class GraphicsPipeline;
        friend class Renderer;
        friend class FrameManager;
	public:
        void create();
        void cleanUp();
        void recreate();
    private:
        void createFrameBuffers();
	private:
        VkSwapchainKHR m_NativeSwapChain;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;

        VkFormat m_ImageFormat;
        VkExtent2D m_Extent;
	};
}