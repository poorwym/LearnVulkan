-- premake5.lua
workspace "Cubed-Server"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Cubed-Server-Headless"

   -- Workspace-wide defines
   defines
   {
       "WL_HEADLESS"
   }

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

   filter "system:macosx"
      buildoptions {
          "-Wno-c++11-compat",
          "-Wno-c++98-compat",
          "-Wno-c++98-compat-pedantic" 
        }
-- Directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir = "Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "Walnut/Build-Walnut-Headless-External.lua"

group "App"
    include "Cubed-Common/Build-Cubed-Common-Headless.lua"
    include "Cubed-Server/Build-Cubed-Server-Headless.lua"
group ""