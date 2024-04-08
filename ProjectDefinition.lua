workspace "Taller"
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
IncludeDir["GLFW"] = "Taller/vendor/GLFW/include"
IncludeDir["Glad"] = "Taller/vendor/Glad/include"
IncludeDir["ImGui"] = "Taller/vendor/ImGui"
IncludeDir["glm"] = "Taller/vendor/glm"

group "Dependencies"
    include "Taller/vendor/GLFW"
    include "Taller/vendor/Glad"
    include "Taller/vendor/ImGui"
group""


project "Taller"
    location "Taller"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" --Links the run time libraries as static

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "tlpch.h"
    pchsource "Taller/src/tlpch.cpp" --For visual studio

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
        --Use for third parties libs in this case
        --"%{prj.name}/vendor/THIRD_PARTY_LIB/include"
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib"
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
        --"Taller/vendor/THIRD_PARTY_LIB/include",
        "Taller/src",
        "Taller/vendor",
        "%{IncludeDir.glm}"
    }

    links{
        "Taller" --Links the sandobx with the engine (the dynamic lib)
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