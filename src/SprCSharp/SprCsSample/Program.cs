using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using SprCs;

namespace SprCsSample {
    class Program {
        static string def = "itvacfrs";
        static string inc = "A";	// include: "A" for all
        static string exc = "f";	// exclude:

        static void Main(string[] args) {
            if (inc.Equals("A")) {
                inc = def;
            }
            if (check_test("i"))  test_intrinsic();
            if (check_test("t"))  test_tostring();
            if (check_test("v"))  test_vector();
            if (check_test("a"))  test_array();
            if (check_test("c"))  test_type_conv();
            if (check_test("f"))  test_func_args();
            if (check_test("r"))  test_func_return();
            if (check_test("s"))  test_simulation();
        }

        static bool check_test(string chk) {
            return inc.Contains(chk) && !exc.Contains(chk);
        }

        static void test_intrinsic() {
            System.Console.WriteLine("---[ intrinsic ]---");

            // intrinsic member
            PHSceneDesc descScene = new PHSceneDesc();
            PHRaycastHit raycastHit = new PHRaycastHit();
            GRVertexElement vertexelm = new GRVertexElement();
            // simple
            vertexelm.offset = 123;
            System.Console.WriteLine("short:    expected: 123   result: " + vertexelm.offset);
            descScene.numIteration = 123;
            System.Console.WriteLine("int:      expected: 123   result: " + descScene.numIteration);
            descScene.bCCDEnabled = true;
            System.Console.WriteLine("bool:     expected: True  result: " + descScene.bCCDEnabled);
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

            Vec3d v3d = new Vec3d(0.1, 0.2, 0.3);
            //string s = v3d.ToString();
/**/        System.Console.WriteLine("ToString: expected: (0.1, 0.2, 0.3)   result: " + v3d.ToString());
/**/        System.Console.WriteLine("ToString: implicit ToString call            : " + v3d);

            PHSceneDesc descScene = new PHSceneDesc();
            PHSolidDesc descSolid = new PHSolidDesc();
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            PHSceneIf phScene = phSdk.CreateScene(descScene);
            PHSolidIf phSolid = phScene.CreateSolid(descSolid);
            phSolid.SetPose(new Posed(1, 0, 0, 0, 0, 2, 0));
            System.Console.WriteLine("ToString: phSolid:");
            System.Console.Write(phSolid.ToString());

            FWWinBaseDesc descWinBase = new FWWinBaseDesc();
            FWSdkIf fwSdk = FWSdkIf.CreateSdk();
            System.Console.WriteLine("ToString: fwSdk.ToString:");
            System.Console.Write(fwSdk.ToString());
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
            arraywrapper_int iarray = new arraywrapper_int(4);
            for (int i = 0; i < 4; i++)
            {
                meshFace.indices[i] = 200 + i;
            }
            meshFace.indices = iarray;
            for (int i = 0; i < 4; i++)
            {
                System.Console.WriteLine("array<int>: expected: " + (200 + i) + " result: " + meshFace.indices[i]);
            }
            //Vec3i v3i = new Vec3i(1, 2, 3);
            //System.Console.WriteLine("array<int> alloc(3): result:   " + v3i);
            //v3i.alloc(2);
            //for (int i = 0; i < 2; i++) {
            //    v3i[i].x = 10 * (i + 1);
            //    v3i[i].y = 10 * (i + 1);
            //    v3i[i].z = 10 * (i + 1);
            //}
            //System.Console.WriteLine("                     expected: " + "((10, 10, 10), (20, 20, 20))");
            //System.Console.WriteLine("                     result:   " + v3i);

        }

        static void test_type_conv() {
            System.Console.WriteLine("---[ type conversion ]---");

            string msg_f3 = "f2d: (1, 2, 3)";
            string msg_d3 = "d2f: (4, 5, 6)";

            Vec3f f3 = new Vec3f(1, 2, 3);
            Vec3d d3 = new Vec3d(4, 5, 6);
            print_vec3d(f3, msg_f3);
//            print_vec3f(d3, msg_f3);    // This cause CS1502 and CS1503 compile error. <- OK
            print_vec3f((Vec3f) d3, msg_d3);

            Vec3fStruct f3s = f3;
            Vec3dStruct d3s = d3;
            print_vec3ds(f3s, msg_f3);
//            print_vec3fs(d3s, msg_d3);  // This cause CS1502 and CS1503 compile error. <- OK
            print_vec3fs((Vec3fStruct) d3s, msg_d3);

            // -----
            // 戻り値の自動型変換のテスト
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            CDBoxDesc descBox = new CDBoxDesc();
            descBox.boxsize = new Vec3f(1,4,9);
            CDShapeIf shape = phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox);
            // ↑ CreateShapeはCDBoxIfオブジェクトを返す。それをCDShapeIfで受ける。

            // CDShapeIf型の変数に格納されているが中身はCDBoxIfなので型変換可能。
            CDBoxIf box = shape as CDBoxIf;
            System.Console.WriteLine("type_conv: expected: (1, 4, 9)  result: " + box.GetBoxSize().ToString());

            // CDBoxIf is not a CDSphereIf なので nullになることが期待される。
            CDSphereIf sphere = shape as CDSphereIf;
            if (sphere == null) {
                System.Console.WriteLine("type_conv: expected: null  result: null");
            } else {
                System.Console.WriteLine("type_conv: expected: null  result: " + sphere.ToString());
            }
            
