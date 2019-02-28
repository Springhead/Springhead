# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)
#
set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_INSTALL_PREFIX F:/Project/Springhead_CMAKE/core/src/build)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# current platform
set(Windows ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
set(Linux ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")

# compile options
if(${Windows})
    message(STATUS "Host System: Windows")
    set (CMAKE_CXX_FLAGS "/Zi /W3 /sdl /MP /D _LIB /GF- /EHsc /Gy- /Qpar /arch:AVX /openmp /wd4996 /wd4244 /FC")
    set (CMAKE_CXX_FLAGS_DEBUG "/JMC /Od /Ob0 /Oy- /RTC1 /MDd /fp:except ")
    set (CMAKE_CXX_FLAGS_RELEASE "/Ob2 /Ot /Oy /Gr")
    set (CMAKE_CXX_FLAGS_TRACE "/Ob2 /Ot /D TRACE /Oy- /Gr")
elseif(${Linux})
endif()

# constants
set(ProjectName "Springhead")
if(${Windows})
    set(VS_VERSION "15.0")
elseif(${Linux})
endif()

# projects
#
project(${ProjectName} CXX)
add_subdirectory(Base)
add_subdirectory(Collision)
add_subdirectory(Creature)
add_subdirectory(FileIO)
add_subdirectory(Foundation)
add_subdirectory(Framework)
add_subdirectory(Graphics)
add_subdirectory(HumanInterface)
add_subdirectory(Physics)
add_subdirectory(RunSwig)

add_dependencies(Base RunSwig)
add_dependencies(Collision Foundation RunSwig)
add_dependencies(Creature Foundation Framework Physics RunSwig)
add_dependencies(FileIO Foundation RunSwig)
add_dependencies(Foundation Base RunSwig)
add_dependencies(Framework FileIO Foundation Graphics Physics RunSwig)
add_dependencies(Graphics Foundation RunSwig)
add_dependencies(HumanInterface Foundation RunSwig)
add_dependencies(Physics Collision Foundation RunSwig)

# main target
#
set(Target "Springhead")
set(OUTDIR_BASE ${CMAKE_SOURCE_DIR}/../../generated/lib)
if(${Windows})
	string(REPLACE "/" "\\" OUTDIR_BASE ${OUTDIR_BASE})
endif()
set(LIBNAME_DEBUG Springhead${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_RELEASE Springhead${VS_VERSION}${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_TRACE Springhead${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM})

if(${Windows})
    add_custom_target(${Target} ALL
	COMMAND cd ${CMAKE_SOURCE_DIR}
	COMMAND SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${VS_VERSION}${CMAKE_GENERATOR_PLATFORM}
	#COMMAND SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${OUTPUT_NAME}
	DEPENDS Base Collision Creature FileIO Foundation Framework Graphics HumanInterface Physics
	COMMENT [[  generating Springhead Library... ]]
    )
    set_target_properties(${Target} PROPERTIES
	OUTPUT_NAME_DEBUG   ${OUTDIR_BASE}\\${LIBNAME_DEBUG}
	OUTPUT_NAME_RELEASE ${OUTDIR_BASE}\\${LIBNAME_RELEASE}
	OUTPUT_NAME_TRACE   ${OUTDIR_BASE}\\${LIBNAME_TRACE}
    )
endif()

# end: Springhead
