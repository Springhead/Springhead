include Fox

class SprFloatProperty < FXTextField
	def initialize(owner, res=2)
		super(owner, 5, nil, 0, (TEXTFIELD_REAL|JUSTIFY_RIGHT|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
		@res = res
	end

	def update(val, upload)
		if upload
			self.text = sprintf("%.#{@res}f", val[0])
		else
			val[0] = self.text.to_f
		end
	end
end
