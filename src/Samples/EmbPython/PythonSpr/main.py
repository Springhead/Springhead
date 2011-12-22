from Spr import *

import time
def Steps(n):
	for i in range(0,n):
		phScene.Step()
		time.sleep(0.02)

hiTrackball.SetPosition(Vec3f(3, 5, 10))

## -----

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

descSolid = PHSolidDesc()
so1 = phScene.CreateSolid(descSolid)
descSolid.pose = Posed(1,0,0,0, 0,0,-2)
so2 = phScene.CreateSolid(descSolid)

descBox = CDBoxDesc()
so1.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
so2.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))

so1.SetDynamical(False)
phScene.SetContactMode(so1, so2, 0)
so2.SetAngularVelocity(Vec3d(0,0,0))
so2.SetVelocity(Vec3d(0,0,0))

descJoint = PHBallJointDesc()
descJoint.poseSocket = Posed(1,0,0,0, 0,0,0)
descJoint.posePlug = Posed(1,0,0,0, 0,0,2)
descJoint.spring = 10.0
descJoint.damper = 1.0
descJoint.targetPosition = Quaterniond().Rot(1.0, Vec3d(1,0,0))
jo = phScene.CreateJoint(so1, so2, PHBallJoint.GetIfInfoStatic(), descJoint)
for i in range(0,500):
	phScene.Step()

descSolid.pose = Posed(1,0,0,0, 0,2,-2)
box = phScene.CreateSolid(descSolid)
descSolid.pose = Posed(1,0,0,0, 0,10,-2)
sphere = phScene.CreateSolid(descSolid)
box.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
descSp = CDSphereDesc()
sphere.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), descSp))

o = ObjectStates().Create()
o.SaveState(phScene)
p = ObjectStates().Create()
p.SaveState(phScene)
