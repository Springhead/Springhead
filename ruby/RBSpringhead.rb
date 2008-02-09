######################################################################################
#
# RBSpringhead : Wraps some nice interface to a raw ruby extension module created by Swig.
#
#
######################################################################################

module Springhead

class Named
end
class Solid < Named
end
class Joint < Named
end

class PoseRef < Posed
	def initialize(p, o, sop = true)	# p : Posed, o : Solid or Joint, sop : SocketPose or PlugPose when owner is Joint
		super(p)
		@owner = o
		@sock_or_plug = sop
	end
	def update
		if @owner.instance_of? Object
			@owner.pose = self
		elsif @owner.instance_of? Joint
			if @sock_or_plug
				@owner.socket = self
			else
				@owner.plug = self
			end
		end
	end
	def pos
		Vec3Ref.new(super, self)
	end
	def ori
		QuaternionRef.new(super, self)
	end
	def pos=(v) super(v); update; end
	def ori=(q) super(q); update; end
end

class Vec3Ref < Vec3d
	def initialize(v, o)
		super(v)
		@owner = o
	end
	def update
		@owner.pos = self
		@owner.update
	end
	def x=(val) super(val); update; end
	def y=(val) super(val); update; end
	def z=(val) super(val); update; end
end

class QuaternionRef < Quaterniond
	def initialize(q, o)
		super(q)
		@owner = o
	end
	def update
		@owner.ori = self
		@owner.update
	end
	def w=(val) super(val); update; end
	def x=(val) super(val); update; end
	def y=(val) super(val); update; end
	def z=(val) super(val); update; end
end

class Named
	attr_reader :intf
    def initialize(i)
        @intf = i
    end
    def name
        @intf.GetName
    end
    def name=(n)
        @intf.SetName(n)
		self
    end
end

class ShapePoses
    include Enumerable
    def initialize(i)
        @intf = i
    end
    
    def each
        (0...@intf.NShape).each{|i| yield(@intf.GetShapePose(i))}
    end
    def [](i)
        @intf.GetShapePose(i)
    end
    def []=(i, p)
        @intf.SetShapePose(i, p)
    end
end

class Frames
	include Enumerable
	def initialize(f)
		@parent = f
	end
	def each
		#@parent.GetChildren.each{|v| yield(
	end
end

class Frame < Named
	def initialize(i)		# i : GRFrameIf
		super(i)
		@intf = GRFrameIf.Cast(i)
	end
	def transform
		@intf.GetTransform
	end
	def transform=(af)
		@intf.SetTransform(af)
		self
	end
	def children
		Frames.new(self)
	end
end

# FWObject. 
class Object < Named
	attr_reader :scene

	def initialize(i, s)		# i : FWObjectIf, s : Scene
		super(i)
		@intf = FWObjectIf.Cast(i)
		@scene = s
	end
	
    def mass
        @intf.GetPHSolid.GetMass
    end
    def mass=(m)
        @intf.GetPHSolid.SetMass(m)
		self
    end

    def inertia
        @intf.GetPHSolid.GetInertia
    end
    def inertia=(i)
        @intf.GetPHSolid.SetInertia(i)
		self
    end
    
    def center
        @intf.GetPHSolid.GetCenterOfMass
    end
    def center=(c)
        @intf.GetPHSolid.SetCenterOfMass(c)
		self
    end
    
    def pos
        @intf.GetPHSolid.GetCenterPosition
    end
    def pos=(p)
        @intf.GetPHSolid.SetCenterPosition(p)
		self
    end

    def deltaPos(p=nil)
        if p.nil?
            @intf.GetPHSolid.GetDeltaPosition
        else
            @intf.GetPHSolid.GetDeltaPosition(p)
        end
    end

    def ori
        @intf.GetPHSolid.GetOrientation
    end
    def ori=(q)
        @intf.GetPHSolid.SetOrientation(q)
		self
    end
    
    def pose
		PoseRef.new(@intf.GetPHSolid.GetPose, self)
    end
    def pose=(p)
        @intf.GetPHSolid.SetPose(p)
		self
    end
    
    def vel
        @intf.GetPHSolid.GetVelocity
    end
    def vel=(v)
        @intf.GetPHSolid.SetVelocity(v)
		self
    end
    
    def angVel
        @intf.GetPHSolid.GetAngularVelocity
    end
    def angVel=(w)
        @intf.GetPHSolid.SetAngularVelocity(w)
		self
    end
    
    def force
        @intf.GetPHSolid.GetForce
    end
    def addForce(f, r=nil)
        if r.nil?
            @intf.GetPHSolid.AddForce(f)
        else
            @intf.GetPHSolid.AddForce(f,r)
        end
		self
    end
    def torque
        @intf.GetPHSolid.GetTorque
    end
    def addTorque(t)
        @intf.GetPHSolid.AddTorque(t)
		self
    end

	def dynamical
		@intf.GetPHSolid.IsDynamical
	end
	def dynamical=(on)
		@intf.GetPHSolid.SetDynamical(on)
		self
	end
    
    def shapes
        Shapes.new(self)
    end
	
	def frame
		Frame.new(self.intf.GetGRFrame)
	end
