# CMakeLists.txt for Collision

# ------------------------------------------------------------------------------
#  Set target name.
#
set(Target "Collision")
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Read setting parameters.
#
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
endif()

# ------------------------------------------------------------------------------
#  Define sources and headers.
#
file(STRINGS "RunSwig_gen_files.txt" _tmp_buff)
string(REPLACE " " ";" _tmp_buff ${_tmp_buff})
foreach(_file ${_tmp_buff})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_file})
	message("     create dummy file: ${_file}")
	file(TOUCH ${_file})
    endif()
endforeach()
#
file(GLOB SRCS *.cpp *.h)
file(GLOB COMMON_HDRS
	${SPR_INC_DIR}/${Target}/*.h
)
file(GLOB EXCLUDE_SRCS "")

list(APPEND SRCS ${COMMON_HDRS})
if(EXCLUDE_SRCS)
    message("++ exclude: ${EXCLUDE_SRCS}")
    list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()

# ------------------------------------------------------------------------------
#  Definition of library.
#
if(${Windows})
    add_library(${Target} ${LIBTYPE} ${SRCS})
    set(_platform ${CMAKE_GENERATOR_PLATFORM})
    set(_output_name_debug   "${Target}${VS_VERSION}${LIB_SUFFIX_DEBUG}${_platform}")
    set(_output_name_release "${Target}${VS_VERSION}${LIB_SUFFIX_RELEASE}${_platform}")
    set(_output_name_trace   "${Target}${VS_VERSION}${LIB_SUFFIX_TRACE}${_platform}")
    set_target_properties("${Target}" PROPERTIES
	OUTPUT_NAME_DEBUG   ${_output_name_debug}
	OUTPUT_NAME_RELEASE ${_output_name_release}
	OUTPUT_NAME_TRACE   ${_output_name_trace}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_TRACE   ${SPR_SRC_DIR}/${Target}
    )
    #message("     lib name debug:   ${_output_name_debug}")
    #message("     lib name release: ${_output_name_release}")
    #message("     lib name trace:   ${_output_name_trace}")
elseif(${Linux})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    add_library(${Target} ${LIBTYPE} ${SRCS})
endif()

# ------------------------------------------------------------------------------
#  Compiler/linker options.
#
if(${Windows})
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
elseif(${unix})
    target_compile_options(${Target} PUBLIC ${COMP_FLAGS}>)
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()
target_compile_definitions(${Target} PUBLIC ${DEFINE_MACROS})

# ------------------------------------------------------------------------------
#  Include directories.
#
set(CMAKE_USE_RELATIVE_PATHS TRUE)
target_include_directories(${Target} BEFORE PRIVATE ${INCLUDE_PATHS})
if(DEFINED Boost_INCLUDE_DIRS)
    target_include_directories(${Target} BEFORE PRIVATE ${Boost_INCLUDE_DIRS})
endif()
if(DEFINED GLEW_INCLUDE_DIRS)
    target_include_directories(${Target} BEFORE PRIVATE ${GLEW_INCLUDE_DIRS})
endif()
if(DEFINED GLUT_INCLUDE_DIR)
    target_include_directories(${Target} BEFORE PRIVATE ${GLUT_INCLUDE_DIR})
endif()

# ------------------------------------------------------------------------------
#  Integrate intermediate directory.
#	${CMAKE_BINARY_DIR}/${Target}/${Target}.dir
#	linked to (for Windows)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${Target}.dir
#	or (for unix)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${Target}.dir
#
set(INTDIR ${Target}.dir)
set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE})
if(${Windows})
    set(ACTDIR ${ACTDIR}/${VS_VERSION})
    set(LINKDIR ${INTDIR})
elseif(${Linux})
    set(LINKDIR .${CMAKE_FILES_DIRECTORY}/${INTDIR})
endif()
set(LINKLIST
    "${LINKDIR}=${ACTDIR}/${INTDIR}"
)
set(_script_dir ${SPR_SRC_DIR})
if(${Windows})
    set(_tmp_cmnd cmd /c ${SPR_SRC_DIR}\\do_python.bat)
    set(_tmp_args ${_script_dir}\\cmake_preconfig.py -v ${LINKLIST})
else()
    set(_tmp_cmnd python)
    set(_tmp_args ${_script_dir}/cmake_preconfig.py -v ${LINKLIST})
endif()
set(ENV{_SPRTOP_} ${SPR_TOP_DIR})
execute_process(
    COMMAND ${_tmp_cmnd} ${_tmp_args}
    RESULT_VARIABLE CMD_RESULT
    OUTPUT_VARIABLE CMD_OUTPUT
    ERROR_VARIABLE CMD_ERROR
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
if(${CMD_RESULT})
    message("Error: ${CMD_ERROR}")
endif()
if(NOT "${CMD_OUTPUT}" STREQUAL "")
    string(REPLACE "\n" "\n     " CMD_OUTPUT ${CMD_OUTPUT})
endif()
string(STRIP ${CMD_OUTPUT} CMD_OUTPUT)
message("     ${CMD_OUTPUT}")

# end: CMakeLists.txt
