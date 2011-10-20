#!/usr/bin/env python-
# -*- coding: utf-8 -*

##from Test import * 

import sys
import time
import threading
import math
import random

from Spr import *

print("def Pick(PHSolidIf,Vec3d)")
def Pick(so,vec):
	so.SetFramePosition( vec )
	so.SetVelocity( Vec3d(0,0,0) ) 

print("def AddBox() \n\tReturn PHSolidIf")
def AddBox(_mesh=None):
	tmp = scene.CreateSolid( desc )
	if _mesh:
		tmp.AddShape( _mesh )
	else:
		tmp.AddShape( mesh )
	Pick( tmp , Vec3d( 0,20,0 ) )
	return tmp

#デフォルトでは、現在のシンボルテーブルで、直接test.pyを実行する つまり、変数とか関数とか上書きされる
print("def Reload()")
def Reload(g = globals() , l = locals()):
	execfile('test.py',g,l)


def Shower(box = None):
	if not box:
		box = [None] * 20
		
	start = time.time()
	num = len(box)
	
	for i in range(num):
		if box[i] == None :
			box[i] = AddBox()

	# 10sec
	span = 1
	while time.time() < start + span:
		for i in range(num):
			spent = time.time() - start
			x = ( spent / span * 14  + 4 ) * math.cos( 2 * math.pi * i / num )
			z = ( spent / span * 14  + 4 ) * math.sin( 2 * math.pi * i / num )
			Pick( box[i] , Vec3d(x,27.0,z) ) 
			box[i].SetVelocity(Vec3d(x*1.5,10,z*1.5))
			time.sleep(0.05)
	return box 


def MoveRnd(so):
	start = time.time()
	span = 5	#5sec
	
	while time.time() < start + span:
		v = Vec3d((random.random()-0.5)*15,(random.random()-0.5)*25,(random.random()-0.5)*15)
		so.SetVelocity(so.GetVelocity()+v)
		time.sleep(0.5)


class PD( threading.Thread):
	def __init__(self,so,pos=Vec3d(5,15,0),k=10,d=10):
		self.so = so
		self.pos= pos
		self.k = k
		self.d = d
		threading.Thread.__init__(self)

	def run(self):
		dt = 0.1

		while(True):
			self.so.AddForce( (self.pos - self.so.GetFramePosition())*self.k + (Vec3d(0,0,0) - self.so.GetVelocity())*self.d ) 
			time.sleep(dt)
		
def Chain(n):
	tList = []
	oldx = None
	oldy = None
	oldz = None
	for i in range(n):
		t = PD(AddBox())
		tList.append(t)
		
		x = oldx     if oldx else (random.random()-0.5)*40
		y = oldy - 7 if oldy else random.random()*40+ 8 * n
		z = oldz     if oldz else (random.random()-0.5)*40

		oldx = x
		oldy = y
		oldz = z

		t.pos = Vec3d( x, y, z)
		t.start()

	return tList

if __name__ == "__main__":
	print("v1 = Vec3d() v2 = Vec3d()")
	
	v1 = Vec3d(1.0,2.0,3.0)
	v2 = Vec3d(4.0,5.0,6.0)
	
	print( v1 , v2 )
	
	v3 = v2 + v1
	print( "v3" , "=" ,"v2" , "+" , "v1" )
	print( "v3 =",v3 )
	
	print( "v3" , "+=", "v2"  )
	print( "v3 =",v3 )
	
	mat1 = Matrix3d(1,2,3,2,3,4,3,4,5)
	print( "mat1 =" , mat1  )
	
	v3 = Vec3d(1,2,3)
	
	print( "Shower(box)!!" )
	
	#soList = scene.GetSolids()
	
	box = [None] * 20



	myMeshDesc = CDConvexMeshDesc()
	myVertices = []
	
	mesh.GetDesc(myMeshDesc)
	
	for i in range(len(myMeshDesc.vertices)):
		myVertices.append(myMeshDesc.vertices[i] * i * 0.5)
	
	myMeshDesc.vertices = myVertices
	mesh.SetDesc(myMeshDesc)
	

	