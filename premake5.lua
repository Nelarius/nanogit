require "extern/common/common"

workspace "nanogit"

    if _ACTION then
        location("build/" .._ACTION)
    end

    configurations {"Debug", "Release", "Test"}

    architecture "x86_64"

    filter "configurations:Debug"
        defines "NLRS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

    filter "configurations:Test"
        defines "NLRS_DEBUG"
        optimize "On"

    filter { "action:gmake or action:xcode*", "language:C++" }
        buildoptions { "-std=c++14" }

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    --[[
            __             
    _____ __/ /____ _______ 
    / -_) \ / __/ -_) __/ _ \
    \__/_\_\\__/\__/_/ /_//_/
                            
    --]]

    group "extern"

    project "nanovg"
        kind "StaticLib"
        language "C"
        targetdir "lib"
        files { "extern/nanovg/**.c", "extern/nanovg/**.h" }
        includedirs { "extern/common/extern", "extern/nanovg", "extern/common/extern/gl3w/include" }
        filter "toolset:msc"
            disablewarnings "4267"
        filter "toolset:clang or toolset:gcc"
            disablewarnings "constant-conversion"

    project_gl3w("extern")

    project_window("extern", "extern/SDL/include")

    project_common("extern")

    project_test("extern")

    --[[
                    
    _______  _______ 
    / __/ _ \/ __/ -_)
    \__/\___/_/  \__/ 
                    
    --]]

    group "core"

    project "nanogit"
        kind "ConsoleApp"
        language "C++"
        targetdir "bin"
        files { "src/**.cpp", "src/**.h" }
        includedirs {
            "src", "extern", "extern/common/extern/gl3w/include",
            "extern/imgui/", "extern/SDL/include",
            "extern/nanovg",
            "extern/libgit2/include",
            "extern/common/include"
        }
        links { "gl3w", "nanovg", "window", "common" }

        postbuildcommands { "{COPY} ../../resources/* %{cfg.targetdir}" }

        filter "system:macosx"
            includedirs { "/Library/Frameworks/SDL2.framework/Headers" }
            links { "git2.24", "Cocoa.framework" }
            libdirs { "extern/libgit2/bin/osx" }
            linkoptions { "-F/Library/Frameworks -framework SDL2" }

        filter "action:vs*"
            debugdir "bin"
            includedirs { "extern/SDL/include" }
            libdirs { "extern/libgit2/bin/win64", "extern/SDL/bin/win64" }
            links { "SDL2", "SDL2main", "opengl32", "git2" }
            postbuildcommands {
                "{COPY} ../../extern/SDL/bin/win64/SDL2.dll %{cfg.targetdir}",
                "{COPY} ../../extern/libgit2/bin/win64/git2.dll %{cfg.targetdir}"
            }
