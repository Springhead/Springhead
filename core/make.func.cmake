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

# ------------------------------------------------------------------------------
#  ファイルをincludeする。
#  ${option}が定義されているならば、それが指すファイルをincludeする。
#  さもなければ、${file_path}, ${default_file}の順にファイルを探し、
#  最初に見つかったファイルをincludeする。
#  
#  macro include_file()
#	option_var	オプション変数名
#			  -D <option_var>=<file-absolute-path>
#	file_path	ディレクトリ名 (絶対パス)
#	default_file	デフォルトディレクトリ名 (絶対パス)
#	file_name	includeするファイル名	
#
macro(include_file option_var file_path default_file)
    if(DEFINED ${option_var})
	if(EXISTS ${${option_var}})
	    message(STATUS "option: ${option_var}=\"${${option_var}}\"")
	    if(IS_DIRECTORY ${${option_var}})
		message("Error: \"${${option_var}}\" is directory")
		return()
	    endif()
	    message(STATUS "including \"${${option_var}}\"")
	    include(${${option_var}})
	else()
	    message("Error: File \"${${option_var}}\" not exists!")
	    return()
	endif()
    else()
	if(EXISTS ${file_path})
	    message(STATUS "including \"${file_path}\"")
	    include(${file_path})
	elseif(EXISTS ${default_file})
	    message(STATUS "including \"${default_file}\"")
	    include(${default_file})
	else()
	    message("Error: File \"${file_name}\" not found")
	    return()
	endif()
    endif()
endmacro()

# end: make.func.cmake
