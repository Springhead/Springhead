require 'rubygems'
require 'fox16'
require 'fox16/responder'
require 'fox16/undolist'

include Fox
include Math

class SprCameraView < FXGLCanvas

	def initialize(owner)
	    super(owner, $glvisual, nil, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT)
	    connect(SEL_PAINT){drawScene}
	    connect(SEL_CONFIGURE){configure}
		connect(SEL_MOTION, method(:onMotion))

		@cameraRotX = 0
		@cameraRotY = 0
		@cameraZoom = 1
	end

	def create
		super
		makeCurrent
		$sprapp.CreateRender
		makeNonCurrent
	end
	# draw scene
	def	drawScene()
	    # Make context current
    	makeCurrent()

		$sprapp.Reshape(width, height)
		$sprapp.Display()

		# Swap if it is double-buffered
    	if $glvisual.isDoubleBuffer
      		swapBuffers
    	end
    
    	# Make context non-current
    	makeNonCurrent
	end

	def configure()
    	if makeCurrent
        	$sprapp.Reshape(width, height)
        	makeNonCurrent
      	end
	end

	def onMotion(sender, sel, event)
		state = cursorPosition()[2]
		xrel = event.win_x - event.last_x
		yrel = event.win_y - event.last_y
		# 左ボタン
		if state == 256
			@cameraRotY += xrel * 0.01
			@cameraRotY = Rad( 180.0) if @cameraRotY > Rad( 180.0)
			@cameraRotY = Rad(-180.0) if @cameraRotY < Rad(-180.0)
			@cameraRotX += yrel * 0.01
			@cameraRotX = Rad( 80.0) if @cameraRotX > Rad( 80.0)
			@cameraRotX = Rad(-80.0) if @cameraRotX < Rad(-80.0)
		end
		# 右ボタン
		if state == 1024
			@cameraZoom *= exp(yrel/10.0)
			@cameraZoom = 10.0 if @cameraZoom > 10.0
			@cameraZoom =  0.1 if @cameraZoom <  0.1
		end

		if state == 256 || state == 1024
			# カメラフレームを動かす
			af = Affinef.new;
			af.pos = Vec3f.new(cos(@cameraRotX) * cos(@cameraRotY), sin(@cameraRotX), cos(@cameraRotX) * sin(@cameraRotY)) * @cameraZoom
			af.LookAtGL(Vec3f.new(0.0, 0.0, 0.0), Vec3f.new(0.0, 100.0, 0.0))

			$sprapp.scene.camera.frame.transform = af

			drawScene
		end
	end

end
