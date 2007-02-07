include Fox

class SprBoolProperty < FXCheckButton
	def initialize(owner)
		super(owner, 'hoge')
	end

	def update(bool, upload)
		if upload
			setCheck(bool)
		else
			bool = checked?
		end
	end
end
