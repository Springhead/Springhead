include Fox

class SprSolidProperty < FXVerticalFrame
	def initialize(owner)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)

		matrix = FXMatrix.new(self, 2, (MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y))

		# mass
		FXLabel.new(matrix, 'mass')
		@mass = FXTextField.new(matrix, 10, nil, 0, (TEXTFIELD_REAL|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
		
		# inertia
		FXLabel.new(matrix, 'inertia')
		inertiamatrix = FXMatrix.new(matrix, 3, (MATRIX_BY_ROWS|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@inertia = [[nil, nil, nil], [nil, nil, nil], [nil, nil, nil]]
		for r in 0..2
			for c in 0..2
				@inertia[r][c] = FXTextField.new(inertiamatrix, 10, nil, 0, (TEXTFIELD_REAL|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
			end
		end
	end

	def update(solid)
		@mass.text = solid.GetMass().to_s
		for r in 0..2
			for c in 0..2
				@inertia[r][c].text = solid.GetInertia()[r][c].to_s
			end
		end
	end
end