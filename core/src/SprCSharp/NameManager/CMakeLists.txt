# CMakeLists.txt for NameManager
#
set(NAME_MANAGER_VERSION 1.1.0)		# 2021/07/08 バージョン管理開始

# ------------------------------------------------------------------------------
#  Target definition.
#
set(Target NameManager)
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Main target.
#
set(Script ${CMAKE_CURRENT_SOURCE_DIR}/NameManager.py)
set(InitFile ${CMAKE_CURRENT_SOURCE_DIR}/NameManager.ini)
set(OutFile ${CMAKE_CURRENT_SOURCE_DIR}/NameManager.bat)
file(TO_NATIVE_PATH ${Script} Script)
file(TO_NATIVE_PATH ${InitFile} InitFile)
file(TO_NATIVE_PATH ${OutFile} OutFile)

set(PYTHON "${CMAKE_SOURCE_DIR}/../../../buildtool/win32/python")
get_filename_component(PYTHON ${PYTHON} ABSOLUTE)
file(TO_NATIVE_PATH ${PYTHON} PYTHON)

add_custom_target(${Target}
    BYPRODUCTS ${OutFile}
    COMMAND echo ${PYTHON} ${Script} -i ${InitFile} -o ${OutFile}
    COMMAND ${PYTHON} ${Script} -i ${InitFile} -o ${OutFile}
    DEPENDS ${OutFile}
)

# end: CMakeLists.txt
