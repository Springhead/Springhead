using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using Spr;

namespace SprCsSample {
    class Program {
        static void Main(string[] args) {
            Vec3d v1 = new Vec3d(1, 2, 3), v2 = new Vec3d(4, 5, 6);
            while (true) {
                System.Console.WriteLine(v1 + v2);
            }
        }
    }
}
