# CMakeLists.txt for 'swig' binary generation.

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Basic definitions.
#
set(ProjectName swig)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

if(NOT DEFINED SPR_CORE_DIR)
	set(SPR_CORE_DIR ${CMAKE_SOURCE_DIR}/../../../..)
endif()
set(SWIG_INSTALL_DIR ${SPR_CORE_DIR}/bin/swig)
get_filename_component(SWIG_INSTALL_DIR ${SWIG_INSTALL_DIR} ABSOLUTE)

# ------------------------------------------------------------------------------
#  Definition of project.
#
message(STATUS "Project: ${ProjectName}")
message(STATUS "Host System: ${CMAKE_HOST_SYSTEM_NAME}")
project(${ProjectName} C CXX)
set(Target ${ProjectName})
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)

# ------------------------------------------------------------------------------
#  Sytem environment.
#
include("${SPR_CORE_DIR}/make.system.cmake")
message(STATUS "Architecture: ${ARCHITECTURE}")
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})

# ------------------------------------------------------------------------------
#  Read function definitions.
#
include("${SPR_CORE_DIR}/make.func.cmake")

# ------------------------------------------------------------------------------
#  Main target.
#
set(SRC_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Source)
file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	${SRC_BASE_DIR}/CParse/*.h ${SRC_BASE_DIR}/CParse/*.c
	${SRC_BASE_DIR}/DOH/*.h ${SRC_BASE_DIR}/DOH/*.c
	${SRC_BASE_DIR}/include/*.h
	${SRC_BASE_DIR}/Modules/*.h ${SRC_BASE_DIR}/Modules/*.cpp
				    ${SRC_BASE_DIR}/Modules/*.cxx
	${SRC_BASE_DIR}/Modules/sprpy/*.h ${SRC_BASE_DIR}/Modules/sprpy/*.cpp
	${SRC_BASE_DIR}/Preprocessor/*.h ${SRC_BASE_DIR}/Preprocessor/*.c
	${SRC_BASE_DIR}/Swig/*.h ${SRC_BASE_DIR}/Swig/*.c)
set(EXCLUDE_SRCS ${EMPTY})

if(EXCLUDE_SRCS)
	message("++ exclude: ${EXCLUDE_SRCS}")
	list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()
add_executable(${Target} ${SRCS})

# ------------------------------------------------------------------------------
#  Compiler options.
#
set(COPT_DEFAULT	"/Zi /W3 /WX- /diagnostics:classic /MP \
			 /Gm- /EHsc /fp:precise \
			 /wd\"4244\" /wd\"4267\" \
			 /Zc:wchar_t /Zc:forScope /Zc:inline \
			 /Gd /FC")
set(COPT_DEBUG		"/Od /Oy- /RTC1 /MDd /GS")
set(COPT_RELEASE	"/JMC /O2 /MD /GS-")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
set(CMAKE_CXX_FLAGS		"${COPT_DEFAULT}")
set(CMAKE_CXX_FLAGS_DEBUG	"${COPT_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE	"${COPT_RELEASE}")
#
foreach(_macro ${DEFINE_MACROS})
	target_compile_definitions(${Target} PRIVATE ${_macro})
endforeach()

# ------------------------------------------------------------------------------
#  Include directories.
#
set(CMAKE_USE_RELATIVE_PATHS TRUE)
target_include_directories(${Target} BEFORE PRIVATE
	../Source/swig
	../Source/Include
	../Source/doh
	../Source/Preprocessor
	../Source/Modules
	../Source/CParse
	../../../../src
	../../../../include)

# ------------------------------------------------------------------------------
#  Linker options.
#
string(TOUPPER ${ARCHITECTURE} _arch)
set(LOPT_DEFAULT	"/NOLOGO /MANIFEST /MANIFESTUAC: \
			 /level='asInvoker' /uiAccess:'false' \
			 /DEBUG /SUBSYSTEM:CONSOLE \
			 /ERRORREPORT:QUEUE \
			 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:${_arch}")
set(LOPT_DEBUG	/NOINCREMENTAL)
set(LOPT_RELEASE	/OPT:REF /OPT:ICF)
#
set(CMAKE_EXE_LINKER_FLAGS		"${LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG	"${LINK_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE	"${LINK_FLAGS_RELEASE}")

# ------------------------------------------------------------------------------
#  Set working directory to VS debugger.
#
foreach(_prop DEBUGGER_WORKING_DIRECTORY DEBUGGER_COMMAND_ARGUMENTS)
    if(DEFINED ${_prop})
	set_target_properties(${Target} PROPERTIES VS_${_prop} ${${_prop}})
    endif()
endforeach()

# ------------------------------------------------------------------------------
#  Install swig binary.
#
set(_exe ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${Target}.exe)
file(TO_NATIVE_PATH "${_exe}" _exe)
file(TO_NATIVE_PATH "${SWIG_INSTALL_DIR}" _install)

set(_makefile ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.SwigInstall)
set(_make_args SWIGBINARY=${_exe} INSTALLDIR=${_install})
add_custom_target(swig_install ALL
	COMMAND nmake /NOLOGO -f ${_makefile} ${_make_args}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	DEPENDS ${Target}
	#COMMENT [[  copying swig binary... ]]
)

# end: CMakeLists.txt
