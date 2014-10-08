import sys

from   Spr import *

print("Started")

fwSdk = app.GetSdk()

objs  = ObjectIfs()
objs.Push(fwSdk.GetGRSdk())
objs.Push(fwSdk.GetPHSdk())
objs.Push(fwSdk)

fiSdk   = FISdk().CreateSdk()
fileSpr = fiSdk.CreateFileSpr()
if not fileSpr.Load(objs, "test.spr"):
	print("File Load Failure")
else:
	print("File Successfully Loaded")

app.GetCurrentWin().SetScene(fwSdk.GetScene(1))

phScene = fwSdk.GetScene(1).GetPHScene()

fwSdk.GetScene(1).EnableRenderIK(True)

# phScene.SetContactMode(0)

phScene.GetIKEngine().SetMaxVelocity(20)
phScene.GetIKEngine().SetMaxAngularVelocity(1000 * 3.14 / 180.0)
phScene.GetIKEngine().SetRegularizeParam(0.5)

# phScene.FindObject("so___AppleR2").SetDynamical(False)

# phScene.GetIKEngine().Enable(True)

# phScene.FindObject("ike__RightHand").SetTargetPosition(phScene.FindObject("so___AppleR2").GetPose().getPos())
# phScene.FindObject("ike__RightHand").Enable(True)
# phScene.FindObject("ike__RightHand").EnablePositionControl(True)

# phScene.FindObject("ike__LeftHand").SetTargetPosition(phScene.FindObject("so___AppleR2").GetPose().getPos())
# phScene.FindObject("ike__LeftHand").Enable(True)
# phScene.FindObject("ike__LeftHand").EnablePositionControl(True)

# for j in phScene.GetJoints():
# 	j.SetDamper(j.GetDamper() *   10.0)
# 	j.SetSpring(j.GetSpring() * 1000.0)

# for j in phScene.GetIKActuators():
# 	j.SetPullbackRate(0.5)

# st = ObjectStates().Create()
# st.SaveState(phScene)


