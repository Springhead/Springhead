require 'sprquateditdlg'

include Fox

class SprQuatProperty < SprVectorProperty
	def initialize(owner)
		super(owner, 4)
		FXButton.new(self, " edit... ", nil, nil, 0, (FRAME_RAISED|LAYOUT_FILL_Y)).connect(SEL_COMMAND, method(:onEdit))
	end

	def onEdit(sender, sel, ptr)
		q = []
		update(q, false)
		dlg = SprQuatEditDlg.new(self, q)
		if dlg.execute != 0
			dlg.update(q, false)
			update(q, true)
		end
	end
end