workspace "Primitivo"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Distribution"
    }

    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Primitivo/vendor/GLFW/include"
IncludeDir["Glad"] = "Primitivo/vendor/Glad/include"
IncludeDir["ImGui"] = "Primitivo/vendor/ImGui"
IncludeDir["glm"] = "Primitivo/vendor/glm"
IncludeDir["stb_image"] = "Primitivo/vendor/stb_image"
IncludeDir["SoLoud"] = "Primitivo/vendor/SoLoud/include"


group "Dependencies"
    include "Primitivo/vendor/GLFW"
    include "Primitivo/vendor/Glad"
    include "Primitivo/vendor/ImGui"
    include "Primitivo/vendor/SoLoud"
group""


project "Primitivo"
    location "Primitivo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" --Links the run time libraries as static

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "plpch.h"
    pchsource "Primitivo/src/plpch.cpp" --For visual studio

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.SoLoud}"
        --Use for third parties libs in this case
        --"%{prj.name}/vendor/THIRD_PARTY_LIB/include"
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib",
        "SoLoud"
    }

    filter "system:windows" --Tabulation doesnt mean anything, from this line below is applied only for windows
        systemversion "latest"

        defines{
            "TL_PLATFORM_WINDOWS",
            "TL_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "TL_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "TL_RELEASE"
        optimize "on"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
        optimize "on"


project "Sandbox"
    location "SandboxProjects/Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" --Links the run time libraries as static

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files{
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
    }

    includedirs{
        --Use for third parties libs in this case
        --"Primitivo/vendor/THIRD_PARTY_LIB/include",
        "Primitivo/src",
        "Primitivo/vendor",
        "%{IncludeDir.glm}"
    }

    links{
        "Primitivo" --Links the sandobx with the engine (the dynamic lib)
    }

    filter "system:windows" --Tabulation doesnt mean anything, from this line below is applied only for windows 
        systemversion "latest"

        defines{
            "TL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "TL_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "TL_RELEASE"
        optimize "on"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
        optimize "on"