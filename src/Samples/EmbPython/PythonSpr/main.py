from Spr import *

## -----

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

descSolid = PHSolidDesc()
so1 = phScene.CreateSolid(descSolid)
so2 = phScene.CreateSolid(descSolid)

descBox = CDBoxDesc()
# so1.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
# so2.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))

so1.SetDynamical(False)
# phScene.SetContactMode(so1, so2, 0)
# so2.SetAngularVelocity(Vec3d(0,0,0))
# so2.SetVelocity(Vec3d(0,0,0))

descJoint = PHBallJointDesc()
descJoint.spring = 10.0
descJoint.damper =  0.1
jo  = phScene.CreateJoint(so1, so2, PHBallJoint.GetIfInfoStatic(), descJoint)


