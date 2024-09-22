#include "ResourceManager.h"
#include <iostream>
#include "Application/Application.h"

void Renderer::VertexBuffer::createBuffer()
{
	m_Vertices = {
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
	m_Buffer = Application::Get()->getResourceManager().createVertexBuffer(sizeof(m_Vertices[0]) * m_Vertices.size());
	Application::Get()->getResourceManager().copyBuffer(m_Buffer, (void*)m_Vertices.data());
}
void Renderer::VertexBuffer::destroyBuffer()
{
	Application::Get()->getResourceManager().DestroyBuffer(m_Buffer);
}


// Helper
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(Renderer::Application::Get()->getDevice().m_PhysicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

}

// General usage functions
Renderer::Buffer Renderer::ResourceManager::createVertexBuffer(VkDeviceSize size)
{
	// Buffer creation
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	Buffer vertexBuffer;
	if (vkCreateBuffer(Application::Get()->getNativeDevice(), &bufferInfo, nullptr, &vertexBuffer.m_Buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}
	vertexBuffer.m_Size = size;

	// Checking memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(Application::Get()->getNativeDevice(), vertexBuffer.m_Buffer, &memRequirements);

	// Memory allocation
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (vkAllocateMemory(Application::Get()->getNativeDevice(), &allocInfo, nullptr, &vertexBuffer.m_Memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	// Memory binding
	vkBindBufferMemory(Application::Get()->getNativeDevice(), vertexBuffer.m_Buffer, vertexBuffer.m_Memory, 0);
	return vertexBuffer;
}


void Renderer::ResourceManager::DestroyBuffer(Buffer buffer)
{
	vkDestroyBuffer(Application::Get()->getNativeDevice(), buffer.m_Buffer, nullptr);
	vkFreeMemory(Application::Get()->getNativeDevice(), buffer.m_Memory, nullptr);
}
void Renderer::ResourceManager::copyBuffer(Buffer buffer, void* data)
{
	void* mappedData;
	vkMapMemory(Application::Get()->getNativeDevice(), buffer.m_Memory, 0, buffer.m_Size, 0, &mappedData);
	memcpy(mappedData, data, (size_t)buffer.m_Size);
	vkUnmapMemory(Application::Get()->getNativeDevice(), buffer.m_Memory);

}


// Initialization and cleanup
void Renderer::ResourceManager::init()
{
	m_VertexBuffer.createBuffer();
}

void Renderer::ResourceManager::cleanUp()
{
	m_VertexBuffer.destroyBuffer();
}

