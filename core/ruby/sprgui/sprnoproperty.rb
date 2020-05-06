include Fox

class SprNoProperty < FXVerticalFrame

	attr :index

	def initialize(owner, index)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		@index = index
		FXLabel.new(self, 'no information')
	end
end