#require "../rbPhysics/RBPhysics"
#require "../rbGraphics/RBGraphics"
require "RBFramework"

#include RBPhysics
#include RBGraphics
include RBFramework

app = FWAppGLUT.new
app.Link
app.Init(1 + ARGV.size, [$0] + ARGV + ["hoge", "moge"])
app.LoadScene("test.x")
#app.Start