            // CDBoxIf is a CDConvexIf なのでnullにはならず型変換される。
            CDConvexIf convex = shape as CDConvexIf;
            if (convex == null) {
                System.Console.WriteLine("type_conv: expected: not null  result: null");
            } else {
                System.Console.WriteLine("type_conv: expected: not null  result: " + convex.ToString());
            }

            // -----
            PHSceneDesc descScn = new PHSceneDesc();
            PHSceneIf scene = phSdk.CreateObject(PHSceneIf.GetIfInfoStatic(), descScn) as PHSceneIf;
            System.Console.WriteLine((scene == null) ? "null" : scene.ToString());
        }

        static void print_vec3d(Vec3d v, string exp) {
            System.Console.WriteLine("type_conv: expected: " + exp + " result: " + v.ToString());
        }

        static void print_vec3f(Vec3f v, string exp) {
            System.Console.WriteLine("type_conv: expected: " + exp + " result: " + v.ToString());
        }

        static void print_vec3ds(Vec3dStruct v, string exp) {
            System.Console.WriteLine("type_conv: expected: " + exp + " result: (" + v.x + ", " + v.y + ", " + v.z + ")");
        }

        static void print_vec3fs(Vec3fStruct v, string exp) {
            System.Console.WriteLine("type_conv: expected: " + exp + " result: (" + v.x + ", " + v.y + ", " + v.z + ")");
        }

        static void test_func_args() {
            System.Console.WriteLine("---[ function arguments ]---");

            // ここでのコードは正常には動作はしない － 例外が起きて停止する。
            // デバッガで止めて値を確認すること。
            //
            // [int]
            CDShapePairIf shapePairIf = new CDShapePairIf();
            shapePairIf.GetShape(123);
        }
        static void test_func_return() {
            System.Console.WriteLine("---[ function return ]---");
            int memoryLeakTest = 0;

            PHSceneDesc descScene = new PHSceneDesc();
            PHSolidDesc descSolid = new PHSolidDesc();
            if (memoryLeakTest == 1) return;

            PHSdkIf phSdk = PHSdkIf.CreateSdk();        // ここでメモリリークする
            if (memoryLeakTest == 2) return;
            PHSceneIf phScene = phSdk.CreateScene(descScene);

            descSolid.mass = 2.0;
            descSolid.inertia = new Matrix3d(2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 2.0);
            PHSolidIf phSolid = phScene.CreateSolid(descSolid);
            descSolid.mass = 1e20f;

            descSolid.inertia = new Matrix3d(1e20f, 0.0, 0.0, 0.0, 1e20f, 0.0, 0.0, 0.0, 1e20f);
            PHSolidIf solid1 = phScene.CreateSolid(descSolid);

            PHHapticPointerDesc descHaptic = new PHHapticPointerDesc();
            PHHapticPointerIf phHaptic = phScene.CreateHapticPointer();

            //HISdkDesc descHi = new HISdkDesc();
            //HISdkIf hiSdk = HISdkIf.CreateSdk();

            System.Console.WriteLine("ret int:    expected: 3       result: " + phScene.NSolids());
            for (int i = 0; i < 20; i++) {
                phScene.Step();
            }
            System.Console.WriteLine("ret Uint:   expected: 20      result: " + phScene.GetCount());
            phHaptic.SetLocalRange(2.345f);
            System.Console.WriteLine("ret float:  expected: 2.345   result: " + phHaptic.GetLocalRange());
            System.Console.WriteLine("ret double: expected: 0.005   result: " + phScene.GetTimeStep());
            phScene.SetMaxVelocity(1.23);
            System.Console.WriteLine("ret double: expected: 0.123   result: " + phScene.GetMaxVelocity());
            phScene.EnableContactDetection(false);
            System.Console.WriteLine("ret bool:   expected: False   result: " + phScene.IsContactDetectionEnabled());
            phScene.EnableContactDetection(true);
            System.Console.WriteLine("ret bool:   expected: True    result: " + phScene.IsContactDetectionEnabled());

            System.Console.WriteLine("ret size_t: expected: 152?    result: " + phScene.GetDescSize());

            System.Console.WriteLine("ret Vec3d:  expected: (0.0, -9.8, 0.0) result: " + phScene.GetGravity());
            phScene.SetGravity(new Vec3d(0.1, -9.9, 0.2));
            System.Console.WriteLine("ret Vec3d:  expected: (0.1, -9.9, 0.2) result: " + phScene.GetGravity());
        }

        static void test_simulation() {
            System.Console.WriteLine("---[ physical simulation ]---");

            PHSceneDesc descScene = new PHSceneDesc();
            PHSolidDesc descSolid = new PHSolidDesc();
            CDBoxDesc descBox = new CDBoxDesc();
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            PHSceneIf phScene = phSdk.CreateScene(descScene);
            PHSolidIf phSolid = phScene.CreateSolid(descSolid);
            phSolid.AddShape(phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox));
            phSolid.SetPose(new Posed(1, 0, 0, 0, 0, 2, 0));

            PHSolidIf phFloor = phScene.CreateSolid(descSolid);
            phFloor.SetDynamical(false);
            descBox.boxsize = new Vec3f(10, 10, 10);
            phFloor.AddShape(phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox));
            phFloor.SetPose(new Posed(1, 0, 0, 0, 0, -5, 0));

            for (int i = 0; i < 200; i++) {
                phScene.Step();
                //System.Console.WriteLine(i.ToString() + " : " + phSolid.GetPose());
                System.Console.WriteLine(String.Format("{0, 3}", i) + " : " + phSolid.GetPose());
            }

        }
    }
}
