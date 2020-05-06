include Fox

class SprIntProperty < FXSpinner
	def initialize(owner, range)
	    super(owner, 3, nil, 0, (JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
	    self.range = range
	end

	def update(val, upload)
		if upload
			self.value = val[0]
		else
			val[0] = self.value
		end
	end
end
