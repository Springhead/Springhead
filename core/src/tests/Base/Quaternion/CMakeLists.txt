# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)

# projects
set(Target "Quaternion")
message(STATUS "Target: ${Target}")
project(${Target} CXX)

# sytem environment
if("$ENV{SPR_TOP_DIR}" STREQUAL "")
    # assume current directory is ".../Springhead/core/src"
    set(SPR_TOP_DIR ${CMAKE_SOURCE_DIR}/../../../../..)
else()
    set(SPR_TOP_DIR $ENV{SPR_TOP_DIR})
endif()
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_BLD_DIR ${PROJECT_BINARY_DIR} ABSOLUTE)
set(SPR_SRC_DIR ${SPR_TOP_DIR}/core/src)
set(SPR_INC_DIR ${SPR_TOP_DIR}/core/include)
#
file(TO_NATIVE_PATH "${SPR_TOP_DIR}" SPR_TOP_DIR)
file(TO_NATIVE_PATH "${SPR_SRC_DIR}" SPR_SRC_DIR)
file(TO_NATIVE_PATH "${SPR_INC_DIR}" SPR_INC_DIR)
message(STATUS  ${Target}:SPR_TOP_DIR: [${SPR_TOP_DIR}])
message(STATUS  ${Target}:SPR_SRC_DIR: [${SPR_SRC_DIR}])
message(STATUS  ${Target}:SPR_INC_DIR: [${SPR_INC_DIR}])
message(STATUS  ${Target}:SPR_BLD_DIR: [${SPR_BLD_DIR}])

#
set(CMAKE_MAKE_SYSTEM_PATH ${SPR_TOP_DIR}/core/make.system.cmake)
include(${CMAKE_MAKE_SYSTEM_PATH})
message(STATUS "Architecture: ${ARCHITECTURE}")

# sources
file(GLOB SRCS "*.cpp")
if(${Windows})
    if("${ARCHITECTURE}" STREQUAL "x86")
	add_executable(${Target} ${SRCS} WIN32)
    else()
	add_executable(${Target} ${SRCS})
    endif()
endif()

#
set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# function definitions
set(CMAKE_MAKE_FUNC_PATH ${SPR_TOP_DIR}/core/make.func.cmake)
include(${CMAKE_MAKE_FUNC_PATH})

# compile options
if(${Windows})
    message(STATUS "Host System: Windows")
    set(CMAKE_CXX_FLAGS "/Zi /W3 /sdl /MP /D _LIB /GF- /EHsc /Gy- /Qpar \
			 /arch:AVX /openmp /wd4996 /wd4244 /FC")
    set(CMAKE_CXX_FLAGS_DEBUG   "/JMC /Od /Ob0 /Oy- /RTC1 /MDd /fp:except ")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /Ot /Oy /Gr")
    set(CMAKE_CXX_FLAGS_TRACE   "/Ob2 /Ot /D TRACE /Oy- /Gr")
    #
    set(CMAKE_EXE_LINKER_FLAGS  "/ERRORREPORT:PROMPT /IGNORE:4099 /NXCOMPAT \
				/MANIFEST:NO /MANIFESTUAC:YES /DYNAMICBASE /NOLOGO")
    if(${ARCHITECTURE} STREQUAL "x86")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X86")
    else()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X64")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS_TRACE ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
else()
endif()

# depending projects
#
add_subdirectory(${SPR_SRC_DIR}/Base ${SPR_BLD_DIR}/Base)
add_subdirectory(${SPR_SRC_DIR}/RunSwig ${SPR_BLD_DIR}/RunSwig)
add_dependencies(Base RunSwig)
add_dependencies(${Target} Base)

# include directories
#
target_include_directories(${Target}
    PRIVATE ${SPR_INC_DIR}
    PRIVATE ${SPR_SRC_DIR}
    PRIVATE ${SPR_TOP_DIR}/dependency/src
)

# end: Springhead
