from Spr import *

import time
import datetime
import threading

INF = 3.4e+38
rad = 3.14/180.0;

hiTrackball.SetPosition(Vec3f(3, 5, 10))

fwScene.EnableRenderLimit(True)
fwScene.EnableRenderAxis(False, False, False)
fwScene.EnableRenderForce(False, False)

## -----

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

## ----- ----- ----- ----- -----

descSolid   = PHSolidDesc()

descCapsule = CDRoundConeDesc()
descCapsule.radius = Vec2f(0.1, 0.1)
descCapsule.length = 2

descSphere  = CDSphereDesc()
descSphere.radius  = 0.1

shapePose = Posed()

## -----

so0 = phScene.CreateSolid(descSolid)
so0.SetFramePosition(Vec3d(0,2,0))
so0.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
shapePose.setPos(Vec3d(0,0,1))
so0.SetShapePose(0, shapePose)
so0.SetDynamical(False)

so1 = phScene.CreateSolid(descSolid)
so1.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
shapePose.setPos(Vec3d(0,0,-1))
so1.SetShapePose(0, shapePose)

# Pointer
soPointer = phScene.CreateSolid(descSolid)
soPointer.SetFramePosition(Vec3d(5,0,0))
soPointer.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), descSphere))
soPointer.SetDynamical(False)

## ----- ----- ----- ----- -----

descJoint = PHBallJointDesc()

descJoint.poseSocket = Posed(1,0,0,0, 0,0, 2)
descJoint.posePlug   = Posed(1,0,0,0, 0,0,-2)
descJoint.spring =  10.0
descJoint.damper =   1.0
descJoint.targetPosition = Quaterniond.Rot(3.14/4.0,'x')

descLimit = PHBallJointSplineLimitDesc()

jo0 = phScene.CreateJoint(so0, so1, PHBallJoint.GetIfInfoStatic(), descJoint)
limit0 = jo0.CreateLimit(PHBallJointSplineLimit.GetIfInfoStatic(), descLimit)

limit0.AddNode( 90*rad,   0*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 87*rad,  48*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 65*rad, 132*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode(173*rad, 226*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 55*rad, 232*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 62*rad, 247*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode(147*rad, 291*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 90*rad, 360*rad, 10*rad, 50*rad, -85*rad, 0*rad)

## ----- ----- ----- ----- -----

descSpring = PHSpringDesc()
descSpring.spring = Vec3d( 10.0, 10.0, 10.0)
descSpring.damper = Vec3d(  1.0,  1.0,  1.0)

joPointer = phScene.CreateJoint(so1, soPointer, PHSpring.GetIfInfoStatic(), descSpring)

## ----- ----- ----- ----- -----

phScene.SetContactMode(0)

st = ObjectStates().Create()
st.SaveState(phScene)

class SpaceNavigatorThread(threading.Thread):
	def __init__(self, soPointer):
		self.soPointer = soPointer
		threading.Thread.__init__(self)

	def run(self):
		while True:
			p = spaceNavigator0.GetPose().getPos()
			self.soPointer.SetFramePosition(Vec3d(p.x, p.y, p.z))

spaceNav = SpaceNavigatorThread(soPointer)
spaceNav.start()
