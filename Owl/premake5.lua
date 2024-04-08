project "Owl"
    kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "opch.h"
	pchsource "src/opch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
    
	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
    }

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"OWL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "OWL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OWL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OWL_DIST"
		runtime "Release"
		optimize "on"