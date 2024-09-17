#include "Shader.h"
#include <fstream>
#include "Renderer/Core/Device/Device.h"
std::vector<char> readFile(const std::filesystem::path& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file! :(");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
void Renderer::Shader::LoadShader(const Device& device, std::filesystem::path path)
{
    auto code = readFile(path);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    if (vkCreateShaderModule(device.m_LogicalDevice, &createInfo, nullptr, &m_NativeShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module! :(");
    }
}

void Renderer::Shader::CleanUp(const Device& device)
{
    vkDestroyShaderModule(device.m_LogicalDevice, m_NativeShaderModule, nullptr);
}
