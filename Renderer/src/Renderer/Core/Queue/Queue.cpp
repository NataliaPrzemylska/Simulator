#include "Queue.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

namespace Renderer {
    QueueFamilyIndices Queue::m_QueueFamilyIndices;

    // Helper function
    std::string queueFlagsToString(VkQueueFlags queueFlags) {
        std::vector<std::string> flagNames;

        if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            flagNames.push_back("Graphics");
        }
        if (queueFlags & VK_QUEUE_COMPUTE_BIT) {
            flagNames.push_back("Compute");
        }
        if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
            flagNames.push_back("Transfer");
        }
        if (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            flagNames.push_back("Sparse Binding");
        }
        if (queueFlags & VK_QUEUE_PROTECTED_BIT) {
            flagNames.push_back("Protected");
        }
        if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) {
            flagNames.push_back("Video Decode");
        }
        if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) {
            flagNames.push_back("Video Encode");
        }
        if (queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) {
            flagNames.push_back("Optical Flow");
        }

        if (flagNames.empty()) {
            return "{}";
        }

        std::ostringstream oss;
        oss << "{ ";
        for (size_t i = 0; i < flagNames.size(); ++i) {
            oss << flagNames[i];
            if (i != flagNames.size() - 1) {
                oss << " | ";
            }
        }
        oss << " }";

        return oss.str();
    }

    void showQueueFamilies(VkPhysicalDevice device) {
#ifndef NDEBUG
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        std::cout << "The queue families:" << std::endl;
        for (const auto& queueFamily : queueFamilies) {
            std::cout << "queue number " << queueFamily.queueCount << std::endl;
            std::cout << "queue family properties " << queueFlagsToString(queueFamily.queueFlags) << std::endl;
        }
#endif // !NDEBUG
    }

    // Static functions
    QueueFamilyIndices Queue::findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                VkBool32 presentSupport = false;
                //vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);

                //if (presentSupport)
                 //   indices.presentFamily = i;

                indices.graphicsFamily = i;
            }
            if (indices.isComplete())
                break;

            i++;
        }
        if (!indices.isComplete())
        {
            throw std::runtime_error("Cannot find familes that meet the requirements :(");
        }
        return indices;
    }

    // Class functions
    QueueFamilyIndices Queue::GetQueueFamiliesIndices(VkPhysicalDevice device, const bool& shouldBeSaved)
    {
        if (!shouldBeSaved) return findQueueFamilies(device);
        if (!m_QueueFamilyIndices.isComplete()) m_QueueFamilyIndices = findQueueFamilies(device);
        return m_QueueFamilyIndices;
    }

    std::vector<float>* Queue::PopulateQueuesCreationInfo(std::vector<VkDeviceQueueCreateInfo>& InfoToPopulate)
    {
        if (!m_QueueFamilyIndices.isComplete())
        {
            throw std::runtime_error("Queue creation requested but queue families are still not found! :(");
        }


        std::set<uint32_t> uniqueQueueFamilies = { m_QueueFamilyIndices.graphicsFamily.value() };

        std::vector<float>* priorities = new std::vector<float>();
        (*priorities).resize(m_QueueFamilyIndices.getUniqueQueueFamilyIndices().size());
        int curPriority = 0;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = (uint32_t)uniqueQueueFamilies.size();
            (*priorities)[curPriority] = .1f;
            queueCreateInfo.pQueuePriorities = (*priorities).data();
            curPriority++;
            InfoToPopulate.push_back(queueCreateInfo);
        }
        return priorities;
    }

}