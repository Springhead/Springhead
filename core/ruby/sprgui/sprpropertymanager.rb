require 'sprnoproperty'
require 'sprphsceneproperty'
require 'sprsolidproperty'

include Fox

class SprPropertyManager < FXSwitcher
	def initialize(owner)
		super(owner)
		idx = 0
		@noproperty = SprNoProperty.new(self, idx); idx+=1
		@phsceneproperty = SprPHSceneProperty.new(self, idx); idx+=1
		@solidproperty = SprSolidProperty.new(self, idx); idx+=1
	end

	def create()
		super
		refresh(nil)
	end

	def refresh(obj)
		if obj.nil?
			setCurrent(@noproperty.index)
			return
		end
		if obj.kind_of?(PHSceneIf)
			@phsceneproperty.update(obj, true)
			setCurrent(@phsceneproperty.index)
			return
		end
		if obj.kind_of?(PHSolidIf)
			@solidproperty.update(obj, true)
			setCurrent(@solidproperty.index)
			return
		end
	end

	def clear()
		refresh(nil)
	end
end
