workspace "BlockHunter-Server"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Server"

    defines { "WL_HEADLESS" }

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir = "Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "Walnut/Build-Walnut-Headless-External.lua"

group "App"
    include "Common/build-common-headless.lua"
    include "Server/build-server.lua"
group ""
