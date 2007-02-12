require '../PTM'

include PTM

v1 = Vec2d.new(1, 2)
v2 = Vec2d.new(3, 4)
v = v1 + v2
puts sprintf("%f %f", v.x, v.y)