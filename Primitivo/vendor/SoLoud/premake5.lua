project "SoLoud"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/**.h",
		"src/core/**.cpp",
		"src/filter/**.cpp",
		"src/audiosource/wav/**.h",
		"src/audiosource/wav/**.cpp",
		"src/audiosource/wav/**.c"
	}

	includedirs{
		"include",
		"src"
	}

	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"src/backend/wasapi/**.cpp"
		}
		
		defines{
			"WITH_WASAPI"
		}

	filter "system:linux"
		pic "On"
		systemversion "latest"

		files
		{
			"src/backend/oss/**.cpp"
		}

		defines{
			"WITH_OSS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
        symbols "off"