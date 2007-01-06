require "rbPhysics"
include RbPhysics

sdk = PHSdkIf.CreateSdk()
sceneDesc = PHSceneDesc.new
scene = sdk.CreateScene(sceneDesc)

solidDesc = PHSolidDesc.new
solid = scene.CreateSolid(solidDesc)
scene.SetGravity([0.0, 0.0, -9.8])
for i in 1..10
	scene.Step()
	pos = solid.GetPose()
	print pos.join(', ') + "\n";
end
