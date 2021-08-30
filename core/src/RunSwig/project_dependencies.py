#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  プロジェクトとその依存関係の定義
#	依存するプロジェクトが複数あるときはカンマで区切る.
#	依存するプロジェクトがないときは 'None' とする
#
# -----------------------------------------------------------------------------
#  VERSION:
#     Ver 1.0	 2021/03/22 F.Kanehori	do_swigall.projs を python script 化.
#     Ver 1.1	 2021/07/26 F.Kanehori	依存関係見直し.
# ==============================================================================
proj_depts = {
	'Collision':	    'Foundation',
	'Creature':	    'Physics,Foundation',
	'FileIO':	    'Foundation',
	'Foundation':	    'None',
	'Framework':	    'FileIO,Physics,Foundation,Graphics,HumanInterface',
	'Graphics':	    'Foundation',
	'HumanInterface':   'Foundation',
	'Physics':	    'Collision,Foundation'
}
projs = proj_depts.keys()

#end: project_dependencies.py
