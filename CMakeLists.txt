cmake_minimum_required(VERSION 3.22)

project(
	AutoInputSwitch
	VERSION 1.2.1
	LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds are not allowed."
	)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio")
	option(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD "Include INSTALL target to default build." OFF)
endif()

get_filename_component(
	Skyrim64Path
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim Special Edition;installed path]"
	ABSOLUTE CACHE
)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${Skyrim64Path}/Data" CACHE PATH
		"Install path prefix (e.g. Skyrim Data directory or Mod Organizer virtual directory)."
		FORCE
	)
endif()

list(APPEND CMAKE_IGNORE_PATH "${PROJECT_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(AddCXXFiles)

add_library(${PROJECT_NAME} SHARED)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_20
)

add_cxx_files("${PROJECT_NAME}")

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h
		${CMAKE_CURRENT_BINARY_DIR}/version.rc
		.clang-format
		.editorconfig
		README.md
		vcpkg.json
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

add_subdirectory(extern/CommonLibSSE CommonLibSSE EXCLUDE_FROM_ALL)

find_package(spdlog CONFIG REQUIRED)
find_path(SIMPLEINI_INCLUDE_DIRS "ConvertUTF.c")

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		CommonLibSSE::CommonLibSSE
		spdlog::spdlog
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${SIMPLEINI_INCLUDE_DIRS}
)

install(
	FILES
		"$<TARGET_FILE:${PROJECT_NAME}>"
		"$<TARGET_PDB_FILE:${PROJECT_NAME}>"
	DESTINATION "SKSE/Plugins"
)

install(
	FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/data/${PROJECT_NAME}.ini"
	DESTINATION "SKSE/Plugins"
)

set(CPACK_GENERATOR "7Z")
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
include(CPack)
