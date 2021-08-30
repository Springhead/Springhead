# CMakeLists.txt for SprCsSample
#
set(SPR_CS_SAMPLE_VERSION 1.1.0)	# 2021/07/08 バージョン管理開始

# ------------------------------------------------------------------------------
#  Target definition.
#
set(Target SprCsSample)
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Directories.
#
#set(SPR_TOP_DIR  "${TOPDIR}")
#set(SPR_CORE_DIR "${TOPDIR}/core")
#set(SPR_BIN_DIR  "${TOPDIR}/generated/bin")
set(SPR_CSHARP_DIR "${TOPDIR}/core/src/SprCSharp")
set(SPR_TARGETMGR_DIR "${SPR_CSHARP_DIR}/TargetManager")
#get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
#get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
#get_filename_component(SPR_COREDIR ${SPR_CORE_DIR} ABSOLUTE)
#get_filename_component(SPR_BIN_DIR ${SPR_BIN_DIR} ABSOLUTE)
get_filename_component(SPR_CSHARP_DIR ${SPR_CSHARP_DIR} ABSOLUTE)
get_filename_component(SPR_TARGETMGR_DIR ${SPR_TARGETMGR_DIR} ABSOLUTE)
#message(STATUS "SPR_TOP_DIR:       [${SPR_TOP_DIR}]")
#message(STATUS "SPR_CORE_DIR:      [${SPR_CORE_DIR}]")
#message(STATUS "SPR_BIN_DIR:       [${SPR_BIN_DIR}]")
message(STATUS "SPR_CSHARP_DIR:    [${SPR_CSHARP_DIR}]")
message(STATUS "SPR_TARGETMGR_DIR: [${SPR_TARGETMGR_DIR}]")
message(STATUS "TARGET_TYPE: ${TARGET_TYPE}")

# ------------------------------------------------------------------------------
#  Main target.
#
if("${TARGET_TYPE}" STREQUAL "ALL")
    set(SRCS Program.cs)
elseif("${TARGET_TYPE}" STREQUAL "PH")
    set(SRCS ProgramPH.cs)
endif()
message(STATUS "SRCS: ${SRCS}")
add_executable(${Target} ${SRCS})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${Target})

# ------------------------------------------------------------------------------
#  Compiler options.
#	Cmake:	Sample program の中で DLL path の制御のために使用する.
#
add_compile_definitions("CMake")

# ------------------------------------------------------------------------------
#  Clean up.
#
unset(TARGET_TYPE CACHE)

# end: CMakeLists.txt
