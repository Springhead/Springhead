include Fox

class SprShapesEditDlg < FXDialogBox
	def initialize(owner, solid)
	    super(owner, "Edit Shapes", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,
		     0, 0, 0, 0, 0, 0, 0, 0, 4, 4)

		rows = FXVerticalFrame.new(self, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y)

		@shapelist = FXVerticalFrame.new(rows, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y)

	    cols = FXHorizontalFrame.new(rows, LAYOUT_FILL_X|LAYOUT_FILL_Y)
		FXButton.new(cols, " Add ").connect(SEL_COMMAND, method(:onAddShape))
		FXButton.new(cols, " Delete ").connect(SEL_COMMAND, method(:onDelShape))

	    cols = FXHorizontalFrame.new(rows, LAYOUT_FILL_X|LAYOUT_FILL_Y)
		FXButton.new(cols, " OK ", nil, self, ID_ACCEPT, LAYOUT_RIGHT)
		FXButton.new(cols, " Cancel ", nil, self, ID_CANCEL, LAYOUT_RIGHT)

	    # solid‚ªŽ‚ÂŒ`ó‚Æ‚»‚ê‚ç‚ÌˆÊ’u‚Ì”z—ñ
		@shapenames = []
		@shapeposes = []
		for i in 0..solid.NShape()-1
			shape = solid.GetShape(i)
			@shapenames.push(shape.GetName())
			@shapeposes.push(Posed.new(solid.GetShapePose(i)))
		end
		
		@posarray = []
		@quatarray = []
		@listboxarray = []
		updatelist()
	end

	def updatelist()
		@posarray.each {destroy}
		@quatarray.each {destroy}
		@listboxarray.each{destroy}

		for i in 0..@shapenames.size-1
	    	cols = FXHorizontalFrame.new(@shapelist, LAYOUT_FILL_X|LAYOUT_FILL_Y)
			@posarray.push(SprVectorProperty.new(cols, 3))
			@posarray.last.update(@shapeposes[i].pos, true)
			
			@quatarray.push(SprVectorProperty.new(cols, 4))
			@quatarray.last.update(@shapeposes[i].ori, true)

			@listboxarray.push(SprNameList.new(cols, SprNameList::SHAPES, @solid))
			@listboxarray.last.update(@shapenames[i])
		end

		@shapelist.recalc
		recalc
	end

	def onAddShape(sender, sel, ptr)

	end

	def onDelShape(sender, sel, ptr)

	end
end
