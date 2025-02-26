workspace "ModelOne"
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
IncludeDir["GLFW"] = "ModelOne/vendor/GLFW/include"
IncludeDir["Glad"] = "ModelOne/vendor/Glad/include"
IncludeDir["ImGui"] = "ModelOne/vendor/ImGui"
IncludeDir["glm"] = "ModelOne/vendor/glm"
IncludeDir["stb_image"] = "ModelOne/vendor/stb_image"
IncludeDir["SoLoud"] = "ModelOne/vendor/SoLoud/include"


group "Dependencies"
    include "ModelOne/vendor/GLFW"
    include "ModelOne/vendor/Glad"
    include "ModelOne/vendor/ImGui"
    include "ModelOne/vendor/SoLoud"
group""


project "ModelOne"
    location "ModelOne"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" --Links the run time libraries as static

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "mopch.h"
    pchsource "ModelOne/src/mopch.cpp" --For visual studio

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
            "MO_PLATFORM_WINDOWS",
            "MO_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "MO_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "MO_RELEASE"
        optimize "on"

    filter "configurations:Distribution"
        defines "MO_DISTRIBUTION"
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
        --"ModelOne/vendor/THIRD_PARTY_LIB/include",
        "ModelOne/src",
        "ModelOne/vendor",
        "%{IncludeDir.glm}"
    }

    links{
        "ModelOne" --Links the sandobx with the engine (the dynamic lib)
    }

    filter "system:windows" --Tabulation doesnt mean anything, from this line below is applied only for windows 
        systemversion "latest"

        defines{
            "MO_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "MO_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "MO_RELEASE"
        optimize "on"

    filter "configurations:Distribution"
        defines "MO_DISTRIBUTION"
        optimize "on"