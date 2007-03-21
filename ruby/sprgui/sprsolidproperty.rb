require 'sprfloatproperty'
require 'sprboolproperty'
require 'sprvectorproperty'
require 'sprmatrixproperty'
require 'sprquatproperty'
require 'sprobjectproperty'
require 'sprobjectproperty'
require 'sprshapeseditdlg'

include Fox

class SprSolidProperty < FXVerticalFrame

	attr :index

	def initialize(owner, index)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)

		@index = index

		@object = SprObjectProperty.new(self)

		FXHorizontalSeparator.new(self)

		matrix = FXMatrix.new(self, 2, (MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y), 0, 0, 0, 0, 0, 0, 0, 0, 1, 1)

		# velocity
		FXLabel.new(matrix, 'velocity')
		@velocity = SprVectorProperty.new(matrix, 3)

		# angular velocity
		FXLabel.new(matrix, 'ang-velocity')
		@angvel = SprVectorProperty.new(matrix, 3)

		# position
		FXLabel.new(matrix, 'position')
		@pose = SprVectorProperty.new(matrix, 3)

		# orientation
		FXLabel.new(matrix, 'orientation')
		@orientation = SprQuatProperty.new(matrix)

		# force
		FXLabel.new(matrix, 'force')
		@force = SprVectorProperty.new(matrix, 3)

		# torque
		FXLabel.new(matrix, 'torque')
		@torque = SprVectorProperty.new(matrix, 3)

		# mass
		FXLabel.new(matrix, 'mass')
		@mass = SprFloatProperty.new(matrix)
		
		# inertia
		FXLabel.new(matrix, 'inertia')
		@inertia = SprMatrixProperty.new(matrix, 3, 3)

		# center
		FXLabel.new(matrix, 'center of mass')
		@center = SprVectorProperty.new(matrix, 3)

		# dynamical?
		FXLabel.new(matrix, 'dynamical')
		@dynamical = SprBoolProperty.new(matrix)

		# shapes
		FXLabel.new(matrix, 'shapes')
		FXButton.new(matrix, ' edit ').connect(SEL_COMMAND, method(:onEditShapes))
	end

	def update(solid, upload)
		@object.update(solid, upload)
		if upload
			@solid = solid
			@mass.update([solid.GetMass()], true)
			@inertia.update(solid.GetInertia(), upload)
		else
			m = [0]
			i = [[0,0,0],[0,0,0],[0,0,0]]
			@mass.update(m, false)
			@inertia.update(i, false)
			solid.SetMass(m)
			solid.SetInertia(i)
		end
	end

	def onEditShapes(sender, sel, ptr)
		dlg = SprShapesEditDlg.new(self, @solid)
		if dlg.execute != 0

		end
	end
end
