using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using SprCs;

namespace SprCsSample {
    class Program {
        static void Main(string[] args) {
            bool do_intrinsic = true;
            bool do_tostring = true;
            bool do_vector = true;
            bool do_array = true;

            if (do_intrinsic)   test_intrinsic();
            if (do_tostring)    test_tostring();
            if (do_vector)      test_vector();
            if (do_array)       test_array();
        }

        static void test_intrinsic() {
            System.Console.WriteLine("---[ intrinsic ]---");
            // intrinsic member
            PHSceneDesc descScene = new PHSceneDesc();
            PHRaycastHit raycastHit = new PHRaycastHit();
            // simple
            descScene.numIteration = 123;
            System.Console.WriteLine("int:      expected: 123   result: " + descScene.numIteration);
            descScene.bCCDEnabled = true;
            System.Console.WriteLine("bool:     expected: true  result: " + descScene.bCCDEnabled);
            raycastHit.distance = 0.123F;
            System.Console.WriteLine("float:    expected: 0.123 result: " + raycastHit.distance);
            descScene.airResistanceRate = 0.123;
            System.Console.WriteLine("double:   expected: 0.123 result: " + descScene.airResistanceRate);
            // nested
            descScene.gravity.x = 0;
            descScene.gravity.y = 0;
            descScene.gravity.z = -4.5;
/**/        System.Console.WriteLine("set by elm:   expected: (0.0, 0.0, -4.5)  result: " + descScene.gravity);
            descScene.gravity = new Vec3d(2.5, -5.2, 0.5);
/**/        System.Console.WriteLine("set struct:   expected: (2.5, -5.2, 0.5)  result: " + descScene.gravity);
        }

        static void test_tostring() {
            System.Console.WriteLine("---[ ToString ]---");
            //PHSceneDesc descScene = new PHSceneDesc();
/**/        //Vec3d v3d = descScene.gravity;
            Vec3d v3d = new Vec3d(0.1, 0.2, 0.3);
/**/        System.Console.WriteLine("ToString: expected: (0.1, 0.2, 0.3)   result: " + v3d.ToString());
/**/        System.Console.WriteLine("ToString: C# ToString:                        " + v3d);
        }

        static void test_vector() {
            System.Console.WriteLine("---[ vector ]---");
            // vector member
            PHFemMeshNewDesc descFemMeshNew = new PHFemMeshNewDesc();
            vectorwrapper_int tets = descFemMeshNew.tets;
            // intrinsic element
            tets.push_back(101);
            tets.push_back(102);
            System.Console.WriteLine("vec<int>: expected: 2     result: " + tets.size());
            System.Console.WriteLine("vec<int>: expected: 101   result: " + tets[0]);
            System.Console.WriteLine("vec<int>: expected: 102   result: " + tets[1]);
            tets.clear();
            tets.push_back(101);
            tets.push_back(102);
            tets[0] = 201;
            tets[1] = 202;
            tets.push_back(203);
            System.Console.WriteLine("vec<int>: expected: 3     result: " + tets.size());
            System.Console.WriteLine("vec<int>: expected: 201   result: " + tets[0]);
            System.Console.WriteLine("vec<int>: expected: 202   result: " + tets[1]);
            System.Console.WriteLine("vec<int>: expected: 203   result: " + tets[2]);
            // structure element
            vectorwrapper_PHFemMeshNewDesc_Vec3d vertices = descFemMeshNew.vertices;
            vertices.push_back(new Vec3d(0.1, 0.2, 0.3));
            vertices.push_back(new Vec3d(0.4, 0.5, 0.6));
            System.Console.WriteLine("vec<Vec3d>: expected: 2     result: " + vertices.size());
            System.Console.WriteLine("vec<Vec3d>: expected: 0.1   result: " + vertices[0].x);
            System.Console.WriteLine("vec<Vec3d>: expected: 0.2   result: " + vertices[0].y);
            System.Console.WriteLine("vec<Vec3d>: expected: 0.3   result: " + vertices[0].z);
            System.Console.WriteLine("vec<Vec3d>: expected: (0.4, 0.5, 0.6) result: " + vertices[1].ToString());
        }

        static void test_array() {
            System.Console.WriteLine("---[ array ]---");
            PHOpObjDesc descOpObj = new PHOpObjDesc();
            GRMeshFace meshFace = new GRMeshFace();
            for (int i = 0; i < 4; i++) {
                meshFace.indices[i] = 100 + i;
            }
            for (int i = 0; i < 4; i++) {
                System.Console.WriteLine("array<int>: expected: " + (100 + i) + " result: " + meshFace.indices[i]);
            }
        }
    }
}