end

class Objects
	include Enumerable
	def initialize(scene)		# Scene object is passed
		@scene = scene;
	end
	def each
		@scene.intf.GetObjects.each {|o| yield(Object.new(o, @scene))}
	end
	def [](i)
		if i.kind_of? Integer
			Object.new(@scene.intf.GetObjects[i])
		elsif i.kind_of? String
			obj = @scene.intf.FindObject(i)
			if obj.nil?
				nil
			else
				Object.new(FWObjectIf.Cast(obj), @scene)
			end
		else
			nil
		end
	end
	def size
		@scene.intf.NObject()
	end
	def add(name = nil)
		obj = Object.new(@scene.intf.CreateObject(PHSolidDesc.new, GRFrameDesc.new), @scene)
		if name.kind_of? String
			obj.name = name
		end
		obj
	end
	def delete(o)
		@scene.intf.DelChildObject(o.intf)
		self
	end
end

class Camera < Named
	def initialize(i, s)			# i : GRCameraIf, s : Scene
		super(i)
		@intf = GRCameraIf.Cast(i)
		@scene = s
	end
	def frame
		if @intf.GetFrame.nil?		# create a frame if not exist
			f = GRFrameIf.Cast(@scene.intf.GetGRScene.CreateVisual(GRFrameDesc.GetIfInfo, GRFrameDesc.new))
			f.SetName('cameraframe')
			@intf.SetFrame(f)
		end
		Frame.new(@intf.GetFrame)
	end
	def frame=(f)
		@intf.SetFrame(f.intf)
		self
	end
#	def size
#		@intf.
end

JOINT_HINGE = 1
JOINT_SLIDER = 2
JOINT_BALL = 3

class Joint < Named
	def initialize(i)		# i : PHConstraintIf
		super(i)
	end
	def enabled
		@intf.IsEnabled
	end
	def enabled=(on)
		@intf.Enable(on)
		self
	end

	def relativePose
		@intf.GetRelativePose
	end
	def relativeVel
		@intf.GetRelativeVelocity
	end
	def constraintForce
		@intf.GetConstraintForce
	end

	def socket
		p = Posed.new
		@intf.GetSocketPose(p)
		PoseRef.new(p, self, true)
	end
	def socket=(p)
		@intf.SetSocketPose(p)
		self
	end

	def plug
		p = Posed.new
		@intf.GetPlugPose(p)
		PoseRef.new(p, self, false)
	end
	def plug=(p)
		@intf.SetPlugPose(p)
		self
	end
end

class Joint1D < Joint

end

class BallJoint < Joint

end

#
class Joints
    include Enumerable
    def initialize(s)		# o : Scene
        @scene = s
		@intf = s.intf.GetPHScene
    end
    
	def size
		@intf.NJoints
	end
    def each
		(0...@intf.NJoints).each{|i| yield(Joint.new(@intf.GetJoint(i)))}
    end
    def [](i)
        Joint.new(@intf.GetJoint(i))
    end
    def add(type, obj0, obj1)
        if type == JOINT_HINGE
            j = Joint.new(@intf.CreateJoint(obj0.intf.GetPHSolid, obj1.intf.GetPHSolid, PHHingeJointDesc.GetIfInfo, PHHingeJointDesc.new))
        elsif type == JOINT_SLIDER
            j = Joint.new(@intf.CreateJoint(obj0.intf.GetPHSolid, obj1.intf.GetPHSolid, PHSliderJointDesc.GetIfInfo, PHSliderJointDesc.new))
		elsif type == JOINT_BALL
			j = Joint.new(@intf.CreateJoint(obj0.intf.GetPHSolid, obj1.intf.GetPHSolid, PHBallJointDesc.GetIfInfo, PHBallJointDesc.new))
        end
		
		j	# return new Joint
    end
end

class Scene < Named
	attr_reader :app

	def initialize(i, a)		# i : FWSceneIf, a : App
		super(i)
		@intf = FWSceneIf.Cast(i)
		@app = a
	end
  
	def objects
		Objects.new(self)
	end
  
    def joints
        Joints.new(self)
    end
    
    def step
        @intf.Step
		self
    end

    def draw
        @intf.Draw
		self
    end

    def numIteration
        @intf.GetPHScene.GetNumIteration
    end
    def numIteration=(n)
        @intf.GetPHScene.SetNumIteration(n)
		self
    end

	def camera
		if @intf.GetGRScene.GetCamera.nil?
			@intf.GetGRScene.SetCamera(GRCameraDesc.new)
		end
		Camera.new(@intf.GetGRScene.GetCamera, self)
	end
