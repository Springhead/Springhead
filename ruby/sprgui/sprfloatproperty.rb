include Fox

class SprFloatProperty < FXTextField
	def initialize(owner)
		super(owner, 10, nil, 0, (TEXTFIELD_REAL|JUSTIFY_RIGHT|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
	end

	def update(val, upload)
		if upload
			self.text = val.to_s
		else
			val = self.text.to_f
		end
	end
end
