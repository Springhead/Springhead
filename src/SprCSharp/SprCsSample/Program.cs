using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using SprCs;

namespace SprCsSample {
    class Program {
        static void Main(string[] args) {
            PHSceneDesc descScene = new PHSceneDesc();
            descScene.airResistanceRate = 0.5;
            descScene.gravity = new Vec3d(0, 0, -4.5);

            System.Console.WriteLine(descScene.gravity);
        }
    }
}