end

#
class Scenes
    include Enumerable
    def initialize(a)		# App is passed
        @app = a
    end
    def size
        @app.GetSdk.NScene
    end
    def each
        (0...@app.GetSdk.NScene).each{|i| yield(Scene.new(@app.GetSdk.GetScene(i), @app))}
    end
    def [](i)
        Scene.new(@app.GetSdk.GetScene(i), @app)
    end
    def add
        Scene.new(@app.GetSdk.CreateScene(PHSceneDesc.new, GRSceneDesc.new), @app)
    end
    def delete(scene)
        @app.GetSdk.DelChildObject(scene.intf)
		self
    end
end

SHAPE_MESH = 1
SHAPE_SPHERE = 2
SHAPE_CAPSULE = 3
SHAPE_BOX = 4

class Shape < Named
    def initialize(i)
        super(i)
    end
end

class Mesh < Shape
	def initialize(i)
		super(i)
		@intf = CDConvexMeshIf.Cast(i)
	end
end

class Sphere < Shape
	def initialize(i)
		super(i)
		@intf = CDSphereIf.Cast(i)
	end
end

class Capsule < Shape
	def initialize(i)
		super(i)
		@intf = CDCapsuleIf.Cast(i)
	end
end

class Box < Shape
	def initialize(i)
		super(i)
		@intf = CDBoxIf.Cast(i)
	end
	def size
		@intf.GetBoxSize
	end
	def size=(sz)
		@intf.SetBoxSize(sz)
		self
	end
end

#
class Shapes
    include Enumerable
    def initialize(o)		# o could be App or Object
        @owner = o
		if @owner.instance_of? App
			@intf = @owner.GetSdk.GetPHSdk
		else
			@intf = @owner.intf.GetPHSolid
		end
    end
    
	def size
		@intf.NShape
	end
    def each
		(0...@intf.NShape).each{|i| yield(Shape.new(@intf.GetShape(i)))}
    end
    def [](i)
		if i.kind_of? Integer
	        Shape.new(@intf.GetShape(i))
		elsif i.kind_of? String
			shape = @intf.FindObject(i)
			if shape.nil?
				nil
			else
				Shape.new(CDShapeIf.Cast(shape))
			end
		else
			nil
		end
    end
    def add(shape, name = nil)
		# if an integer is passed, create a shape of the specified type
		if shape.kind_of? Integer

			phsdk = (@owner.instance_of? App) ? @intf : @owner.scene.app.GetSdk.GetPHSdk

	        if shape == SHAPE_MESH
	            s = Mesh.new(phsdk.CreateShape(CDConvexMeshDesc.GetIfInfo, CDConvexMeshDesc.new))
	        elsif shape == SHAPE_SPHERE
	            s = Sphere.new(phsdk.CreateShape(CDSphereDesc.GetIfInfo, CDSphereDesc.new))
			elsif shape == SHAPE_CAPSULE
				s = Capsule.new(phsdk.CreateShape(CDCapsuleDesc.GetIfInfo, CDCapsuleDesc.new))
			else
				s = Box.new(phsdk.CreateShape(CDBoxDesc.GetIfInfo, CDBoxDesc.new))
	        end
			
			# add it if the owner is Object
			if @owner.instance_of? Object
				@owner.intf.GetPHSolid.AddShape(s.intf)
			end
			# set name if specified
			if name.kind_of? String
				s.name = name
			end
			s	# return new Shape

		# if a Shape is passed, just add it
		elsif shape.instance_of? Shape
			if @owner.instance_of? App
				@owner.GetSdk.GetPHSdk.AddChildObject(shape.intf)
			else
				@owner.intf.GetPHSolid.AddShape(shape.intf)
			end
			shape
		else
			nil
		end
    end
end

# Application
class App < FWAppGL
    def initialize
		super
        self.Link
        self.Init
        self.GetSdk.SetDebugMode
    end

	def scene
		Scene.new(self.GetSdk.GetScene, self)
	end
    def scenes
        Scenes.new(self)
    end
    
    def shapes
        Shapes.new(self)
    end

    def debug
        self.GetSdk.GetDebugMode
    end
    def debug=(on)
        self.GetSdk.SetDebugMode(on)
		self
    end

	def clear
		self.GetSdk.Clear
		self
	end
end

end
