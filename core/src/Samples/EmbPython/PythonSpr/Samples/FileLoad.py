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
if not fileSpr.Load(objs, "pitagora_new.blend.spr"):
	print("File Load Failure")
else:
	print("File Successfully Loaded")

app.GetCurrentWin().SetScene(fwSdk.GetScene(1))

