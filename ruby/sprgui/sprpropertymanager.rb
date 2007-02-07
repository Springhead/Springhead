require 'noproperty'
require 'sprsolidproperty'

include Fox

class SprPropertyManager < FXVerticalFrame
	def initialize(owner)
		super(owner, FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y)
#		@noproperty = SprNoProperty.new(self)
		@solidproperty = SprSolidProperty.new(self)
	end

	def create()
		super
		update(nil)
	end

	def update(obj)
		return
		if obj.nil?
			@noproperty.show()
			@solidproperty.hide()
			layout()
			return
		end
		if obj.kind_of?(PHSolidIf)
			@solidproperty.update(obj, true)
			@noproperty.hide()
			@solidproperty.show()
			layout()
			return
		end
	end
end