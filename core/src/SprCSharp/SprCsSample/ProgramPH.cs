using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using SprCs;

namespace SprCsSample {
    class Program {
        static string def = "itvacfroFs";
        static string inc = "A";	// include: "A" for all
        static string exc = "s";	// exclude:
        static string brk = "n";	// set "y" if run under debugger
        static int OK = 0;
        static int NG = 0;

        static void Main(string[] args) {

	    // DLLのあるディレクトリを求めるための準備
	    //	  cmakeを使ったか否かでcurrent directotyが異なるためのKludge
	    var CurrDirectory = System.IO.Directory.GetCurrentDirectory();
	    var CurrDirectories = CurrDirectory.Split('\\');
	    var ToIndex = 0;
	    for (int i = CurrDirectories.Length-1; i >= 0; i--) {
		if (CurrDirectories[i].Equals("core")) {
		    ToIndex = i;
		    break;
		}
	    }
	    var TopDirectory = "";
	    for (int i = 0; i < ToIndex; i++) {
		TopDirectory += CurrDirectories[i] + @"\";
	    }
	    // ここまで
	    var dllPath_64 = TopDirectory + @"generated\bin\win64";
	    var dllPath_32 = TopDirectory + @"generated\bin\win32";
            var dllDirectory = dllPath_64 + ";" + dllPath_32;
            var dllPath = Environment.GetEnvironmentVariable("PATH") + ";" + dllDirectory;
            Environment.SetEnvironmentVariable("PATH", dllPath);

            ExceptionRaiser er = new ExceptionRaiser();
            try {
                if (inc.Equals("A"))  inc = def;
                if (check_test("i"))  test_intrinsic();
                if (check_test("t"))  test_tostring();
                if (check_test("v"))  test_vector();
                if (check_test("a"))  test_array();
                if (check_test("c"))  test_type_conv();
                if (check_test("f"))  test_func_args();
                if (check_test("r"))  test_func_return();
                if (check_test("o"))  test_operator();
                if (check_test("F"))  test_func_call();
                if (check_test("s"))  test_simulation();
            }
            catch (System.Exception e) {
                System.Console.WriteLine(SEH_Exception.what(e));
            }
	    try {
                CSlog.Print("Test End");
	    } catch (System.Exception e) {
		    Console.WriteLine(e.ToString());
	    }
            System.Console.WriteLine("----");
            System.Console.WriteLine("Passed: " + OK + ", Failed: " + NG);
        }

        static void test_intrinsic() {
            test_name("intrinsic");

            // intrinsic member
            PHSceneDesc descScene = new PHSceneDesc();
            PHRaycastHit raycastHit = new PHRaycastHit();
            //-- GRVertexElement vertexelm = new GRVertexElement();
            // simple
            descScene.numIteration = 123;        put("int   ", "123  ", descScene.numIteration);
            descScene.bCCDEnabled = true;        put("bool  ", "True ", descScene.bCCDEnabled);
            raycastHit.distance = 0.123F;        put("float ", "0.123", raycastHit.distance);
            descScene.contactTolerance = 0.123; put("double", "0.123", descScene.contactTolerance);
            // nested
            descScene.gravity.x = 0;
            descScene.gravity.y = 0;
            descScene.gravity.z = -4.5;
            put("set by elm", "(0.0, 0.0, -4.5)", descScene.gravity);
            descScene.gravity = new Vec3d(2.5, -5.2, 0.5);
            put("set struct", "(2.5, -5.2, 0.5)", descScene.gravity);
            // structure
            Vec3d v3d = new Vec3d(0.1, 0.2, 0.3);
            put("Vec new", "(0.1, 0.2, 0.3)", v3d);
            put("Vec * c", "(0.2, 0.4, 0.6)", v3d * 2);
            put("c * Vec", "(0.2, 0.4, 0.6)", 2 * v3d);
            Posed pose = new Posed(new Vec3d(1, 2, 3), new Quaterniond(1, 0, 0, 0));
            put2("pose", new Posed(1, 0, 0, 0, 1, 2, 3), pose);
        }

