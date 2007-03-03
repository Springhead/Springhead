require 'sprvectorproperty'

include Fox

class SprQuatEditDlg < FXDialogBox
	def initialize(owner, quat)
	    super(owner, "Edit Quaternion", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,
		     0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
	    rows = FXVerticalFrame.new(self, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y)
		@elems = SprVectorProperty.new(rows, 4)
		@elems.update(quat, true)

		FXHorizontalSeparator.new(rows)

	   	matrix = FXMatrix.new(rows, 3, (MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y), 0, 0, 0, 0, 0, 0, 0, 0, 2, 4)

		FXLabel.new(matrix, "axis-angle")
		frame = FXHorizontalFrame.new(matrix, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
		FXLabel.new(frame, " axis")
		@axis = SprVectorProperty.new(frame, 3)
		FXLabel.new(frame, " angle [deg]")
		@angle = SprFloatProperty.new(frame)
		FXButton.new(matrix, " calc ").connect(SEL_COMMAND, method(:onCalcAxisAngle))

		FXLabel.new(matrix, "yaw-pitch-roll")
		frame = FXHorizontalFrame.new(matrix, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
		FXLabel.new(frame, " yaw [deg]")
		@yaw = SprFloatProperty.new(frame)
		FXLabel.new(frame, " pitch [deg]")
		@pitch = SprFloatProperty.new(frame)
		FXLabel.new(frame, " roll [deg]")
		@roll = SprFloatProperty.new(frame)		
		FXButton.new(matrix, " calc ").connect(SEL_COMMAND, method(:onCalcYawPitchRoll))
		#radiogroup = FXDataTarget.new(0)
		#FXRadioButton.new(cols, "x-axis", radiogroup)
		#FXRadioButton.new(cols, "y-axis", radiogroup)
		#FXRadioButton.new(cols, "z-axis", radiogroup)
		#FXRadioButton.new(cols, "any", radiogroup)

		FXHorizontalSeparator.new(rows)

	    cols = FXHorizontalFrame.new(rows, LAYOUT_FILL_X|LAYOUT_FILL_Y)
		FXButton.new(cols, " OK ", nil, self, ID_ACCEPT, LAYOUT_RIGHT)
		FXButton.new(cols, " Cancel ", nil, self, ID_CANCEL, LAYOUT_RIGHT)
	end

	def onCalcAxisAngle(sender, sel, ptr)
		v = []
		theta = []
		@axis.update(v, false)
		v3 = Vec3d.new(v[0], v[1], v[2])
		@angle.update(theta, false)
		q = Quaterniond.Rot(Rad(theta[0]), v3)
		print q.x, q.y, q.z, q.w
		@elems.update([q.w, q.x, q.y, q.z], true)
	end

	def onCalcYawPitchRoll(sender, sel, ptr)
		y = []
		p = []
		r = []
		@yaw.update(y, false)
		@pitch.update(p, false)
		@roll.update(r, false)
		print y, p, r
		q = Quaterniond.new
		q.Euler(Rad(y[0]), Rad(p[0]), Rad(r[0]))
		@elems.update([q.w, q.x, q.y, q.z], true)
	end

end
