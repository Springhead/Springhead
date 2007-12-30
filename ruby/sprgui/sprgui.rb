#!/usr/bin/env ruby

require '../Springhead'
require '../rbspringhead'
require 'rubygems'
require 'fox16'
require 'fox16/responder'
require 'fox16/undolist'
require 'sprmainwindow'

include Fox
include Springhead

$sprapp

class SprGUIApp < FXApp
	def initialize()
		super("Springhead GUI", "Springhead")

		# GL‰Šú‰»
	    $glvisual = FXGLVisual.new(self, VISUAL_NOACCEL | VISUAL_TRUECOLOR | VISUAL_DOUBLEBUFFER | VISUAL_SWAP_COPY)
		$glvisual.redSize = 5
		$glvisual.greenSize = 5
		$glvisual.blueSize = 5
		$glvisual.depthSize = 16
		
		# Springhead‰Šú‰»
		$sprapp = App.new

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
		#@window.start(ARGV)
		
		# Run
		run
	end
end

# Start the whole thing
if __FILE__ == $0
	# Make application
	application = SprGUIApp.new
	
end
