@echo off
cd ../

cmake -P VulkanSDK.cmake > Shaders/output.txt
cd Shaders
for /f "tokens=*" %%i in ('findstr "VULKAN_SDK_LOCATION is set to:" output.txt') do (
    set "line=%%i"
)

for /f "tokens=6" %%a in ("%line%") do (
    set "VULKAN_SDK_LOCATION=%%a"
)
echo %VULKAN_SDK_LOCATION%
cd Fragment
%VULKAN_SDK_LOCATION%\Bin\glslc.exe Triangle.frag -o frag.spv
cd ../Vertex
%VULKAN_SDK_LOCATION%\Bin\glslc.exe Triangle.vert -o vert.spv
cd ../
del output.txt

pause