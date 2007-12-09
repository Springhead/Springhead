require 'RBSpringhead'

include Springhead

CreateSdk();

scene = Scene.new
solid = scene.solids.add
solid.mass = 3.0

puts scene.solids.size.to_s
puts solid.mass