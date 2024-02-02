workspace "Taller"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Taller"
    location "Taller"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        --Use for third parties libs in this case
        --"%{prj.name}/vendor/THIRD_PARTY_LIB/include"
    }

    filter "system:windows" --Tabulation doesnt mean anything, from this line below is applied only for windows
        cppdialect "C++17"
        staticruntime "On" --Links the run time libraries as static
        systemversion "latest"

        defines{
            "TL_PLATFORM_WINDOWS",
            "TL_BUILD_DLL"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "TL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TL_RELEASE"
        optimize "On"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
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
        "Taller/src"
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
        symbols "On"

    filter "configurations:Release"
        defines "TL_RELEASE"
        optimize "On"

    filter "configurations:Distribution"
        defines "TL_DISTRIBUTION"
        optimize "On"