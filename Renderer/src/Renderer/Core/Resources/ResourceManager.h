#pragma once
#include <vector>
#include "Vertex/Vertex.h"
#include "Descriptors/DescriptorsManager.h"
#include "Frame/FrameManager.h"
#include "Buffer.h"
namespace Renderer {
	extern std::vector<Vertex> Vertices;
	extern std::vector<uint16_t> Indices;
	
	struct VertexBuffer {
	public:
		size_t m_VerticesCount;
		Buffer m_Buffer;
	};
	class ResourceManager
	{
	public:
		// General usage
		Buffer createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties);
		void DestroyBuffer(Buffer& buffer);
		void LoadModel(const char* path);

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
		DescriptorsManager m_DescriptorManager;
		FrameManager m_FrameManager;
	private:
		VertexBuffer m_VertexBuffer;
		Buffer m_IndexBuffer;
	private:

	};
}