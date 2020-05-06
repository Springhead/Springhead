from Spr import *

import time
import datetime
import threading

INF = 3.4e+38
rad = 3.14/180.0;

hiTrackball.SetPosition(Vec3f(0, 0, 50))

initPose = Posef()
initPose.setPos(Vec3f(-10,0,0))
spaceNavigator0.SetPose(initPose)

fwScene.EnableRenderLimit(True)

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

def AddJoint(so1, so2):
	descJoint = PHHingeJointDesc()
	descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0)
	descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0)
	jo = phScene.CreateJoint(so1, so2, PHHingeJoint.GetIfInfoStatic(), descJoint)
	return jo

# Pointer-base
soPB = phScene.CreateSolid(PHSolidDesc())
soPB.SetDynamical(False)

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
	def __init__(self, soPointer):
		self.soPointer = soPointer
		threading.Thread.__init__(self)

	def run(self):
		while True:
			p = spaceNavigator0.GetPose().getPos()
			self.soPointer.SetFramePosition(Vec3d(p.x, p.y, p.z))

spaceNav = SpaceNavigatorThread(soPB)
spaceNav.start()

