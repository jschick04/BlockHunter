workspace "BlockHunter"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Client"

    defines { "IMGUI_DEFINE_MATH_OPERATORS" }

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir = "Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "Walnut/Build-Walnut-External.lua"

group "App"
    include "Common/build-common.lua"
    include "Client/build-client.lua"
group ""
