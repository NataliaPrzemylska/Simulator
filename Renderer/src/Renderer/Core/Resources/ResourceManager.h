#pragma once
#include <vector>
#include "Vertex/Vertex.h"
#include "Descriptors/DescriptorsManager.h"
namespace Renderer {
	extern std::vector<Vertex> Vertices;
	extern std::vector<uint16_t> Indices;
	struct Buffer {
		size_t m_Size;
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};
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
	public:
		void init();
		void cleanUp();
	public:
		bool m_ModelLoaded = false;
		size_t m_IndicesCount = 0;
	private:
		VertexBuffer m_VertexBuffer;
		Buffer m_IndexBuffer;
	private:
		DescriptorsManager m_DescriptorManager;

	};
}