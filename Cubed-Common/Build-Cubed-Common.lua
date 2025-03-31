project "Cubed-Common"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      path.getabsolute("../Walnut/vendor/imgui"),
      path.getabsolute("../Walnut/vendor/glfw/include"),
      path.getabsolute("../Walnut/vendor/glm"),

      path.getabsolute("../Walnut/Walnut/Source"),
      path.getabsolute("../Walnut-Networking/Source"),

      "%{IncludeDir.VulkanSDK}",
      path.getabsolute("../Walnut/vendor/spdlog/include"),

      path.getabsolute("../Walnut-Networking/vendor/GameNetworkingSockets/include")
   }

   links
   {
       "Walnut",
       "Walnut-Networking",
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

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