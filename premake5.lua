include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Owl"
    architecture "x86_64"
    startproject "OwlEditor"

    configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
    
    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Owl/vendor/Box2D"
	include "Owl/vendor/GLFW"
	include "Owl/vendor/Glad"
	include "Owl/vendor/ImGui"
	include "Owl/vendor/yaml-cpp"
group ""

group "Core"
	include "Owl"
	include "OwlEditor"
group ""

group "Misc"
	include "Sandbox"
group ""