# CMakeLists.txt for Collision

# ------------------------------------------------------------------------------
#  Set target name.
#
set(Target "Foundation")
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
file(GLOB SRCS *.cpp *.h
	QuadProg++.cc
	QuadProg++.hh
)
file(GLOB COMMON_HDRS
	${SPR_INC_DIR}/SprFoundation.h
	${SPR_INC_DIR}/${Target}/*.h)
file(GLOB EXCLUDE_SRCS 
	Foundation.h
)

list(APPEND SRCS ${COMMON_HDRS})
if(EXCLUDE_SRCS)
    message("++ exclude: ${EXCLUDE_SRCS}")
    list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()

if(unix)
    list(APPEND SRCS ../kludge_for_unix/kludge.cpp)
endif()

# ------------------------------------------------------------------------------
#  Definition of library.
#
if(Windows)
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
elseif(unix)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    add_library(${Target} STATIC ${SRCS})
endif()

# ------------------------------------------------------------------------------
#  Compiler/linker options.
#
if(Windows)
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
elseif(unix)
    foreach(_flag ${COMP_FLAGS})
        target_compile_options(${Target} PUBLIC ${_flag})
    endforeach()
    #set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()
foreach(_macro ${DEFINE_MACROS})
        target_compile_definitions(${Target} PUBLIC ${_macro})
endforeach()

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
# set python be invokable
#
if(Windows AND EXISTS ${SPR_TOP_DIR}/buildtool/win32/python.exe)
    set(_python ${SPR_TOP_DIR}\\buildtool\\win32\\python.exe)
else()
    set(_python python)
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
if(Windows)
    set(ACTDIR ${ACTDIR}/${VS_VERSION})
    set(LINKDIR ${INTDIR})
elseif(unix)
    set(LINKDIR .${CMAKE_FILES_DIRECTORY}/${INTDIR})
endif()
set(LINKLIST
    "${LINKDIR}=${ACTDIR}/${INTDIR}"
)
set(_script ${SPR_SRC_DIR}/cmake_preconfig.py)
file(TO_NATIVE_PATH "${_script}" _script)
set(_tmp_args -v ${LINKLIST})
execute_process(
    COMMAND ${_python} ${_script} ${_tmp_args}
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

# ------------------------------------------------------------------------------
#  Record this build state.
#
add_custom_command(TARGET ${Target}
    POST_BUILD
    COMMAND echo "LASTLIBTYPE=${LIBTYPE}" > ${LASTBUILD_LIBTYPE_FILE}
    BYPRODUCTS ${LASTBUILD_LIBTYPE_FILE}
    COMMANT [[  last build library type: ${LIBTYPE}  ]]
)

# end: CMakeLists.txt
