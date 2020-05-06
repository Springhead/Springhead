require 'sprstringproperty'

include Fox

class SprObjectProperty < FXVerticalFrame
	def initialize(owner)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		
		matrix = FXMatrix.new(self, 2, (MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		# name
		FXLabel.new(matrix, 'name')
		@name = SprStringProperty.new(matrix)
	end

	def update(object, upload)
		if upload
			@name.update(object.GetName(), true)
		else
			n = ''
			@name.update(n, false)
			object.SetName(n)
		end
	end
end