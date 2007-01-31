include Fox

class SprNoProperty < FXVerticalFrame
	def initialize(owner)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		FXLabel.new(self, 'no information')
	end
end