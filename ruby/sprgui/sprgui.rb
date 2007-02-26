#!/usr/bin/env ruby

require '../Springhead'
require '../PTM'
require 'sprmainwindow'

include Springhead
include PTM

$sprapp

class SprGUIApp < FXApp
	def initialize()
		super("Springhead GUI", "Springhead")
		
		# Make window
		@window = SprMainWindow.new(self)
		
		threadsEnabled = false

		# Open display
		init(ARGV)
			
		# Handle interrupt to save stuff nicely
		# application.addSignal("SIGINT", window, TextWindow::ID_QUIT)
		
		# Create it
		create
		
		# Start
		@window.start(ARGV)
		
		# Run
		run
	end
end

# Start the whole thing
if __FILE__ == $0
	# 
	$sprapp = FWAppGL.new
	$sprapp.Link
	$sprapp.Init(1 + ARGV.size, [$0] + ARGV)
	$sprapp.CreateRender

	# Make application
	application = SprGUIApp.new
	
end
