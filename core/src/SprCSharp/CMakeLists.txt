# CMakeLists.txt for SprCSharp

cmake_minimum_required(VERSION 3.1)
set(SPRCSHARP_PROJECT_VERSION 1.0.0)

# ------------------------------------------------------------------------------
#  Definition of project.
#
set(ProjectName SprCSharpMain)
set(ProjectVersion ${SPRCSHARP_PROJECT_VERSION})
project(${ProjectName} VERSION ${ProjectVersion} LANGUAGES CXX CSharp)
message(STATUS "Project: ${ProjectName}, Version ${ProjectVersion}")

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)

# ------------------------------------------------------------------------------
#  We set top directory of Springhead tree to "TOPDIR".
#
set(TOPDIR "${CMAKE_SOURCE_DIR}/../../..")

set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_CORE_DIR "${TOPDIR}/core")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
set(SPR_INC_DIR  "${TOPDIR}/core/include")
set(SPR_CSHARP_DIR "${TOPDIR}/core/src/SprCSharp")
set(BUILDTOOL_DIR "${TOPDIR}/buildtool/win32")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_COREDIR ${SPR_CORE_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(SPR_INC_DIR ${SPR_INC_DIR} ABSOLUTE)
get_filename_component(SPR_CSHARP_DIR ${SPR_CSHARP_DIR} ABSOLUTE)
get_filename_component(BUILDTOOL_DIR ${BUILDTOOL_DIR} ABSOLUTE)
message(STATUS "SPR_TOP_DIR:     [${SPR_TOP_DIR}]")
message(STATUS "SPR_CORE_DIR:    [${SPR_CORE_DIR}]")
message(STATUS "SPR_SRC_DIR:     [${SPR_SRC_DIR}]")
message(STATUS "SPR_INC_DIR:     [${SPR_INC_DIR}]")
message(STATUS "SPR_CSHARP_DIR:  [${SPR_CSHARP_DIR}]")
message(STATUS "BUILDTOOL_DIR:   [${BUILDTOOL_DIR}]")

# ------------------------------------------------------------------------------
#  Sytem environment.
#
include("${SPR_CORE_DIR}/make.system.cmake")
message(STATUS "Host System: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Architecture: ${ARCHITECTURE}")
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})

# ------------------------------------------------------------------------------
#  Read function definitions.
#
include("${SPR_CORE_DIR}/make.func.cmake")

# ------------------------------------------------------------------------------
#  Read compiler/linker option file (try to find in following order).
#	(1) File specified by option "-D OPTS=<file>".
#	(2) "${CMAKE_SOURCE_DIR}/CMakeOpts.txt".
#	(3) Default file: "${SPR_SRC_DIR}/CMakeOpts.txt.dist".
#
set(OPTS_FILE_NAME "CMakeOpts.txt")
set(OPTS_FILE "${CMAKE_SOURCE_DIR}/${OPTS_FILE_NAME}")
set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
message("include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE}")
include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Read setting parameters.
#
set(SETTING_FILE CMakeSettings.txt)
include(${SETTING_FILE})

# ------------------------------------------------------------------------------
#  Merge setting parameters.
#	Additional parameters are already read at the top of this file.
#	We need merge process here.
#
set(DEFINE_MACROS	${DEFINE_MACROS} ${DEFINE_MACROS_ADD})
set(INCLUDE_PATHS	${INCLUDE_PATHS_ADD} ${INCLUDE_PATHS})
set(COMP_FLAGS		${COMP_FLAGS_ADD} ${COMP_FLAGS})
set(LINK_FLAGS		${LINK_FLAGS_ADD} ${LINK_FLAGS})

# ------------------------------------------------------------------------------
#  SprCSharp をビルドするには python と nmake が必要となる.
#  これらのパスは ../NameManger にあるスクリプト NameManager.py を実行して生成
#  される NameManager.cmake をインクルードすればよい.
#  ただしこのスクリプトを実行するのにも python が必要で…　よって python だけは
#  ここで仮にパスを設定する.
#
set(PYTHON "${TOPDIR}/buildtool/win32/python.exe")
if(NOT EXISTS ${PYTHON})
    set(PYTON "${TOPDIR}/../DailyBuildTestTools/Python/pyton.exe")
    if(NOT EXISTS ${PYTHON})
	set(PYTHON python)	# default python
    endif()
endif()
execute_process(
    COMMAND echo ${PYTHON} NameManager.py --cmake
    COMMAND ${PYTHON} NameManager.py --cmake
    WORKING_DIRECTORY ${SPR_CSHARP_DIR}/NameManager
)
include(NameManager/NameManager.cmake)
message(STATUS "using ${PYTHON}")
message(STATUS "using ${NMAKE}")
file(TO_NATIVE_PATH ${PYTHON} PYTHON)
file(TO_NATIVE_PATH ${NMAKE} NMAKE)

# ------------------------------------------------------------------------------
#  Some other definitions.
#
if(Windows)
    set(RunSwigDir ${SPR_SRC_DIR}/RunSwig)
    file(TO_NATIVE_PATH "${RunSwigDir}" RunSwigDir)
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
elseif(unix)
    set(CMAKE_CXX_COMPILER "${GPP_PATH}/g++")
    set(Python python)
endif()

# ------------------------------------------------------------------------------
#  Definition of last build library type record file.
#
set(LASTBUILD_LIBTYPE_STATIC ${SPR_SRC_DIR}/lastbuilt.static)
set(LASTBUILD_LIBTYPE_SHARED ${SPR_SRC_DIR}/lastbuilt.shared)
if("${LIBTYPE}" STREQUAL "STATIC")
    set(LASTBUILD_LIBTYPE_FILE ${LASTBUILD_LIBTYPE_STATIC})
