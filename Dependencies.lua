-- Owl Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/Owl/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/Owl/vendor/glfw/include"
IncludeDir["Glad"] = "%{wks.location}/Owl/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Owl/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Owl/vendor/ImGuizmo"
IncludeDir["entt"] = "%{wks.location}/Owl/vendor/entt/include"
IncludeDir["glm"] = "%{wks.location}/Owl/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Owl/vendor/stb_image"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Owl/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"