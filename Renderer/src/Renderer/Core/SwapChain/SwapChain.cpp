#define NOMINMAX
#include "SwapChain.h"
#include "Renderer/Core/Device/Device.h"
#include "Application/Application.h"
#include <iostream>
#include <algorithm>
namespace Renderer {
//Helper
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
    {
        {

            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
    }


    void SwapChain::create(const Device& device)
    {
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = device.m_Surface;

        SwapChainSupportDetails supportDetails = querySwapChainSupport(device.m_PhysicalDevice, device.m_Surface);

        // Surface format
        for (const auto& availableFormat : supportDetails.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                createInfo.imageFormat = availableFormat.format;
                createInfo.imageColorSpace = availableFormat.colorSpace;
                break;
            }
        }
        if (createInfo.imageFormat == VK_FORMAT_UNDEFINED) {
            createInfo.imageFormat = supportDetails.formats[0].format;
            createInfo.imageColorSpace = supportDetails.formats[0].colorSpace;
        }

        // Presentation mode
        for (const auto& availablePresentMode : supportDetails.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                createInfo.presentMode = availablePresentMode;
                break;
            }
        }
        if (createInfo.presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

        }
        createInfo.clipped = VK_TRUE;

        // Swap extent
        if (supportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            createInfo.imageExtent = supportDetails.capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(Application::Get()->getGLFWwindow(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, supportDetails.capabilities.minImageExtent.width, supportDetails.capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, supportDetails.capabilities.minImageExtent.height, supportDetails.capabilities.maxImageExtent.height);

            createInfo.imageExtent = actualExtent;
        }

        // Image count
        createInfo.minImageCount = supportDetails.capabilities.minImageCount + 1;
        if (supportDetails.capabilities.maxImageCount > 0 && createInfo.minImageCount > supportDetails.capabilities.maxImageCount) {
            createInfo.minImageCount = supportDetails.capabilities.maxImageCount;
        }

        //Other
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // Queue families
        QueueFamilyIndices indices = Queue::GetQueueFamiliesIndices(device.m_PhysicalDevice, device.m_Surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        if (vkCreateSwapchainKHR(device.m_LogicalDevice, &createInfo, nullptr, &m_NativeSwapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain! :(");
        }

        // Getting swapchain images
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device.m_LogicalDevice, m_NativeSwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device.m_LogicalDevice, m_NativeSwapChain, &imageCount, m_SwapChainImages.data());
        
        // Creating swapchain imageViews
        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_SwapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_ImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if (vkCreateImageView(device.m_LogicalDevice, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views! :(");
            }
        }

    }

    void SwapChain::cleanUp(VkDevice device)
    {
        vkDestroySwapchainKHR(device, m_NativeSwapChain, nullptr);
        for (VkImageView imageView : m_SwapChainImageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }

}