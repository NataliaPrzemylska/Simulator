#pragma once
#include <vector>
#include "Vertex/Vertex.h"
#include "Descriptors/DescriptorsManager.h"
#include "Frame/FrameManager.h"
#include "Images/TextureManager.h"
#include "Buffer.h"
#include "Scene/Scene.h"
namespace Renderer {
	extern std::vector<Vertex> Vertices;
	extern std::vector<uint16_t> Indices;
	
	struct VertexBuffer {
	public:
		size_t m_VerticesCount;
		Buffer m_Buffer;
	};
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	class ResourceManager
	{
	public:
		// General usage
		Buffer createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties);
		void DestroyBuffer(Buffer& buffer);
		void LoadModel(const char* path);
		void LoadTexture(const char* path, ImageUsage usage);

		void copyDataToBuffer(Buffer& buffer, void* data);
		void copyBuffer(Buffer& srcBuffer, Buffer& destBuffer, VkDeviceSize size);
		std::vector<VkBuffer> getVertexBuffers() { return { m_VertexBuffer.m_Buffer.m_Buffer }; };
		VkBuffer getIndexBuffer() { return m_IndexBuffer.m_Buffer; };
		size_t getVerticesCount() { return m_VertexBuffer.m_VerticesCount; }
		size_t getIndicesCount() { return  m_IndicesCount; }
		VkDescriptorSetLayout& getDescriptorSetLayout() { return m_DescriptorManager.getDescriptorSetLayout(); }
		std::vector<VkDescriptorSet>& getDescriptorSets() { return m_DescriptorManager.m_DescriptorSets; }
	public:
		void init();
		void cleanUp();
	public:
		bool m_ModelLoaded = false;
		size_t m_IndicesCount = 0;

		// Managers
		DescriptorsManager m_DescriptorManager;
		FrameManager m_FrameManager;
		TextureManager m_TextureManager;
	private:
		VertexBuffer m_VertexBuffer;
		Buffer m_IndexBuffer;
	private:

	};
}