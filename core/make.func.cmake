#  make.func.cmake

# ------------------------------------------------------------------------------
#  eval()
#	code		実行する(CMakeの)コード
#
function(eval code)
    set(path ${CMAKE_CURRENT_LIST_DIR}/.temp.cmake)
    file(WRITE ${path} "${code}")
    include(${path})
    file(REMOVE ${path})
endfunction()

# ------------------------------------------------------------------------------
#  環境変数が設定されていたらその値を、さもなければデフォルト値を返す。
#  function env()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#	default_value	デフォルト値
#
function(envval return_var environ_name default_value)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} ${default_value} PARENT_SCOPE)
    else()
	set(${return_var} ${tmp} PARENT_SCOPE)
    endif()
endfunction()

# ------------------------------------------------------------------------------
#  環境変数の値に従って"-I"オプションを設定する。
#  function add_inc()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#
function(add_inc return_var environ_name)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} "")
    else()
	set(${return_var} -I${tmp} PARENT_SCOPE)
    endif()
endfunction()

# ------------------------------------------------------------------------------
#  環境変数の値に従って"-L"オプションを設定する。
#  function add_inc()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#
function(add_lib return_var environ_name)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} "")
    else()
	set(${return_var} -L${tmp} PARENT_SCOPE)
    endif()
endfunction()

# end: make.func.cmake
