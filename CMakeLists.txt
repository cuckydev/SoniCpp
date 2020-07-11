cmake_minimum_required(VERSION 3.12)

###########
# Options #
###########
option(SCPP_COMPILE_SDL2 "Enable the SDL2 backend" ON)

#########
# Setup #
#########

# Define project, source, and includes
project(SoniCpp LANGUAGES CXX)

add_executable(SoniCpp WIN32
	"src/Main.cpp"
	"src/Error.cpp"
	"src/Error.h"
	"src/Engine.cpp"
	"src/Engine.h"
	"src/VDP.cpp"
	"src/VDP.h"
	"src/Backend/Core.h"
	"src/Backend/Render.h"
)

target_include_directories(SoniCpp PRIVATE "src")

# Force strict C++17
set_target_properties(SoniCpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

############
# Backends #
############

# Link and include SDL2 backend
if(SCPP_COMPILE_SDL2)
	# Include the backend code
	target_compile_definitions(SoniCpp PRIVATE SCPP_COMPILE_SDL2)
	target_sources(SoniCpp PRIVATE	"src/Backend/SDL2/Core.cpp"
									"src/Backend/SDL2/Core.h"
									"src/Backend/SDL2/Render.cpp"
									"src/Backend/SDL2/Render.h")
	
	# Link SDL2
	find_package(SDL2)
	target_link_libraries(SoniCpp PRIVATE SDL2::SDL2)
endif()

##########
# Tweaks #
##########
# This is messy as hell, and has been replaced by CMAKE_MSVC_RUNTIME_LIBRARY,
# but that's a very recent CMake addition, so we're still doing it this way for now
if(MSVC AND MSVC_LINK_STATIC_RUNTIME)
	# Statically-link the CRT (vcpkg static libs do this)
	foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif()
	endforeach()
endif()

# Do some other MSVC fixes
if(MSVC)
	# Disable warnings that normally fire up on MSVC when using "unsafe" functions instead of using MSVC's "safe" _s functions
	target_compile_definitions(SoniCpp PRIVATE _CRT_SECURE_NO_WARNINGS)
	
	# Make it so source files are recognized as UTF-8 by MSVC
	target_compile_options(SoniCpp PRIVATE "/utf-8")
	
	# Use `main` instead of `WinMain`
	set_target_properties(SoniCpp PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
