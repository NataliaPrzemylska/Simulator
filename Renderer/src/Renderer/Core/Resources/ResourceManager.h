#pragma once
#include <vector>
#include "Vertex/Vertex.h"
namespace Renderer {
	struct Buffer {
		size_t m_Size;
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};
	struct VertexBuffer {
	public:
		void createBuffer();
		void destroyBuffer();
		std::vector<Vertex> m_Vertices;
		Buffer m_Buffer;
	};
	class ResourceManager
	{
	public:
		// General usage
		Buffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void DestroyBuffer(Buffer buffer);
		void LoadModel(const char* path);

		void copyDataToBuffer(Buffer buffer, void* data);
		std::vector<VkBuffer> getVertexBuffers() { return { m_VertexBuffer.m_Buffer.m_Buffer }; };
		uint32_t getVerticesCount() { return m_VertexBuffer.m_Vertices.size(); }
	public:
		void init();
		void cleanUp();
	private:
		VertexBuffer m_VertexBuffer;
	};
}