project "Common-Headless"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "%{wks.location}/Walnut/vendor/glm",

      "%{wks.location}/Walnut/Walnut/Source",
      "%{wks.location}/Walnut-Networking/Source",

      "%{wks.location}/Walnut/vendor/spdlog/include",

      "%{wks.location}Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include"
   }

   links
   {
      "Walnut-Headless",
      "Walnut-Networking"
   }

   targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

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