else()
    set(LASTBUILD_LIBTYPE_FILE ${LASTBUILD_LIBTYPE_SHARED})
endif()

# ------------------------------------------------------------------------------
#  Define depending projects.
#
message(STATUS "TARGET_TYPE: ${TARGET_TYPE}")
if("${TARGET_TYPE}" STREQUAL "ALL")
    set(_tmpf allprojs.cmake)
    execute_process(
	COMMAND echo ${PYTHON} make_projs_file.py ${_tmpf} SPR_PROJS --cmake
	COMMAND ${PYTHON} make_projs_file.py ${_tmpf} SPR_PROJS --cmake
	WORKING_DIRECTORY ${SPR_CSHARP_DIR}/RunSwig_CSharp
    )
    include(RunSwig_CSharp/${_tmpf})
else()
    set(_target_type ${TARGET_TYPE})
    string(REPLACE "PH" "Physics" _target_type ${TARGET_TYPE})
    message("   target type -> ${_target_type}")
    set(_tmpf module.depts)
    execute_process(
	COMMAND echo ${PYTHON} make_projs_file.py ${_tmpf}
	COMMAND ${PYTHON} make_projs_file.py ${_tmpf}
	WORKING_DIRECTORY ${SPR_CSHARP_DIR}/RunSwig_CSharp
    )
    file(STRINGS RunSwig_CSharp/${_tmpf} _lines NEWLINE_CONSUME)
    if(${_lines} MATCHES "${_target_type} ([^\n]*)")
	set(SPR_PROJS ${CMAKE_MATCH_1})
    endif()
    string(REPLACE "," ";" SPR_PROJS ${SPR_PROJS})
    list(PREPEND SPR_PROJS Base)
    list(APPEND SPR_PROJS ${_target_type})
endif()
message(STATUS "SPR_PROJS: ${SPR_PROJS}")

# ------------------------------------------------------------------------------
#  Add Springhead subprojects.
#
set(DEP_Base)
set(DEP_Collision Foundation)
set(DEP_Creature Foundation Framework Physics)
set(DEP_FileIO Foundation)
set(DEP_Foundation Base)
set(DEP_Framework FileIO Foundation Graphics Physics)
set(DEP_Graphics Foundation)
set(DEP_HumanInterface Foundation)
set(DEP_Physics Collision Foundation)
add_subdirectory(${SPR_SRC_DIR}/RunSwig ${SPR_BLD_DIR}/RunSwig)
foreach(proj ${SPR_PROJS})
    add_subdirectory(${SPR_SRC_DIR}/${proj} ${SPR_BLD_DIR}/${proj})
    set(DEP_LIST RunSwig)
    foreach(dep IN ITEMS ${DEP_${proj}})
	list(APPEND DEP_LIST ${dep})
    endforeach()
    add_dependencies(${proj} ${DEP_LIST})
    #message("   dependencies: ${proj} => ${DEP_LIST}")
endforeach()
if(EXISTS "setup.conf")
    message("** setup.conf exists")
else()
    if(Windows)
	set(INCLUDED_BY_SPRLIB true)
    elseif(unix)
	set(SPR_SWIG_DIR "${TOPDIR}/core/bin/src/swig")
	add_custom_target(genswig ALL
	    COMMAND make -s -f Makefile.autogen
	    WORKING_DIRECTORY ${SPR_SWIG_DIR}
	    COMMENT [[  checking/generating swig binary... ]]
	)
	add_dependencies(RunSwig genswig)
    endif()
endif()

# ------------------------------------------------------------------------------
#  Main target (dummy!).
#
set(Target ${ProjectName})			# 仮のターゲットの定義
set(DUMMY_OUT_DIR ${CMAKE_BINARY_DIR}/dummy)	# 仮の生成物の置き場所
file(MAKE_DIRECTORY ${DUMMY_OUT_DIR})

set(DummySrcFile ${DUMMY_OUT_DIR}/_cmake_dummy.cpp) # ダミーソース
file(TOUCH ${DummySrcFile})
set(SRCS ${DummySrcFile})
add_library(${Target} ${LIBTYPE} ${SRCS})	# 仮のライブラリの生成
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT SprCsSample)
#
#  Following variable need to be set something.
set(CMAKE_EXE_LINKER_FLAGS_TRACE /Dummy)

# ------------------------------------------------------------------------------
#  Define depending projects.
#
add_subdirectory(RunSwig_CSharp RunSwig_CSharp)
add_subdirectory(SprExport SprExport)
add_subdirectory(SprImport SprImport)
add_subdirectory(SprCSharp SprCSharp)
add_subdirectory(Install Install)

add_dependencies(${Target} SprCSharp)
add_dependencies(SprCSharp SprExport SprImport)
add_dependencies(SprExport RunSwig_CSharp)
add_dependencies(SprImport RunSwig_CSharp)
add_dependencies(RunSwig_CSharp TargetManager ${SPR_PROJS})
add_dependencies(TargetManager NameManager)
add_dependencies(Install SprExport SprImport SprCSharp)

add_subdirectory(SprCsSample SprCsSample)
add_dependencies(SprCsSample Install)
add_dependencies(SprCsSample SprExport SprImport SprCSharp Install)

# ------------------------------------------------------------------------------
#  Clean up.
#
unset(TARGET_TYPE CACHE)

# end: CMakeLists.txt