        static void test_tostring() {
            test_name("ToString");

            Vec3d v3d = new Vec3d(0.1, 0.2, 0.3);
            put("ToString", "(0.1, 0.2, 0.3)", edit_vector(v3d));
            System.Console.WriteLine("     ToString: implicit ToString call           : " + v3d);

            PHSceneDesc descScene = new PHSceneDesc();
            PHSolidDesc descSolid = new PHSolidDesc();
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            PHSceneIf phScene = phSdk.CreateScene(descScene);
            PHSolidIf phSolid = phScene.CreateSolid(descSolid);
            phSolid.SetPose(new Posed(1, 0, 0, 0, 0, 2, 0));
            put_title("ToString: phSolid");
            put_indent(2, phSolid.ToString());
        }

        static void test_vector() {
            test_name("vector");

            // vector member
            PHFemMeshNewDesc descFemMeshNew = new PHFemMeshNewDesc();
            vectorwrapper_int tets = descFemMeshNew.tets;
            // intrinsic element
            tets.push_back(101);
            tets.push_back(102);
            put("vec<int>", "2  ", tets.size());
            put("vec<int>", "101", tets[0]);
            put("vec<int>", "102", tets[1]);
            tets.clear();
            tets.push_back(101);
            tets.push_back(102);
            tets[0] = 201;
            tets[1] = 202;
            tets.push_back(203);
            put("vec<int>", "3  ", tets.size());
            put("vec<int>", "201", tets[0]);
            put("vec<int>", "202", tets[1]);
            put("vec<int>", "203", tets[2]);
            // structure element
            vectorwrapper_Vec3d vertices = descFemMeshNew.vertices;
            vertices.push_back(new Vec3d(0.1, 0.2, 0.3));
            vertices.push_back(new Vec3d(0.4, 0.5, 0.6));
            put("vec<Vec3d>", "2  ", vertices.size());
            put("vec<Vec3d>", "0.1", vertices[0].x);
            put("vec<Vec3d>", "0.2", vertices[0].y);
            put("vec<Vec3d>", "0.3", vertices[0].z);
            put("vec<Vec3d>", "(0.4, 0.5, 0.6)", edit_vector(vertices[1]));
        }

        static void test_array() {
            test_name("array");

            string[] strsrc = { "abc", "def", "gh", "jklm" };
            arraywrapper_char_p icharp = new arraywrapper_char_p(4);
            for (int i = 0; i < 4; i++) {
                icharp[i] = strsrc[i];
            }
            for (int i = 0; i < 4; i++) {
                put("array<char*>", strsrc[i]+"\\0", icharp[i]);
            }
            Vec3i v3i = new Vec3i(1, 2, 3);
            System.Console.WriteLine("array<int> alloc(3): result:   " + edit_vector(v3i));

            Vec3i[] v3ia = new Vec3i[3];
            for (int i = 0; i < 3; i++) {
                v3ia[i] = new Vec3i(10*(i+1), 10*(i+1), 10*(i+1));
            }
            string result = string.Format("(({0}, {1}, {2}), ({3}, {4}, {5}), ({6}, {7}, {8}))",
                v3ia[0][0], v3ia[0][1], v3ia[0][2], v3ia[1][0], v3ia[1][1], v3ia[1][2], v3ia[2][0], v3ia[2][1], v3ia[2][2]);
            put2("array<int> Vec3i[3]: ", "((10, 10, 10), (20, 20, 20), (30, 30, 30))", result);
        }

        static void test_type_conv() {
            test_name("type conversion");

            string msg_f3 = "(1.0, 2.0, 3.0)";
            string msg_d3 = "(4.0, 5.0, 6.0)";

            Vec3f f3 = new Vec3f(1, 2, 3);
            Vec3d d3 = new Vec3d(4, 5, 6);
            Vec3d to_d = f3;
          //Vec3f to_f = d3;		// This cause CS0266 compile error
            Vec3f to_f = (Vec3f) d3;
            put("type_conv: f2d", msg_f3, to_d);
            put("type_conv: d2f", msg_d3, to_f);

            Vec3fStruct f3s = f3;
            Vec3dStruct d3s = d3;
            put("type_conv: f2d", msg_f3, edit_vector(f3s));
            put("type_conv: d2f", msg_d3, edit_vector((Vec3fStruct)d3s));

            // -----
            // 戻り値の自動型変換のテスト
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            CDBoxDesc descBox = new CDBoxDesc();
            CDShapeIf shape = phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox);

