project "Cubed-Client"
   kind "WindowedApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      path.getabsolute("../Cubed-Common/Source"),

      path.getabsolute("../Walnut/vendor/imgui"),
      path.getabsolute("../Walnut/vendor/glfw/include"),

      path.getabsolute("../Walnut/Walnut/Source"),
      path.getabsolute("../Walnut/Walnut/Platform/GUI"),

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",

      -- Walnut-Networking
      path.getabsolute("../Walnut/Walnut-Modules/Walnut-Networking/Source"),
      path.getabsolute("../Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include")
   }

    links
    {
        "Cubed-Common",
        "Walnut"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

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
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
