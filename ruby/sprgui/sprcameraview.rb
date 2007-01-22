require 'rubygems'
require 'fox16'
require 'fox16/responder'
require 'fox16/undolist'

include Fox

class SprCameraView < FXGLCanvas

	def initialize(app)
  		# Call base class initialize first
  		super(app, "Springhead GUI", nil, nil, DECOR_ALL, 50, 50, 850, 600, 0, 0)

	end

end