            // CDShapeIf is a NamedObjectIf なのでnullにはならず型変換される。
            NamedObjectIf named_obj = shape as NamedObjectIf;
            if (named_obj == null) {
                put("type_conv", "null", "null");
            } else {
                put("type_conv", "<CDBox shape0/>", named_obj.ToString());
            }
            
            // CDShapeIf is not a CDBoxIf なのでnullになることが期待される。
            CDBoxIf box = shape as CDBoxIf;
            if (box == null) {
                put("type_conv", "null", "null");
            } else {
                put("type_conv", "not null", box.ToString());
            }

            // -----
            PHSceneDesc descScn = new PHSceneDesc();
            PHSceneIf scene = phSdk.CreateObject(PHSceneIf.GetIfInfoStatic(), descScn) as PHSceneIf;
            System.Console.WriteLine((scene == null) ? "null" : scene.ToString());

	        // constructor による初期値設定のテスト
	        // ---- State を継承 ----
	        // 暗黙の型変換による
            PHSceneDescStruct structScene1 = new PHSceneDesc();
            Vec3d v3d1 = new Vec3d(0.0, -9.8, 0.0);
            Vec3d v3d2 = new Vec3d(1.0, -8.8, 1.0);
            put("by typeconv:    gravity ", edit_vector(v3d1), edit_vector(structScene1.gravity));
            put("by typeconv:    timeStep", "0.005", structScene1.timeStep);
            // constructor による
            PHSceneDescStruct structScene2 = new PHSceneDescStruct();
            put("by constructor: gravity ", edit_vector(v3d1), edit_vector(structScene2.gravity));
            put("by constructor: timeStep", "0.005", structScene2.timeStep);
            // ApplyFrom による
            PHSceneDescStruct structScene3 = new PHSceneDescStruct();
            structScene3.gravity = v3d2;
            structScene3.timeStep = 1.001;
            put("by Apply(): fm: gravity ", edit_vector(v3d2), edit_vector(structScene3.gravity));
            put("by Apply(): fm: timeStep", "1.001", structScene3.timeStep);
            structScene2.ApplyFrom((PHSceneDesc) structScene3);
            put("by Apply(): to: gravity ", edit_vector(v3d2), edit_vector(structScene2.gravity));
            put("by Apply(): to: timeStep", "0.005", structScene2.timeStep);
	        //
	        // ---- Desc を継承 ----
            CDSphereDescStruct structSphere1 = new CDSphereDescStruct();
            CDSphereDesc descSphere1 = structSphere1;
            CDSphereDescStruct structSphere2 = new CDSphereDescStruct();
            CDSphereDesc descSphere2 = structSphere2;
            //
            put("DescStruct: radius      ", "1", structSphere1.radius);
            put("DescStruct: base.density", "1000", structSphere1.material.density);
            put("ApplyFrom:");
            descSphere2.radius = 2;
            descSphere2.material.density = 2;
            structSphere1.ApplyFrom(descSphere2);
            put("DescStruct: radius      ", "2", structSphere1.radius);
            put("DescStruct: base.density", "2", structSphere1.material.density);
            put("assignment:");
            descSphere2.radius = 3;
            descSphere2.material.density = 3;
            structSphere1 = descSphere2;
            put("DescStruct: radius      ", "3", structSphere1.radius);
            put("DescStruct: base.density", "3", structSphere1.material.density);
            //
            put("ApplyTo:");
            structSphere1.radius = 4;
            structSphere1.material.density = 4;
            structSphere1.ApplyTo(descSphere2);
            put("Desc: radius      ", "4", descSphere2.radius);
            put("Desc: base.density", "4", descSphere2.material.density);
            put("assignment:");
            structSphere1.radius = 5;
            structSphere1.material.density = 5;
            descSphere2 = structSphere1;
            put("Desc: radius      ", "5", descSphere2.radius);
            put("Desc: base.density", "5", descSphere2.material.density);

