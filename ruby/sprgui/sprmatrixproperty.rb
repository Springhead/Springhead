include Fox

class SprMatrixProperty < FXHorizontalFrame
	def initialize(owner, nrow, ncol, res=2)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		@nrow = nrow
		@ncol = ncol
		@res = res
		@matrix = FXMatrix.new(self, @ncol, (MATRIX_BY_ROWS|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		for r in 0..(@nrow-1)
			for c in 0..(@ncol-1)
				FXTextField.new(@matrix, 5, nil, 0, (TEXTFIELD_REAL|JUSTIFY_RIGHT|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW))
			end
		end
	end

	def update(m, upload)
		for r in 0..(@nrow-1)
			for c in 0..(@ncol-1)
				if upload
					@matrix.childAtRowCol(r, c).text = sprintf("%.#{@res}f", m[r][c])
				else
					m[r][c] = @matrix.childAtRowCol(r, c).text.to_f
				end
			end
		end
	end
end
