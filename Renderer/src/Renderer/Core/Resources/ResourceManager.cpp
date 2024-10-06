#include <iostream>
#include "Application/Application.h"
#include "ResourceManager.h"

std::vector<Renderer::Vertex> Renderer::Vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
std::vector<uint16_t> Renderer::Indices = {
	0, 1, 2, 2, 3, 0
};

// Helper
uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(Application::Get()->getDevice().m_PhysicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

}

// General usage functions
Renderer::Buffer Renderer::ResourceManager::createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties)
{
	// Buffer creation
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	Buffer Buffer;
	if (vkCreateBuffer(Application::Get()->getNativeDevice(), &bufferInfo, nullptr, &Buffer.m_Buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}
	Buffer.m_Size = size;

	// Checking memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(Application::Get()->getNativeDevice(), Buffer.m_Buffer, &memRequirements);

	// Memory allocation
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	if (vkAllocateMemory(Application::Get()->getNativeDevice(), &allocInfo, nullptr, &Buffer.m_Memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	// Memory binding
	vkBindBufferMemory(Application::Get()->getNativeDevice(), Buffer.m_Buffer, Buffer.m_Memory, 0);

	return Buffer;
}

void Renderer::ResourceManager::DestroyBuffer(Buffer& buffer)
{
	vkDestroyBuffer(Application::Get()->getNativeDevice(), buffer.m_Buffer, nullptr);
	vkFreeMemory(Application::Get()->getNativeDevice(), buffer.m_Memory, nullptr);
}

void Renderer::ResourceManager::LoadModel(const char* path)
{
	//DestroyBuffer(m_VertexBuffer.m_Buffer);
	//m_VertexBuffer.m_Buffer = createBuffer(Vertices.size()*sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	Buffer stagingBuffer = createBuffer(Vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	copyDataToBuffer(stagingBuffer, (void*)Vertices.data());
	copyBuffer(stagingBuffer, m_VertexBuffer.m_Buffer, Vertices.size() * sizeof(Vertex));
	DestroyBuffer(stagingBuffer);


	Buffer stagingBufferIndex = createBuffer(Indices.size() * sizeof(Indices[0]), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	copyDataToBuffer(stagingBufferIndex, (void*)Indices.data());
	copyBuffer(stagingBufferIndex, m_IndexBuffer, Indices.size() * sizeof(Indices[0]));
	DestroyBuffer(stagingBufferIndex);

	m_IndicesCount = Indices.size();
	m_VertexBuffer.m_VerticesCount = Vertices.size();
	m_ModelLoaded = true;
}

void Renderer::ResourceManager::LoadTexture(const char* path)
{
	m_TextureManager.LoadTexture(path);
}

void Renderer::ResourceManager::copyDataToBuffer(Buffer& buffer, void* data)
{
	void* mappedData;
	vkMapMemory(Application::Get()->getNativeDevice(), buffer.m_Memory, 0, buffer.m_Size, 0, &mappedData);
	memcpy(mappedData, data, (size_t)buffer.m_Size);
	vkUnmapMemory(Application::Get()->getNativeDevice(), buffer.m_Memory);

}

void Renderer::ResourceManager::copyBuffer(Buffer& srcBuffer, Buffer& destBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = Application::Get()->getRenderer().BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer.m_Buffer, destBuffer.m_Buffer, 1, &copyRegion);

	Application::Get()->getRenderer().EndSingleTimeCommands(commandBuffer);
}


// Initialization and cleanup
void Renderer::ResourceManager::init()
{
	m_DescriptorManager.init(this);
	m_VertexBuffer.m_Buffer = createBuffer(Vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	m_IndexBuffer = createBuffer(Indices.size() * sizeof(Indices[0]), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	LoadModel("just test");
	m_TextureManager.init();

}

void Renderer::ResourceManager::cleanUp()
{
	m_TextureManager.cleanUp();
	m_DescriptorManager.cleanUp();
	DestroyBuffer(m_VertexBuffer.m_Buffer);
	DestroyBuffer(m_IndexBuffer);
}