            //
            // ---- 比較 ----
            Vec3d v3d_c11 = new Vec3d(1, 2, 3);
            Vec3d v3d_c12 = new Vec3d(1, 2, 3);
            Vec3d v3d_c21 = new Vec3d(4, 5, 6);
            put("compare: ==, same", "True ", v3d_c11 == v3d_c12);
            put("compare: ==, diff", "False", v3d_c11 == v3d_c21);
            put("compare: ==, null", "False", v3d_c11 == null);
            put("compare: ==, null", "False", null == v3d_c11);
            put("compare: !=, same", "False", v3d_c11 != v3d_c12);
            put("compare: !=, diff", "True ", v3d_c11 != v3d_c21);
            put("compare: !=, null", "True ", v3d_c11 != null);
            put("compare: !=, null", "True ", null != v3d_c11);
            vectorwrapper_int vw11 = new vectorwrapper_int((IntPtr) 0);
            vectorwrapper_int vw12 = new vectorwrapper_int((IntPtr) 0);
            vectorwrapper_int vw21 = new vectorwrapper_int((IntPtr) 1);
            put("compare: ==, wrap", "True ", vw11 == vw12);
            put("compare: ==, wrap", "False", vw11 == vw21);
            put("compare: ==, null", "False", vw11 == null);
            put("compare: ==, null", "False", null == vw21);
            put("compare: !=, wrap", "False", vw11 != vw12);
            put("compare: !=, wrap", "True ", vw11 != vw21);
            put("compare: !=, null", "True ", vw11 != null);
            put("compare: !=, null", "True ", null != vw21);
        }

        static void test_func_args() {
            test_name("function arguments");

            // 引数をちゃんと設定しないと例外が起きる！！
            if (brk != "y") return;

            // [int]
            CDShapePairIf shapePairIf = new CDShapePairIf();
            shapePairIf.GetShape(123);
        }

