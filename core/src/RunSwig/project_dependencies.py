#!/usr/local/bin/python
# -*- coding: utf-8 -*-
# ==============================================================================
#  プロジェクトとその依存関係の定義
#	依存するプロジェクトが複数あるときはカンマで区切る.
#	依存するプロジェクトがないときは 'None' とする
#
# ==============================================================================
#  Version:
#     Ver 1.00	 2021/03/22 F.Kanehori	do_swigall.projs を python script 化.
# ==============================================================================
proj_depts = {
	'Collision':	    'Foundation',
	'Creature':	    'Foundation',
	'FileIO':	    'Foundation',
	'Foundation':	    'None',
	'Framework':	    'Physics,Foundation,HumanInterface',
	'Graphics':	    'Foundation',
	'HumanInterface':   'Foundation',
	'Physics':	    'Collision,Foundation'
}
projs = proj_depts.keys()

#end: project_dependencies.py
