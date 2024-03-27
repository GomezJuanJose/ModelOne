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
    kind "SharedLib"
    language "C++"

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
        cppdialect "C++17"
        staticruntime "On" --Links the run time libraries as static
        systemversion "latest"

        defines{
            "TL_PLATFORM_WINDOWS",
            "TL_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "TL_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "TL_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
        buildoptions "/MD"
        optimize "On"


project "Sandbox"
    location "SandboxProjects/Sandbox"
    kind "ConsoleApp"
    language "C++"

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
        cppdialect "C++17"
        staticruntime "On" --Links the run time libraries as static
        systemversion "latest"

        defines{
            "TL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "TL_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "TL_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
        buildoptions "/MD"
        optimize "On"