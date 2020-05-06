require 'sprnamelist'

include Fox

class SprJointCreateDlg < FXDialogBox
	def initialize(owner)
	    super(owner, "Create Joint", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,
		     0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
	    rows = FXVerticalFrame.new(self, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		
		@jointtype = SprNameList.new(rows, SprNameList::JOINTTYPE)
		@lhs = SprNameList.new(rows, SprNameList::SOLIDS)
		@rhs = SprNameList.new(rows, SprNameList::SOLIDS)

	    cols = FXHorizontalFrame.new(rows, LAYOUT_FILL_X|LAYOUT_FILL_Y)
		FXButton.new(cols, " OK ", nil, self, ID_ACCEPT, LAYOUT_RIGHT)
		FXButton.new(cols, " Cancel ", nil, self, ID_CANCEL, LAYOUT_RIGHT)
		
	end

end
