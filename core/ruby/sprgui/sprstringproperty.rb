include Fox

class SprStringProperty < FXTextField
	def initialize(owner)
		super(owner, 10, nil, 0, (JUSTIFY_RIGHT|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
	end

	def update(str, upload)
		if upload
			self.text = str
		else
			str = self.text
		end
	end
end
