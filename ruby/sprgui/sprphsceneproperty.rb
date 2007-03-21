require 'sprintproperty'
require 'sprfloatproperty'
require 'sprvectorproperty'

include Fox

class SprPHSceneProperty < FXVerticalFrame

	attr :index

	def initialize(owner, index)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		@index = index

		matrix = FXMatrix.new(self, 2, (MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		# time step
		FXLabel.new(matrix, 'time step')
		@timestep = SprFloatProperty.new(matrix, 3)

		# gravity
		FXLabel.new(matrix, 'gravity')
		@gravity = SprVectorProperty.new(matrix, 3)

		# num of iteration
		FXLabel.new(matrix, 'iteration')
		@numiteration = SprIntProperty.new(matrix, 1..100)
	end

	def update(scene, upload)
		if upload
			@timestep.update([scene.GetTimeStep()], true)
			@gravity.update(scene.GetGravity(), true)
			@numiteration.update([scene.GetNumIteration()], true)
		else
			step = 0
			grav = [0,0,0]
			iter = 0
			@timestep.update(step, false)
			@gravity.update(grav, false)
			@numiteration.update(iter, false)
			scene.SetTimeStep(step)
			scene.SetGravity(grav)
			scene.SetNumIteration(iter)
		end
	end
end