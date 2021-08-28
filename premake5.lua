workspace "MinecraftClone"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project  "MinecraftClone"
	location "MinecraftClone"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. output .. "/%{prj.name}")
	objdir("bin-int/" .. output .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Dependencies/glfw/include",
		"Dependencies/glew/include",
		"Dependencies/libnoise/include"
	}

	libdirs 
	{
		"Dependencies/glfw/lib",
		"Dependencies/glew/lib",
		"Dependencies/libnoise/lib"
	}

	links 
	{
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"User32.lib",
		"Gdi32.lib",
		"Shell32.lib",
		"libnoise.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		defines 
		{
			"GLEW_STATIC"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"