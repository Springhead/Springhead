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
            PHRaycastHit raycastHit = new PHRaycastHit();
            descScene.numIteration = 123;
            System.Console.WriteLine("int:      expected: 123   result: " + descScene.numIteration);
            descScene.bCCDEnabled = true;
            System.Console.WriteLine("bool:     expected: true  result: " + descScene.bCCDEnabled);
            raycastHit.distance = 0.123F;
            System.Console.WriteLine("float:    expected: 0.123 result: " + raycastHit.distance);
            descScene.airResistanceRate = 0.123;
            System.Console.WriteLine("double:   expected: 0.123 result: " + descScene.airResistanceRate);

            descScene.gravity.x = 0;
            descScene.gravity.y = 0;
            descScene.gravity.z = -4.5;
/**/        //System.Console.WriteLine("st_elm:   expected: (0.0, 0.0, -4.5)  result: " + descScene.gravity);
            descScene.gravity = new Vec3d(2.5, -5.2, 0.5);
/**/        //System.Console.WriteLine("st_assign expected: (2.5, -5.2, 0.5)  result: " + descScene.gravity);
            
/**/        // Vec3d d = descScene.gravity;
/**/        //System.Console.WriteLine(d.ToString());
/**/        //System.Console.WriteLine(d);

            // vector - intrinsic
            PHFemMeshNewDesc descFemMeshNew = new PHFemMeshNewDesc();
            vectorwrapper_int tets = descFemMeshNew.tets;

            tets.push_back(101);
            tets.push_back(102);
            System.Console.WriteLine("vec<int>: expected: 2     result: " + tets.size());
            System.Console.WriteLine("vec<int>: expected: 101   result: " + tets[0]);
            System.Console.WriteLine("vec<int>: expected: 102   result: " + tets[1]);
            tets[0] = 201;
            tets[1] = 202;
            tets.push_back(203);
            System.Console.WriteLine("vec<int>: expected: 3     result: " + tets.size());
            System.Console.WriteLine("vec<int>: expected: 201   result: " + tets[0]);
            System.Console.WriteLine("vec<int>: expected: 202   result: " + tets[1]);
            System.Console.WriteLine("vec<int>: expected: 203   result: " + tets[2]);

            // vector -struct
            vectorwrapper_PHFemMeshNewDesc_Vec3d vertices = descFemMeshNew.vertices;
            //vertices.push_back(new Vec3d(0.1, 0.2, 0.3));
            //vertices.push_back(new Vec3d(0.4, 0.5, 0.6));
        }
    }
}
