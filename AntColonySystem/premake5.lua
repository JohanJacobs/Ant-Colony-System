workspace "Ant Colony system"
    
    configurations {"Debug", "Release"}
    outputdir = "%{cfg.buildcfg}"

project "ACS"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"      

        targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")      

        flags 
        {   
            "MultiProcessorCompile"
        }

        files 
        {
            "src/**.cpp" -- recursively add all cpp files uned src folder            
        }

        includedirs 
        {
            "Vendor/Effolkronium/"
        }

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"
  
        filter "configurations:Release"
            defines { "RELEASE" }
            optimize "Full"
            