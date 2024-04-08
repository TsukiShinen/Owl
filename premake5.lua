include "Dependencies.lua"

workspace "Owl"
    architecture "x86_64"
    startproject "Sandbox"

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
	include "Owl/vendor/GLFW"
	include "Owl/vendor/Glad"
group ""

group "Core"
	include "Owl"
group ""

group "Misc"
	include "Sandbox"
group ""