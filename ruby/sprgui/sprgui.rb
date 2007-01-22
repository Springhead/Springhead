#!/usr/bin/env ruby

require '../Springhead'
require 'sprmainwindow'

include Springhead

$sprapp

# Start the whole thing
if __FILE__ == $0
	# 
	$sprapp = FWAppGL.new
	$sprapp.Link
	$sprapp.Init(1 + ARGV.size, [$0] + ARGV)
	$sprapp.CreateRender
	$sprapp.LoadScene("test.x")

	# Make application
	application = FXApp.new("Springhead GUI", "Springhead")
	application.threadsEnabled = false
	
	# Open display
	application.init(ARGV)
	
	# Make window
	window = SprMainWindow.new(application)
	
	# Handle interrupt to save stuff nicely
	# application.addSignal("SIGINT", window, TextWindow::ID_QUIT)
	
	# Create it
	application.create
	
	# Start
	window.start(ARGV)
	
	# Run
	application.run
end
