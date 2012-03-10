from Spr import *

import time
import datetime
import threading

INF = 3.4e+38
rad = 3.14/180.0;

def to_quat(vec4):
	rv=Quaternionf()
	rv.w = vec4.w
	rv.x = vec4.x
	rv.y = vec4.y
	rv.z = vec4.z
	return rv

hiTrackball.SetPosition(Vec3f(50, 0, 0))

initPose = Posef()
initPose.setPos(Vec3f(-10,0,0))
spaceNavigator0.SetPose(initPose)

initPose.setPos(Vec3f( 50,0,0))
spaceNavigator1.SetPose(initPose)

fwScene.EnableRenderLimit(True)
fwScene.EnableRenderGrid(False,True,False)

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

## ----- ----- ----- ----- -----

def AddSolid():
	descSolid = PHSolidDesc()
	descBox = CDBoxDesc()
	descBox.boxsize = Vec3d(1,0.5,1)
	so = phScene.CreateSolid(descSolid)
	so.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
	return so

def AddJoint(so1, so2, ikaParent=None):
	descJoint = PHHingeJointDesc()
	descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0)
	descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0)
	descJoint.damper = 100.0
	jo = phScene.CreateJoint(so1, so2, PHHingeJoint.GetIfInfoStatic(), descJoint)
	descIKA = PHIKHingeActuatorDesc()
	descIKA.velocityGain = 10.0
	ika = phScene.CreateIKActuator(PHIKHingeActuator.GetIfInfoStatic(), descIKA)
	ika.AddChildObject(jo)
	if not ikaParent==None:
		ikaParent.AddChildObject(ika)
	return ika

so0 = AddSolid(); so0.SetDynamical(False)
so1 = AddSolid()
so2 = AddSolid()
so3 = AddSolid()
so4 = AddSolid()
so5 = AddSolid()
so6 = AddSolid()

ika0 = AddJoint(so0, so1)
ika1 = AddJoint(so1, so2, ika0)
ika2 = AddJoint(so2, so3, ika1)
ika3 = AddJoint(so3, so4, ika2)
ika4 = AddJoint(so2, so5, ika1)
ika5 = AddJoint(so5, so6, ika4)

descIKE = PHIKEndEffectorDesc()
ike0 = phScene.CreateIKEndEffector(PHIKEndEffector.GetIfInfoStatic(), descIKE); ike0.AddChildObject(so4)
ike1 = phScene.CreateIKEndEffector(PHIKEndEffector.GetIfInfoStatic(), descIKE); ike1.AddChildObject(so6)

# ike0 = phScene.CreateIKEndEffector(PHIKEndEffector.GetIfInfoStatic(), descIKE); ike0.AddChildObject(so2)
# ika1.AddChildObject(ike0)

# ike0 = phScene.CreateIKEndEffector(PHIKEndEffector.GetIfInfoStatic(), descIKE); ike0.AddChildObject(so1)
# ika0.AddChildObject(ike0)

ika3.AddChildObject(ike0)
ika5.AddChildObject(ike1)

ike0.SetTargetPosition(Vec3d(2,5,0))
ike0.Enable(True)

ike1.SetTargetPosition(Vec3d(4,5,0))
ike1.Enable(True)

for i in range(0,10):
	phScene.Step()

phScene.GetIKEngine().Enable(True)
phScene.Step()

phScene.SetContactMode(0)



# Pointer-base
soPB = phScene.CreateSolid(PHSolidDesc())
soPB.SetDynamical(False)

soPB1 = phScene.CreateSolid(PHSolidDesc())
soPB1.SetDynamical(False)

# Pointer
soP = phScene.CreateSolid(PHSolidDesc())
soP.SetFramePosition(Vec3d(-10,0,0))
soP.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), CDSphereDesc()))

# Pointer Spring
descSpring = PHSpringDesc()
descSpring.spring = Vec3d(1,1,1) * 200.0
descSpring.damper = Vec3d(1,1,1) *  10.0
descSpring.springOri = 200.0
descSpring.damperOri =  10.0

# Pointer
joP = phScene.CreateJoint(soPB, soP, PHSpring.GetIfInfoStatic(), descSpring)

class SpaceNavigatorThread(threading.Thread):
	def __init__(self, soPointer, spcNav):
		self.soPointer = soPointer
		self.spcNav    = spcNav
		threading.Thread.__init__(self)

	def run(self):
		self.spcNav.SetMaxVelocity(3.0)
		self.spcNav.SetMaxAngularVelocity(30*rad)
		while True:
			dV   = self.spcNav.GetVelocity() * phScene.GetTimeStep()

			if dV.norm() > 1e-5:
				dV += (dV.unit() * (dV.norm() ** 3))

			ori0 = hiTrackball.GetOrientation()
			pos0 = self.soPointer.GetPose().getPos()
			pos1 = ori0*dV + pos0
			self.soPointer.SetFramePosition(pos1)
			time.sleep(0.01)

spaceNav = SpaceNavigatorThread(soPB, spaceNavigator1)
spaceNav.start()


class SpaceNavigatorViewThread(threading.Thread):
	def __init__(self, soPointer, spcNav0, spcNav1):
		self.soPointer = soPointer
		self.spcNav0   = spcNav0
		self.spcNav1   = spcNav1
		threading.Thread.__init__(self)

	def run(self):
		self.spcNav0.SetMaxVelocity(2.0)
		self.spcNav0.SetMaxAngularVelocity(30*rad)
		while True:
			dV   = self.spcNav0.GetVelocity()        * phScene.GetTimeStep()
			dQ   = self.spcNav1.GetAngularVelocity() * phScene.GetTimeStep()

			ori0 = hiTrackball.GetOrientation()
			pos0 = hiTrackball.GetPosition()

			pos1 =                 ori0*dV  + pos0
			ori1 = Quaternionf.Rot(ori0*dQ) * ori0

			hiTrackball.SetPosition(pos1)
			hiTrackball.SetOrientation(ori1)

			# print(pos1,ori1)
			time.sleep(0.01)

spaceNav1 = SpaceNavigatorViewThread(soPB1, spaceNavigator0, spaceNavigator0)
spaceNav1.start()

phScene.SetContactMode(0)