        static void test_func_return() {
            test_name("function return");
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

            put("ret int   ", "3    ", phScene.NSolids());
            for (int i = 0; i < 20; i++) {
                phScene.Step();
            }
            put("ret Uint  ", "20   ", phScene.GetCount());
            phHaptic.SetLocalRange(2.345f);
            put("ret float ", "2.345", phHaptic.GetLocalRange());
            put("ret double", "0.005", phScene.GetTimeStep());
            phScene.SetMaxVelocity(1.23);
            put("ret double", "1.23", phScene.GetMaxVelocity());
            phScene.EnableContactDetection(false);
            put("ret bool  ", "False", phScene.IsContactDetectionEnabled());
            phScene.EnableContactDetection(true);
            put("ret bool  ", "True ", phScene.IsContactDetectionEnabled());
            put("ret size_t", "160 ", phScene.GetDescSize());
            put("ret Vec3d ", "(0.0, -9.8, 0.0)", phScene.GetGravity());
            phScene.SetGravity(new Vec3d(0.1, -9.9, 0.2));
            put("ret Vec3d ", "(0.1, -9.9, 0.2)", phScene.GetGravity());

            // function returns array by using pointer
            CDConvexMeshDesc descMesh = new CDConvexMeshDesc();
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    for (int z = 0; z < 10; z++) {
                        Vec3f v3f = new Vec3f(x, y, z);
                        descMesh.vertices.push_back(new Vec3f(x, y, z));
                    }
                }
            }
            Vec3f[] v3fr = new Vec3f[8];
            v3fr[0] = new Vec3f(0, 0, 0); v3fr[1] = new Vec3f(0, 0, 9);
            v3fr[2] = new Vec3f(0, 9, 0); v3fr[3] = new Vec3f(0, 9, 9);
            v3fr[4] = new Vec3f(9, 0, 0); v3fr[5] = new Vec3f(9, 0, 9);
            v3fr[6] = new Vec3f(9, 9, 0); v3fr[7] = new Vec3f(9, 9, 9);
        }

        static void test_operator() {
            test_name("operator");

            // TVector
            Vec3f v3a = new Vec3f(0.1f, 0.2f, 0.3f);
            Vec3f v3b = new Vec3f(0.4f, 0.5f, 0.6f);
            Vec3f v3c = new Vec3f(0.1f, 0.2f, 0.3f);	// v3c == v3a
            Vec3f v3d;
            put("vector unary  -", "(-0.1, -0.2, -0.3)", edit_vector(-v3a));
            put("vector binary +", "(0.5, 0.7, 0.9)", edit_vector(v3a + v3b));
            put("vector binary -", "(-0.3, -0.3, -0.3)", edit_vector(v3a - v3b));
            put("vector binary *", "(0.2, 0.4, 0.6)", edit_vector(v3a * 2));
            put("vector binary *", "(0.8, 1.0, 1.2)", edit_vector(2 * v3b));
            put("vector binary /", "(0.05, 0.10, 0.15)", edit_vector(v3a / 2, 2));
            put("vector binary *", "  0.32", v3a * v3b);
            put("vector binary %", "(-0.03, 0.06, -0.03)", edit_vector(v3a % v3b, 2));
            put("vector binary ^", "(-0.03, 0.06, -0.03)", edit_vector(v3a ^ v3b, 2));
            v3d = v3a; v3d += v3b; put("vector binary +=", "(0.5, 0.7, 0.9)", edit_vector(v3d));
            v3d = v3a; v3d -= v3b; put("vector binary -=", "(-0.3, -0.3, -0.3)", edit_vector(v3d));
            v3d = v3a; v3d *= 2;   put("vector binary *=", "(0.2, 0.4, 0.6)", edit_vector(v3d));
            v3d = v3a; v3d /= 2;   put("vector binary /=", "(0.05, 0.10, 0.15)", edit_vector(v3d, 2));
            put("vector binary ==", "True ", v3a == v3c);
            put("vector binary ==", "False", v3a == v3b);
            put("vector binary !=", "True ", v3a != v3b);
            put("vector binary !=", "False", v3a != v3c);

            // TMatrix
            Matrix3f m3a = new Matrix3f(0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f);
            Matrix3f m3b = new Matrix3f(1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f);
            Matrix3f m3c = new Matrix3f(0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f);	// m3c == m3a
            Matrix3f m3d;
            Matrix3f m3r = new Matrix3f(-0.1f, -0.2f, -0.3f, -0.4f, -0.5f, -0.6f, -0.7f, -0.8f, -0.9f);
            Matrix3f m3s = new Matrix3f(1.2f, 1.4f, 1.6f, 1.8f, 2.0f, 2.2f, 2.4f, 2.6f, 2.8f);
            Matrix3f m3t = new Matrix3f(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f);
            Matrix3f m3u = new Matrix3f(0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f, 1.8f);
            put2("matrix unary  -", edit_matrix(m3r), edit_matrix(-m3a));
            put2("matrix binary +", edit_matrix(m3s), edit_matrix(m3a + m3b));
            put2("matrix binary -", edit_matrix(m3t), edit_matrix(m3a - m3b));
            put2("matrix binary *", edit_matrix(m3u), edit_matrix(m3a * 2));
            put2("matrix binary *", edit_matrix(m3u), edit_matrix(2 * m3a));
            put2("matrix binary *", new Vec3d( 0.14, 0.32, 0.50 ), (m3a * v3a));
            put2("matrix binary *", new Vec3d( 2.16, 2.31, 2.46 ), (v3b * m3b));
            m3d = m3a; m3d += m3b; put2("matrix binary +=", edit_matrix(m3s), edit_matrix(m3d));
            m3d = m3a; m3d -= m3b; put2("matrix binary +=", edit_matrix(m3t), edit_matrix(m3d));
            m3d = m3a; m3d *= 2;   put2("matrix binary +=", edit_matrix(m3u), edit_matrix(m3d));

	    // TQuaternion
            Quaternionf q1 = new Quaternionf(1.0f, 2.0f, 3.0f, 4.0f);
            Quaternionf q2 = new Quaternionf(5.0f, 6.0f, 7.0f, 8.0f);
            Vec3f qv1 = new Vec3f(1.0f, 2.0f, 3.0f);
            Matrix3f qm1 = new Matrix3f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
            Quaternionf qr = new Quaternionf(-60.0f, 12.0f, 30.0f, 24.0f);
            Vec3f qvs = new Vec3f(54f, 60f, 78f);
            Matrix3f qmt = new Matrix3f(150f, 156f, 162f, 120f, 150f, 180, 150f, 192f, 234f);
            put2("quaternion binary *", edit_quaternion(qr), edit_quaternion(q1 * q2));
            put2("quaternion binary *", edit_vector(qvs), edit_vector(q1 * qv1));
            put2("quaternion binary *", edit_matrix(qmt), edit_matrix(q1 * qm1));

	    // TPose
            Posef pp1 = new Posef(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
            Posef pp2 = new Posef(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            Vec3f pv1 = new Vec3f(1.0f, 2.0f, 3.0f);
            Posef pr1 = new Posef(-36.0f, 12.0f, 42.0f, 24.0f, -25.0f, 66.0f, 97.0f);
            put2("pose binary *", edit_pose(pr1), edit_pose(pp1 * pp2));
            put2("pose binary *", "(59.0, 66.0, 85.0)", edit_vector((pp1 * pv1)));

	    // indexing
            System.Console.WriteLine("");
            Vec3f v31 = new Vec3f(0.1f, 0.2f, 0.3f);
            Vec3f v32 = new Vec3f(1.0f, 1.0f, 1.0f);
            Vec3f v3e = v31 + v32;
            for (int i = 0; i < 3; i++) { v31[i] += v32[i]; }
            put("indexing []", edit_vector(v3e), edit_vector(v31));

            Matrix3f m31 = new Matrix3f(0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f);
            Matrix3f m32 = new Matrix3f(1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 3.0f, 3.0f, 3.0f);
            Matrix3f m3e = m31 + m32;
            put("indexing [] ref", "(0.1, 0.2, 0.3)", edit_vector(m31[0]));
            put("            ref", "(0.4, 0.5, 0.6)", edit_vector(m31[1]));
            put("            ref", "(0.7, 0.8, 0.9)", edit_vector(m31[2]));
            for (int i = 0; i < 3; i++) { m31[i] += v32; }
            for (int i = 1; i < 3; i++) { m31[i] += v32; }
            for (int i = 2; i < 3; i++) { m31[i] += v32; }
            put2("indexing [] set", edit_matrix(m3e), edit_matrix(m31));
	}

        static void test_func_call() {
            test_name("func_call");

            // TVector
            Vec3f v31 = new Vec3f(1.0f, 2.0f, 3.0f);
            put("Vec3f: square ", "14.0", string.Format("{0:f1}", v31.square()));
            put("Vec3f: norm   ", "3.741657", string.Format("{0:f6}", v31.norm()));
            Vec3f v3u = v31; v3u.unitize();
            Vec3f v3r = new Vec3f(0.2672612f, 0.5345225f, 0.8017837f);
            put2("Vec3f: unitize", edit_vector(v3r), edit_vector(v3u));

	    // TQuaternion
            System.Console.WriteLine("");
            float pi    = 3.1415927f;
            float rad90 = pi / 2;
            float rad45 = pi / 4;
            float cos45 = 0.7071069f;
            float sqrt3 = 1.7320508f;
            float sqrt3div3 = sqrt3 / 3;
            Quaternionf q1 = new Quaternionf(cos45, sqrt3div3, sqrt3div3, sqrt3div3);
            Quaternionf q2 = new Quaternionf(q1.W(), q1.X(), q1.Y(), q1.Z());
            Vec3f qv1 = new Vec3f(sqrt3div3, sqrt3div3, sqrt3div3);
            put2("Quaternionf: W(),X(),Y(),Z()", edit_quaternion(q1), edit_quaternion(q2));
            put2("Quaternionf: V", edit_vector(qv1), edit_vector(q1.V()));
            put2("Quaternionf: Axis", edit_vector(qv1), edit_vector(q1.Axis()));
            put("Quaternionf: Theta", rad90.ToString(), q1.Theta());
            System.Console.WriteLine("");

            float half = pi / (2 * sqrt3);
            Vec3f qv2 = new Vec3f(half, half, half);
            put2("Quaternionf: RotationHalf", edit_vector(qv2), edit_vector(q1.RotationHalf()));
            put2("Quaternionf: Rotation    ", edit_vector(qv2), edit_vector(q1.Rotation()));

            float angle = rad90;
            float d = sqrt3;
            float s = (float) Math.Sin(angle / 2) / d;
            Quaternionf qr = new Quaternionf((float) Math.Cos(angle / 2), s, s, s);
            Quaternionf q3 = Quaternionf.Rot(angle, new Vec3f(1f, 1f, 1f));
            put2("Quaternionf: Rot", edit_quaternion(qr), edit_quaternion(q3));

            float c1 = (float) Math.Cos(angle / 2);
            float s1 = (float) Math.Sin(angle / 2);
            Quaternionf qs = new Quaternionf(c1, s1, 0f, 0f);
            Quaternionf q4 = Quaternionf.Rot(angle, (sbyte) 'x');
            put2("Quaternionf: Rot", edit_quaternion(qs), edit_quaternion(q4));

            Vec3f qv3 = new Vec3f(1f, 1f, 1f);
            float c2 = (float) Math.Cos(sqrt3 / 2);
            float s2 = (float) Math.Sin(sqrt3 / 2);
            Vec3f qv4 = (s2 / sqrt3) * qv3;
            Quaternionf qt = new Quaternionf(c2, qv4[0], qv4[1], qv4[2]);
            Quaternionf q5 = Quaternionf.Rot(qv3);
            put2("Quaternionf: Rot", edit_quaternion(qt), edit_quaternion(q5));

            Quaternionf qc1 = new Quaternionf(cos45,  sqrt3div3,  sqrt3div3,  sqrt3div3);
            Quaternionf qc2 = new Quaternionf(cos45,  sqrt3div3,  sqrt3div3,  sqrt3div3);
            Quaternionf qc = new Quaternionf(cos45, -sqrt3div3, -sqrt3div3, -sqrt3div3);
            qc1.Conjugate();
            put2("Quaternionf: Conjugate", edit_quaternion(qc), edit_quaternion(qc1));
            put2("Quaternionf: Conjugated", edit_quaternion(qc), edit_quaternion(qc2.Conjugated()));

            float qf1 = (float) (cos45*cos45 + 3*sqrt3div3*sqrt3div3);
            Quaternionf qe = qc2.Conjugated() / qf1;
            put2("Quaternionf: Inv", edit_quaternion(qe), edit_quaternion(qc2.Inv()));

            // TPose
            System.Console.WriteLine("");
            Posef p1 = new Posef(cos45, sqrt3, sqrt3, sqrt3, 1f, 2f, 3f);
            Posef p2 = new Posef(p1.W(), p1.X(), p1.Y(), p1.Z(), p1.Px(), p1.Py(), p1.Pz());
            Vec3f pv31 = new Vec3f(1f, 2f, 3f);
            Quaternionf pq1 = new Quaternionf(cos45, sqrt3, sqrt3, sqrt3);
            Quaternionf pq2 = new Quaternionf();
            Vec3f pv32 = new Vec3f();
            Posef pp1 = new Posef(pq2.w, pq2.x, pq2.y, pq2.z, pv32.x, pv32.y, pv32.z);
            Posef pp2 = new Posef(pq2.w, pq2.x, pq2.y, pq2.z, 1f, 2f, 3f);
            put2("Posef: W(),X(),Y(),Z(),Px(),Py(),Pz()", edit_pose(p1), edit_pose(p2));
            put2("Posef: Pos()", edit_vector(pv31), edit_vector(p1.Pos()));
            put2("Posef: Ori()", edit_quaternion(pq1), edit_quaternion(p1.Ori()));
            put2("Posef: Unit ", edit_pose(pp1), edit_pose(Posef.Unit()));
            put2("Posef: Trn  ", edit_pose(pp2), edit_pose(Posef.Trn(1f, 2f, 3f)));
            put2("Posef: Trn  ", edit_pose(pp2), edit_pose(Posef.Trn(pv31)));
	}

        static void test_simulation() {
            test_name("physical simulation");

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

            PHBallJointDesc descJoint = new PHBallJointDesc();
            PHBallJointIf j = phScene.CreateJoint(phFloor, phSolid, PHBallJointIf.GetIfInfoStatic(), descJoint).Cast();
            System.Console.WriteLine(j.GetName());

            PHIKBallActuatorDescStruct s = new PHIKBallActuatorDesc();
            put("bias", "something", s.bias);

            /**/
            for (int i = 0; i < 200; i++) {
                phScene.Step();
                //System.Console.WriteLine(i.ToString() + " : " + phSolid.GetPose());
                System.Console.WriteLine(String.Format("{0, 3}", i) + " : " + phSolid.GetPose());
            }
            /**/
        }

	// ------------------
	//  helper functions
	// ------------------
        static bool check_test(string chk) {
            return inc.Contains(chk) && !exc.Contains(chk);
        }
	static void test_name(string test_name) {
            System.Console.WriteLine("\n---[ " + test_name + " ]---");
	}
	static void put(string s = "") { System.Console.WriteLine(s); }

	static void put(string title, object expected, object result) {
            //System.Console.Write(result.GetType()+" ");
            switch (result) {
                case SprCs.Vec3f v3f:
                    result = edit_vector(v3f);
                    break;
                case SprCs.Vec3d v3d:
                    result = edit_vector(v3d);
                    break;
                case SprCs.Vec4f v4f:
                    result = edit_vector(v4f);
                    break;
                case SprCs.Matrix3f m3f:
                    result = edit_matrix(m3f);
                    break;
                default:
                    result = result.ToString().Trim();
                    break;
            }
            string exp = expected.ToString().Trim();
            string res = result.ToString().Trim();
            res = res.Replace("\0", "\\0");
            string judge = exp == res ? "OK" : "--";
            System.Console.Write("[" + judge + "] ");
            string line = title + ": expected: " + exp + ", result: " + res;
            System.Console.WriteLine(line);
            if (judge == "OK") OK++;
            if (judge != "OK") NG++;
        }
        static void put2(string title, object expected, object result) {
            //System.Console.Write(result.GetType()+" ");
            string exp = expected.ToString().Trim();
            string res = result.ToString().Trim();
            string judge = exp == res ? "OK" : "--";
            System.Console.Write("[" + judge + "] ");
            string line = title + ":\n"
                                + "     expected: " + exp + ",\n"
                                + "     result:   " + res;
            System.Console.WriteLine(line);
            if (judge == "OK") OK++;
            if (judge != "OK") NG++;
        }
        static void put_title(string title) {
            System.Console.WriteLine(title + ":");
        }
        static void put_indent(int indent, object obj) {
            string str;
            if (obj.GetType() == typeof(string)) {
                str = (string) obj;
            }
            else {
                str = obj.ToString();
            }
            char[] seps = { '\n' };
            string[] lines = str.Split(seps);
            for (int i = 0; i < lines.Length; i++ ) {
                if (lines[i] == "") continue;
                System.Console.WriteLine("        ".Substring(0,indent) + lines[i]);
            }
        }
	//
	static string edit_vector(Vec3i v) {
            return string.Format("({0}, {1}, {2})", v.x, v.y, v.z);
        }
	static string edit_vector(Vec3f v) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", v.x, v.y, v.z);
        }
	static string edit_vector(Vec3d v, int fraction=1) {
            string s;
            if (fraction == 2)
                s = string.Format("({0:f2}, {1:f2}, {2:f2})", v.x, v.y, v.z);
            else
                s = string.Format("({0:f1}, {1:f1}, {2:f1})", v.x, v.y, v.z);
            return s;
        }
	static string edit_vector(Vec4f v) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1})", v.w, v.x, v.y, v.z);
        }
	static string edit_matrix(Matrix3f m) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", m.xx, m.yx, m.zx) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xy, m.yy, m.zy) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xz, m.yz, m.zz);
	}
	static string edit_matrix(Matrix3d m) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", m.xx, m.yx, m.zx) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xy, m.yy, m.zy) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xz, m.yz, m.zz);
	}
	static string edit_quaternion(Quaternionf q) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1})", q.w, q.x, q.y, q.z);
	}
	static string edit_pose(Posef p) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1}, {4:f1}), {5:f1}, {6:f1}",
                                 p.w, p.x, p.y, p.z, p.px, p.py, p.pz);
	}

	static string edit_vectorwrapper(vectorwrapper_Vec3d v, int ix) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", v[ix].x, v[ix].y, v[ix].z);
	}
	static string edit_vectorwrapper(vectorwrapper_int v, int ix) {
	    return v[ix].ToString();
	}
/***
	static string edit_arraywrapper(arraywrapper_Vec3d a, int ix) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", a[ix].x, a[ix].y, a[ix].z);
	}
***/
	static string edit_arraywrapper(arraywrapper_int a, int ix) {
	    return a[ix].ToString();
	}
    }
}
