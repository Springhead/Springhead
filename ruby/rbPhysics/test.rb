require "RBPhysics"
require "../rbGraphics/RBGraphics"

include RBPhysics
include RBGraphics

phsdk = PHSdkIf.CreateSdk()
grsdk = GRSdkIf.CreateSdk()

sceneDesc = PHSceneDesc.new
scene = phsdk.CreateScene(sceneDesc)

solidDesc = PHSolidDesc.new
solid = scene.CreateSolid(solidDesc)
scene.SetGravity([0.0, 0.0, -9.8])



for i in 1..10
	scene.Step()
	pos = solid.GetPose()
	print pos.join(', ') + "\n";
end
