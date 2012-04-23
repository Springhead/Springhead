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
descCapsule.radius = Vec2f(0.2, 0.2)
descCapsule.length = 2

descSphere  = CDSphereDesc()
descSphere.radius  = 0.1

shapePose = Posed()
shapePose.setPos(Vec3d(1,0,0))
shapePose.setOri(Quaterniond.Rot(90*rad, "y"))

## -----

so0 = phScene.CreateSolid(descSolid)
so0.SetFramePosition(Vec3d(0,2,0))
so0.SetDynamical(False)

so1 = phScene.CreateSolid(descSolid)

so2 = phScene.CreateSolid(descSolid)
so2.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so2.SetShapePose(0, shapePose)

so3 = phScene.CreateSolid(descSolid)
so3.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so3.SetShapePose(0, shapePose)

so4 = phScene.CreateSolid(descSolid)
so4.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so4.SetShapePose(0, shapePose)

# Pointer
soPointer = phScene.CreateSolid(descSolid)
soPointer.SetFramePosition(Vec3d(-10,0,0))
soPointer.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), descSphere))
soPointer.SetDynamical(False)

## ----- ----- ----- ----- -----

descSlider = PHSliderJointDesc()
descSlider.poseSocket = Posed(0.707,0,0.707,0, 0,0,0)
descSlider.posePlug   = Posed(0.707,0,0.707,0, 0,0,0)
descSlider.spring =  1.0
descSlider.damper =  0.01
descSlider.targetPosition = 0
# descSlider.bConstraintRollZ = False;

descLimit = PH1DJointLimitDesc()
descLimit.range = Vec2d(-3, +2)

jo0 = phScene.CreateJoint(so0, so1, PHSliderJoint.GetIfInfoStatic(), descSlider)
jo0.CreateLimit(descLimit)

## ----- ----- ----- ----- -----

descJoint = PHHingeJointDesc()

descJoint.poseSocket = Posed(1,0,0,0, 0,0,0)
descJoint.posePlug   = Posed(1,0,0,0, 2,0,0)
descJoint.spring = 10.0
descJoint.damper =  1.0
descJoint.targetPosition = (1.7*3.14/4.0)

descLimit = PH1DJointLimitDesc()
descLimit.spring = 100000.0
descLimit.damper =  10000.0
descLimit.range  = Vec2d((1*3.14/4.0), (2*3.14/4.0))

jo1 = phScene.CreateJoint(so1, so2, PHHingeJoint.GetIfInfoStatic(), descJoint)
jo1.CreateLimit(descLimit)

## ----- ----- ----- ----- -----

descJoint = PHBallJointDesc()

descJoint.poseSocket = Posed(0.707,0,0.707,0, 0,0,0)
descJoint.posePlug   = Posed(0.707,0,0.707,0, 2,0,0)
descJoint.spring =  10.0
descJoint.damper =   1.0
descJoint.targetPosition = Quaterniond.Rot(3.14/4.0,'x')

descLimit = PHBallJointSplineLimitDesc()

jo2 = phScene.CreateJoint(so2, so3, PHBallJoint.GetIfInfoStatic(), descJoint)
limit0 = jo2.CreateLimit(PHBallJointSplineLimit.GetIfInfoStatic(), descLimit)

limit0.AddNode( 90*rad,   0*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 87*rad,  48*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 65*rad, 132*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode(173*rad, 226*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 55*rad, 232*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 62*rad, 247*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode(147*rad, 291*rad, 10*rad, 50*rad, -85*rad, 0*rad)
limit0.AddNode( 90*rad, 360*rad, 10*rad, 50*rad, -85*rad, 0*rad)

## ----- ----- ----- ----- -----

descJoint = PHBallJointDesc()

descJoint.poseSocket = Posed(0.707,0,0.707,0, 0,0,0)
descJoint.posePlug   = Posed(0.707,0,0.707,0, 2,0,0)
descJoint.spring =  10.0
descJoint.damper =   1.0
descJoint.targetPosition = Quaterniond.Rot(3.14/4.0,'x')

descLimit = PHBallJointConeLimitDesc()
descLimit.limitSwing = Vec2d(0, 3.14/4.0)

jo3 = phScene.CreateJoint(so3, so4, PHBallJoint.GetIfInfoStatic(), descJoint)
jo3.CreateLimit(PHBallJointConeLimit.GetIfInfoStatic(), descLimit)

## ----- ----- ----- ----- -----

descSpring = PHSpringDesc()
descSpring.spring = Vec3d( 10.0, 10.0, 10.0)
descSpring.damper = Vec3d(  1.0,  1.0,  1.0)

joPointer = phScene.CreateJoint(so4, soPointer, PHSpring.GetIfInfoStatic(), descSpring)

## ----- ----- ----- ----- -----

root  = phScene.CreateRootNode(so0, PHRootNodeDesc())
node0 = phScene.CreateTreeNode(root,  so1, PHTreeNodeDesc())
node1 = phScene.CreateTreeNode(node0, so2, PHTreeNodeDesc())
node2 = phScene.CreateTreeNode(node1, so3, PHTreeNodeDesc())
node3 = phScene.CreateTreeNode(node2, so4, PHTreeNodeDesc())
root.Enable(True)

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
