project "Client"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "%{wks.location}/Common/src",

      "%{wks.location}/Walnut/vendor/imgui",
      "%{wks.location}/Walnut/vendor/glfw/include",
      "%{wks.location}/Walnut/vendor/glm",
      "%{wks.location}/Walnut/vendor/spdlog/include",

      "%{wks.location}/Walnut/Walnut/Source",
      "%{wks.location}/Walnut/Walnut/Platform/GUI",

      "%{IncludeDir.VulkanSDK}",

      -- Walnut-Networking
      "%{wks.location}/Walnut/Walnut-Modules/Walnut-Networking/Source",
      "%{wks.location}/Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include"
   }

    links
    {
        "Common",
        "Walnut"
    }

   targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

      postbuildcommands
      {
         '{COPY} "../%{WalnutNetworkingBinDir}/GameNetworkingSockets.dll" "%{cfg.targetdir}"',
         '{COPY} "../%{WalnutNetworkingBinDir}/libcrypto-3-x64.dll" "%{cfg.targetdir}"',
         '{COPY} "../%{WalnutNetworkingBinDir}/libprotobufd.dll" "%{cfg.targetdir}"',
      }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "on"
      symbols "on"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "on"
      symbols "off"
