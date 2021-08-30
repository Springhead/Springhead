# CMakeLists.txt for TargetManager
#	変数 TARGET_TYPE が定義されていること (値は 'ALL' または 'PH').
#
set(NTARGETMANAGER_VERSION 1.1.0)	# 2021/07/08 バージョン管理開始

# ------------------------------------------------------------------------------
#  Target definition.
#
set(Target TargetManager)
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Main target.
#
set(Script ${CMAKE_CURRENT_SOURCE_DIR}/TargetManager.bat)
set(TargetFile ${CMAKE_CURRENT_SOURCE_DIR}/target.last)
if(NOT TARGET_TYPE)
    message("Error: TARGET_TYPE not defined")
    message("** force TARGET_TYPE be 'ALL'")
    set(TARGET_TYPE ALL)
endif()
add_custom_target(${Target} ALL
    COMMAND echo ${Script} ${TARGET_TYPE}
    COMMAND ${Script} ${TARGET_TYPE}
    BYPRODUCTS ${TargetFile}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT [[  checking/modifying target.last ... ]]
)

# end: CMakeLists.txt
