#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>
#include <memory>
#include <iostream>

namespace Renderer {
	void showQueueFamilies(VkPhysicalDevice physicalDevice);

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		//std::optional<uint32_t> presentFamily;
		bool isComplete() const {
			return graphicsFamily.has_value();
		}
		const std::vector<uint32_t> getUniqueQueueFamilyIndices() {
			if (!isComplete())
			{
				throw std::runtime_error("Queue families are not set! :(");
			}
			return *(new std::vector<uint32_t>({ graphicsFamily.value() }));
		}
	};

	class Queue
	{
	public:
		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		static QueueFamilyIndices GetQueueFamiliesIndices(VkPhysicalDevice device, const bool& shouldBeSaved = false);
		static std::vector<float>* PopulateQueuesCreationInfo(std::vector<VkDeviceQueueCreateInfo>& InfoToPopulate);

	private:
		static QueueFamilyIndices m_QueueFamilyIndices;
	public:
		Queue() = default;
		~Queue() = default;
		std::shared_ptr <VkQueue> GetNativeQueue() { return m_VulkanQueue; }
	private:
		std::shared_ptr<VkQueue> m_VulkanQueue = std::make_shared<VkQueue>();
	};
}