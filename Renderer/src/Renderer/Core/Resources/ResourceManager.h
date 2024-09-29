#pragma once
#include <vector>
#include "Vertex/Vertex.h"
namespace Renderer {
	extern std::vector<Vertex> Vertices;
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
		Buffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void DestroyBuffer(Buffer buffer);
		void LoadModel(const char* path);

		void copyDataToBuffer(Buffer buffer, void* data);
		std::vector<VkBuffer> getVertexBuffers() { return { m_VertexBuffer.m_Buffer.m_Buffer }; };
		size_t getVerticesCount() { return m_VertexBuffer.m_VerticesCount; }
	public:
		void init();
		void cleanUp();
	public:
		bool m_ModelLoaded = false;
	private:
		VertexBuffer m_VertexBuffer;
	};
}