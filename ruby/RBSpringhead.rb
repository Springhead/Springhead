require 'Springhead'

module Springhead

$phSdk
$phScene

def CreateSdk()
	$phSdk = PHSdkIf.CreateSdk();
end

class Solids
	include Enumerable
	def initialize(scene)
		@scene = scene;
	end
	def each
		@scene.intf.GetSolids.each {|s| yield(Solid.new(s))}
	end
	def size
		@scene.intf.NSolids()
	end
	def add
		Solid.new(@scene.intf.CreateSolid(PHSolidDesc.new))
	end
	def delete(solid)
		@scene.intf.DelChildObject(solid.intf)
	end
end

class Scene
	attr_reader :intf
	def initialize()
		@intf = $phSdk.CreateScene(PHSceneDesc.new);
	end
	def solids
		Solids.new(self);
	end
end

class Solid
	attr_reader :intf
	def initialize(i)
		#@intf = $phScene.CreateSolid(PHSolidDesc.new);
		@intf = i
	end
	def mass
		@intf.GetMass();
	end
	def mass=(m)
		@intf.SetMass(m);
	end
end

end