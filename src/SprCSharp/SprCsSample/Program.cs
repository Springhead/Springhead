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
            if (check_test("o"))  test_operator();
            if (check_test("F"))  test_func_call();
            if (check_test("s"))  test_simulation();
        }

        static bool check_test(string chk) {
            return inc.Contains(chk) && !exc.Contains(chk);
        }

	static void title(string title) {
            System.Console.WriteLine("\n---[ " + title + " ]---");
	}

	static void put(string title, string expected, object obj) {
	    string line = title + ": expected: " + expected + ", result: " + obj;
	    System.Console.WriteLine(line);
	}

        static void test_intrinsic() {
            title("intrinsic");

            // intrinsic member
            PHSceneDesc descScene = new PHSceneDesc();
            PHRaycastHit raycastHit = new PHRaycastHit();
            GRVertexElement vertexelm = new GRVertexElement();
            // simple
            vertexelm.offset = 123;              put("short ", "123  ", vertexelm.offset);
            descScene.numIteration = 123;        put("int   ", "123  ", descScene.numIteration);
            descScene.bCCDEnabled = true;        put("bool  ", "True ", descScene.bCCDEnabled);
            raycastHit.distance = 0.123F;        put("float ", "0.123", raycastHit.distance);
            descScene.airResistanceRate = 0.123; put("double", "0.123", descScene.airResistanceRate);
            // nested
            descScene.gravity.x = 0;
            descScene.gravity.y = 0;
            descScene.gravity.z = -4.5;
/**/        put("set by elm", "(0.0, 0.0, -4.5)", descScene.gravity);
            descScene.gravity = new Vec3d(2.5, -5.2, 0.5);
/**/        put("set struct", "(2.5, -5.2, 0.5)", descScene.gravity);
        }

        static void test_tostring() {
            title("ToString");

            Vec3d v3d = new Vec3d(0.1, 0.2, 0.3);
            //string s = v3d.ToString();
/**/        put("ToString", "(0.1, 0.2, 0.3)", v3d.ToString());
/**/        System.Console.WriteLine("ToString: implicit ToString call           : " + v3d);

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
            title("vector");

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
            put("vec<Vec3d>", "(0.4, 0.5, 0.6)", vertices[1].ToString());
        }

        static void test_array() {
            title("array");

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

            CDConvexMeshDesc descMesh = new CDConvexMeshDesc();
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    for (int z = 0; z < 10; z++) {
                        descMesh.vertices.push_back(new Vec3f(x, y, z));
                    }
                }
            }
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            CDConvexMeshIf mesh = phSdk.CreateShape(CDConvexMeshIf.GetIfInfoStatic(), descMesh) as CDConvexMeshIf;
            mesh.GetVertices();
        }

        static void test_type_conv() {
            title("type conversion");

            string msg_f3 = "f2d: (1, 2, 3)";
            string msg_d3 = "d2f: (4, 5, 6)";

            Vec3f f3 = new Vec3f(1, 2, 3);
            Vec3d d3 = new Vec3d(4, 5, 6);
            put("type_conv", msg_f3, f3);
//            print_vec3f(d3, msg_f3);    // This cause CS1502 and CS1503 compile error. <- OK
            put("type_conv", msg_d3, (Vec3f) d3);

            Vec3fStruct f3s = f3;
            Vec3dStruct d3s = d3;
            put("type_conv", msg_f3, "(" + f3s.x + ", " + f3s.y + ", " + f3s.z + ")");
//            print_vec3fs(d3s, msg_d3);  // This cause CS1502 and CS1503 compile error. <- OK
            put("type_conv", msg_d3,
                "(" + ((Vec3fStruct)d3s).x + ", " + ((Vec3fStruct)d3s).y + ", " + ((Vec3fStruct)d3s).z + ")");

            // -----
            // 戻り値の自動型変換のテスト
            PHSdkIf phSdk = PHSdkIf.CreateSdk();
            CDBoxDesc descBox = new CDBoxDesc();
            descBox.boxsize = new Vec3f(1,4,9);
            CDShapeIf shape = phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox);
            // ↑ CreateShapeはCDBoxIfオブジェクトを返す。それをCDShapeIfで受ける。

            // CDShapeIf型の変数に格納されているが中身はCDBoxIfなので型変換可能。
            CDBoxIf box = shape as CDBoxIf;
            put("type_conv", "(1, 4, 9)", box.GetBoxSize().ToString());

            // CDBoxIf is not a CDSphereIf なので nullになることが期待される。
            CDSphereIf sphere = shape as CDSphereIf;
            if (sphere == null) {
                put("type_conv", "null", "null");
            } else {
                put("type_conv", "null", sphere.ToString());
            }
            
            // CDBoxIf is a CDConvexIf なのでnullにはならず型変換される。
            CDConvexIf convex = shape as CDConvexIf;
            if (convex == null) {
                put("type_conv", "not null", "null");
            } else {
                put("type_conv", "not null", convex.ToString());
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
            put("by typeconv:    gravity ", show_vector_0(v3d1), show_vector_0(structScene1.gravity));
            put("by typeconv:    timeStep", "0.005", structScene1.timeStep);
            // constructor による
            PHSceneDescStruct structScene2 = new PHSceneDescStruct();
            put("by constructor: gravity ", show_vector_0(v3d1), show_vector_0(structScene2.gravity));
            put("by constructor: timeStep", "0.005", structScene2.timeStep);
            // ApplyFrom による
            PHSceneDescStruct structScene3 = new PHSceneDescStruct();
            structScene3.gravity = v3d2;
            structScene3.timeStep = 1.001;
            put("by Apply(): fm: gravity ", show_vector_0(v3d2), show_vector_0(structScene3.gravity));
            put("by Apply(): fm: timeStep", "1.001", structScene3.timeStep);
            structScene2.ApplyFrom((PHSceneDesc) structScene3);
            put("by Apply(): to: gravity ", show_vector_0(v3d2), show_vector_0(structScene2.gravity));
            put("by Apply(): to: timeStep", "0.005", structScene2.timeStep);
	    //
	    // ---- Desc を継承 ----
            CDSphereDescStruct structSphere1 = new CDSphereDescStruct();
            CDSphereDesc descSphere1 = structSphere1;
            put("DescStruct: radius          ", "1", structSphere1.radius);
            put("Desc:       material.density", "1", descSphere1.material.density);
            CDSphereDescStruct structSphere2 = new CDSphereDescStruct();
            CDSphereDesc descSphere2 = structSphere2;
            structSphere2.radius = 2;
            descSphere2.material.density = 2;
            put("DescStruct: radius          ", "2", structSphere2.radius);
            put("Desc:       material.density", "2", descSphere2.material.density);
            structSphere1.ApplyFrom(descSphere2);
            descSphere1 = structSphere1;
            put("DescStruct: radius          ", "2", structSphere1.radius);
            put("Desc:       material.density", "1", descSphere1.material.density);
        }

        static void test_func_args() {
            title("function arguments");

            // ここでのコードは正常には動作はしない － 例外が起きて停止する。
            // デバッガで止めて値を確認すること。
            //
            // [int]
            CDShapePairIf shapePairIf = new CDShapePairIf();
            shapePairIf.GetShape(123);
        }

        static void test_func_return() {
            title("function return");
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

            put("ret int   ", "3    ", phScene.NSolids());
            for (int i = 0; i < 20; i++) {
                phScene.Step();
            }
            put("ret Uint  ", "20   ", phScene.GetCount());
            phHaptic.SetLocalRange(2.345f);
            put("ret float ", "2.345", phHaptic.GetLocalRange());
            put("ret double", "0.005", phScene.GetTimeStep());
            phScene.SetMaxVelocity(1.23);
            put("ret double", "0.123", phScene.GetMaxVelocity());
            phScene.EnableContactDetection(false);
            put("ret bool  ", "False", phScene.IsContactDetectionEnabled());
            phScene.EnableContactDetection(true);
            put("ret bool  ", "True ", phScene.IsContactDetectionEnabled());

            put("ret size_t", "152? ", phScene.GetDescSize());

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
            //PHSdkIf phSdk = PHSdkIf.CreateSdk();
            CDConvexMeshIf mesh = phSdk.CreateShape(CDConvexMeshIf.GetIfInfoStatic(), descMesh) as CDConvexMeshIf;
            mesh.GetVertices();
            arraywrapper_Vec3f vertices = mesh.GetVertices();
            Vec3f[] v3fr = new Vec3f[8];
            v3fr[0] = new Vec3f(0, 0, 0); v3fr[1] = new Vec3f(0, 0, 9);
            v3fr[2] = new Vec3f(0, 9, 0); v3fr[3] = new Vec3f(0, 9, 9);
            v3fr[4] = new Vec3f(9, 0, 0); v3fr[5] = new Vec3f(9, 0, 9);
            v3fr[6] = new Vec3f(9, 9, 0); v3fr[7] = new Vec3f(9, 9, 9);
            for (int i = 0; i< 8; i++) {
                put("ret_array", show_vector_0(v3fr[i]), vertices[i]);
            }
        }

        static void test_operator() {
            title("operator");

            // TVector
            Vec3f v3a = new Vec3f(0.1f, 0.2f, 0.3f);
            Vec3f v3b = new Vec3f(0.4f, 0.5f, 0.6f);
            Vec3f v3c = new Vec3f(0.1f, 0.2f, 0.3f);	// v3c == v3a
            Vec3f v3d;
            put("vector unary  -", "(-0.1, -0.2, -0.3)", -v3a);
            put("vector binary +", "( 0.5,  0.7,  0.9)", v3a + v3b);
            put("vector binary -", "(-0.3, -0.3, -0.3)", v3a - v3b);
            put("vector binary *", "( 0.2,  0.4,  0.6)", v3a * 2);
            put("vector binary *", "( 0.8,  1.0,  1.2)", 2 * v3b);
            put("vector binary /", "( 0.05, 0.10,  0.15)", v3a / 2);
            put("vector binary *", "  0.32", v3a * v3b);
            put("vector binary %", "(-0.03, 0.06, -0.03)", v3a % v3b);
            put("vector binary ^", "(-0.03, 0.06, -0.03)", v3a ^ v3b);
            v3d = v3a; v3d += v3b; put("vector binary +=", "( 0.5,  0.7,  0.9)", v3d);
            v3d = v3a; v3d -= v3b; put("vector binary -=", "(-0.3, -0.3, -0.3)", v3d);
            v3d = v3a; v3d *= 2;   put("vector binary *=", "( 0.2,  0.4,  0.6)", v3d);
            v3d = v3a; v3d /= 2;   put("vector binary /=", "(0.05, 0.10, 0.15)", v3d);
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
            put("matrix unary  -", show_matrix(m3r), show_matrix2(-m3a));
            put("matrix binary +", show_matrix(m3s), show_matrix2(m3a + m3b));
            put("matrix binary -", show_matrix(m3t), show_matrix2(m3a - m3b));
            put("matrix binary *", show_matrix(m3u), show_matrix2(m3a * 2));
            put("matrix binary *", show_matrix(m3u), show_matrix2(2 * m3a));
            put("matrix binary *", "( 0.14, 0.32, 0.50 )", (m3a * v3a));
            put("matrix binary *", "( 2.16, 2.31, 2.46 )", (v3b * m3b));
            m3d = m3a; m3d += m3b; put("matrix binary +=", show_matrix(m3s), show_matrix(m3d));
            m3d = m3a; m3d -= m3b; put("matrix binary +=", show_matrix(m3t), show_matrix(m3d));
            m3d = m3a; m3d *= 2;   put("matrix binary +=", show_matrix(m3u), show_matrix(m3d));

	    // TQuaternion
            Quaternionf q1 = new Quaternionf(1.0f, 2.0f, 3.0f, 4.0f);
            Quaternionf q2 = new Quaternionf(5.0f, 6.0f, 7.0f, 8.0f);
            Vec3f qv1 = new Vec3f(1.0f, 2.0f, 3.0f);
            Matrix3f qm1 = new Matrix3f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
            Quaternionf qr = new Quaternionf(-60.0f, 12.0f, 30.0f, 24.0f);
            Vec3f qvs = new Vec3f(54f, 60f, 78f);
            Matrix3f qmt = new Matrix3f(150f, 156f, 162f, 120f, 150f, 180, 150f, 192f, 234f);
            put("quaternion binary *", show_quaternion(qr), show_quaternion(q1 * q2));
            put("quaternion binary *", show_vector_0(qvs), show_vector_0(q1 * qv1));
            put("quaternion binary *", show_matrix(qmt), show_matrix(q1 * qm1));

	    // TPose
            Posef pp1 = new Posef(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
            Posef pp2 = new Posef(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            Vec3f pv1 = new Vec3f(1.0f, 2.0f, 3.0f);
            Posef pr1 = new Posef(-36.0f, 12.0f, 42.0f, 24.0f, -25.0f, 66.0f, 97.0f);
            put("pose binary *", show_pose(pr1), show_pose(pp1 * pp2));
            put("pose binary *", "(59, 66, 85)", (pp1 * pv1));

	    // indexing
            System.Console.WriteLine("");
            Vec3f v31 = new Vec3f(0.1f, 0.2f, 0.3f);
            Vec3f v32 = new Vec3f(1.0f, 1.0f, 1.0f);
            Vec3f v3e = v31 + v32;
            for (int i = 0; i < 3; i++) { v31[i] += v32[i]; }
            put("indexing []", show_vector_0(v3e), show_vector_0(v31));

            Matrix3f m31 = new Matrix3f(0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f);
            Matrix3f m32 = new Matrix3f(1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 3.0f, 3.0f, 3.0f);
            Matrix3f m3e = m31 + m32;
            put("indexing [] ref", "(0.1, 0.2, 0.3)", show_vector_0(m31[0]));
            put("            ref", "(0.4, 0.5, 0.6)", show_vector_0(m31[1]));
            put("            ref", "(0.7, 0.8, 0.9)", show_vector_0(m31[2]));
            for (int i = 0; i < 3; i++) { m31[i] += v32; }
            for (int i = 1; i < 3; i++) { m31[i] += v32; }
            for (int i = 2; i < 3; i++) { m31[i] += v32; }
            put("indexing [] set", show_matrix(m3e), show_matrix(m31));
	}

	static string show_vector(Vec3f v) {
            string str = "\n" + show_vector_0(v) + "\n";
            return str;
	}
	static string show_vector(Vec3d v) {
            string str = "\n" + show_vector_0(v) + "\n";
            return str;
	}
	static string show_vector_0(Vec3f v) {
            string str = "( " + v.x + ",  " + v.y + ",  " + v.z + " )";
            return str;
	}
	static string show_vector_0(Vec3d v) {
            string str = "( " + v.x + ",  " + v.y + ",  " + v.z + " )";
            return str;
	}
	static string show_matrix(Matrix3f m) {
            string str = "\n"
                       + "(( " + m.xx + ",  " + m.yx + ",  " + m.zx + " ) "
                       + " ( " + m.xy + ",  " + m.yy + ",  " + m.zy + " ) "
                       + " ( " + m.xz + ",  " + m.yz + ",  " + m.zz + " ))\n";
            return str;
	}
	static string show_matrix(Matrix3d m) {
            string str = "\n"
                       + "(( " + m.xx + ",  " + m.yx + ",  " + m.zx + " ) "
                       + " ( " + m.xy + ",  " + m.yy + ",  " + m.zy + " ) "
                       + " ( " + m.xz + ",  " + m.yz + ",  " + m.zz + " ))\n";
            return str;
	}
	static string show_quaternion(Quaternionf q) {
            string str = "\n"
                       + "( " + q.w + ",  " + q.x + ",  " + q.y + ",  " +  q.z + " )\n";
            return str;
	}
	static string show_pose(Posef p) {
            string str = "\n"
                       + "( " + p.w + ", " + p.x + ", " + p.y + ", " + p.z
                       + ", " + p.px + ", " + p.py + ", " + p.pz + " )\n";
            return str;
	}
	static string show_matrix2(Matrix3f m) {
            string str = "\n" + m;
            return str;
	}

        static void test_func_call() {
            title("func_call");

            // TVector
            Vec3f v31 = new Vec3f(1.0f, 2.0f, 3.0f);
            put("Vec3f: square ", "14.0", v31.square());
            put("Vec3f: norm   ", "3.7416574", v31.norm());
            Vec3f v3u = v31; v3u.unitize();
            Vec3f v3r = new Vec3f(0.2672612f, 0.5345225f, 0.8017837f);
            put("Vec3f: unitize", "\n          " + show_vector_0(v3r) + "\n", show_vector_0(v3u));

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
            put("Quaternionf: W(),X(),Y(),Z()", show_quaternion(q1), show_quaternion(q2));
            put("Quaternionf: V", show_vector(qv1), show_vector(q1.V()));
            put("Quaternionf: Axis", show_vector(qv1), show_vector(q1.Axis()));
            put("Quaternionf: Theta", rad90.ToString(), q1.Theta());
            System.Console.WriteLine("");

            float half = pi / (2 * sqrt3);
            Vec3f qv2 = new Vec3f(half, half, half);
            put("Quaternionf: RotationHalf", show_vector(qv2), show_vector(q1.RotationHalf()));
            put("Quaternionf: Rotation    ", show_vector(qv2), show_vector(q1.Rotation()));

            float angle = rad90;
            float d = sqrt3;
            float s = (float) Math.Sin(angle / 2) / d;
            Quaternionf qr = new Quaternionf((float) Math.Cos(angle / 2), s, s, s);
            Quaternionf q3 = Quaternionf.Rot(angle, new Vec3f(1f, 1f, 1f));
            put("Quaternionf: Rot", show_quaternion(qr), show_quaternion(q3));

            float c1 = (float) Math.Cos(angle / 2);
            float s1 = (float) Math.Sin(angle / 2);
            Quaternionf qs = new Quaternionf(c1, s1, 0f, 0f);
            Quaternionf q4 = Quaternionf.Rot(angle, (sbyte) 'x');
            put("Quaternionf: Rot", show_quaternion(qs), show_quaternion(q4));

            Vec3f qv3 = new Vec3f(1f, 1f, 1f);
            float c2 = (float) Math.Cos(sqrt3 / 2);
            float s2 = (float) Math.Sin(sqrt3 / 2);
            Vec3f qv4 = (s2 / sqrt3) * qv3;
            Quaternionf qt = new Quaternionf(c2, qv4[0], qv4[1], qv4[2]);
            Quaternionf q5 = Quaternionf.Rot(qv3);
            put("Quaternionf: Rot", show_quaternion(qt), show_quaternion(q5));

            Quaternionf qt1 = q1; qt1.Conjugate();
            Quaternionf qc = new Quaternionf(cos45, -sqrt3div3, -sqrt3div3, -sqrt3div3);
            //Quaternionf qd = new Quaternionf(q1.Conjugated());
            Quaternionf qd = q1.Conjugated();
            put("Quaternionf: Conjugate", show_quaternion(qc), show_quaternion(qt1));
            put("Quaternionf: Conjugated", show_quaternion(qc), show_quaternion(q1.Conjugated()));

            Quaternionf qt2 = q1; qt2.Inv();
            //float qf1 = (float) Math.Sqrt(cos45*cos45 + 9);
            float qf1 = (float) (cos45*cos45 + 9)*(cos45*cos45 + 9);
            Quaternionf qe = qc / qf1;
            put("Quaternionf: Inv", show_quaternion(qe), show_quaternion(qt2.Inv()));

	    // TPose
            System.Console.WriteLine("");
            Posef p1 = new Posef(cos45, sqrt3, sqrt3, sqrt3, 1f, 2f, 3f);
            Posef p2 = new Posef(p1.W(), p1.X(), p1.Y(), p1.Z(), p1.Px(), p1.Py(), p1.Pz());
            put("Posef: W(),X(),Y(),Z(),Px(),Py(),Pz()", show_pose(p1), show_pose(p2));
            put("Posef: Pos()", show_pose(p1), show_vector(p1.Pos()));
            put("Posef: Ori()", show_pose(p1), show_quaternion(p1.Ori()));
            put("Posef: Unit ", show_pose(p1), show_pose(Posef.Unit()));
            put("Posef: Trn  ", show_pose(p1), show_pose(Posef.Trn(1f, 1f, 1f)));
            put("Posef: Trn  ", show_pose(p1), show_pose(Posef.Trn(new Vec3f(1f, 1f, 1f))));
	}

        static void test_simulation() {
            title("physical simulation");

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

            /*
            for (int i = 0; i < 200; i++) {
                phScene.Step();
                //System.Console.WriteLine(i.ToString() + " : " + phSolid.GetPose());
                System.Console.WriteLine(String.Format("{0, 3}", i) + " : " + phSolid.GetPose());
            }
            */
        }

    }
}
