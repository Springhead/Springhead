#!/bin/bash
# ============================================================================
#  SYNOPSIS
#	setup [-f (--force) ]
#	  -f (--force)	セットアップファイルを無条件に再作成する。
#	  -c (--check)	セットアップファイルの整合性を検査する。
#
#  DESCRIPTION
#	setup.py を実行するためのアダプタ。
#	python が見つからないときはメッセージを表示して処理を中止する。
# ----------------------------------------------------------------------------
#  VERSION
#	Ver 1.0  2020/11/04 F.Kanehori	初版
# ============================================================================
PROG=`basename $0 .sh`
DEBUG=0

which python > /dev/null 2>&1
if [ $? -ne 0 ]; then
	echo "$PROG: Can't find python."
	echo "$PROG: We need python to build Springhead Library."
	echo "$PROG: aborted."
	exit 1
fi

echo "found python (Version `python --version | sed -e 's/Python //i'`)"
python_path=`which python`


if [ $DEBUG -ne 0 ]; then
	echo "python setup.py $* $python_path"
fi
python setup.py $* $python_path

exit 0
# end: setup.sh
