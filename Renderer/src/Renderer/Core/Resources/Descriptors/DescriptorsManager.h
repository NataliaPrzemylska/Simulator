#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include "Renderer/Core/Resources/Buffer.h"
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

namespace Renderer {
	class ResourceManager;
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
	class DescriptorsManager
	{
		friend ResourceManager;
	public:
		void init(ResourceManager* ref);
		void cleanUp();
		void updateUniformBuffer(uint32_t cuffentFrame);
		VkDescriptorSetLayout& getDescriptorSetLayout() { return m_DescriptorSetLayout; };
		std::vector<Buffer> m_UniformBuffers;
		std::vector<void*> m_UniformBuffersMemoryMapped;
		VkDescriptorPool m_DescriptorPool;
	private:
		ResourceManager* m_ResourceManagerRef;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	private:
		glm::mat4 m_ModelMatrix;
	};
}