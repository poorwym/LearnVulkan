project "Cubed-Server"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      path.getabsolute("../Cubed-Common/Source"),

      path.getabsolute("../Walnut/vendor/glm"),

      path.getabsolute("../Walnut/Walnut/Source"),
      path.getabsolute("../Walnut/Walnut/Platform/Headless"),

      path.getabsolute("../Walnut/vendor/spdlog/include"),
      path.getabsolute("../Walnut/vendor/yaml-cpp/include"),

      -- Walnut-Networking
      path.getabsolute("../Walnut/Walnut-Modules/Walnut-Networking/Source"),
      path.getabsolute("../Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include")

   }

   links
   {
       "Cubed-Common-Headless",
       "Walnut-Headless",
       "Walnut-Networking",

       "yaml-cpp",
   }

   	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      buildoptions {"/utf-8"}
      postbuildcommands 
	  {
	    '{COPY} "../%{WalnutNetworkingBinDir}/GameNetworkingSockets.dll" "%{cfg.targetdir}"',
	    '{COPY} "../%{WalnutNetworkingBinDir}/libcrypto-3-x64.dll" "%{cfg.targetdir}"',
	    '{COPY} "../%{WalnutNetworkingBinDir}/libprotobufd.dll" "%{cfg.targetdir}"',
	  }

   filter "system:linux"
      libdirs { "../Walnut/Walnut-Networking/vendor/GameNetworkingSockets/bin/Linux" }
      links { "GameNetworkingSockets" }

       defines { "WL_HEADLESS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"