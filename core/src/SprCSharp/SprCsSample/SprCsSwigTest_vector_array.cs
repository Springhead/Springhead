using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using SprCs;

namespace SprCsSample {
    class Program {
        static string inc = "va";
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
                if (check_test("v"))  test_vector();
                if (check_test("a"))  test_array();
            }
            catch (System.Exception e) {
                System.Console.WriteLine(SEH_Exception.what(e));
            }
            System.Console.WriteLine("----");
            System.Console.WriteLine("Passed: " + OK + ", Failed: " + NG);
        }

        static void test_vector() {
            test_name("vector");

        // Test for instrinsic data type.
            CSTestIntrinsic _intrinsic = new CSTestIntrinsic();

        // int
            System.Console.WriteLine("---- [ int ] ----");
            // by member access
            for (int i = 0; i < 4; i++) {
                _intrinsic.vec_int.push_back(11 * (i+1));
            }
            put("vector<int>: by member", "4", _intrinsic.vec_int.size());
            put("vector<int>: by member", "11", _intrinsic.vec_int[0]);
            put("vector<int>: by member", "22", _intrinsic.vec_int[1]);
            put("vector<int>: by member", "33", _intrinsic.vec_int[2]);
            put("vector<int>: by member", "44", _intrinsic.vec_int[3]);
            // from List
            List<int> vec_int_list = new List<int>(3);
            for (int i = 0; i < 3; i++) {
                vec_int_list.Add(111 * (i+1));
            }
            _intrinsic.vec_int.clear();
            _intrinsic.vec_int = vec_int_list;
            put("vector<int>: from List ", "3", _intrinsic.vec_int.size());
            put("vector<int>: from List ", "111", _intrinsic.vec_int[0]);
            put("vector<int>: from List ", "222", _intrinsic.vec_int[1]);
            put("vector<int>: from List ", "333", _intrinsic.vec_int[2]);
            // to List
            List<int> vec_int_list2;
            vec_int_list2 = _intrinsic.vec_int;
            put("vector<int>: to List   ", "3", vec_int_list2.Count);
            put("vector<int>: to List   ", "111", vec_int_list2[0]);
            put("vector<int>: to List   ", "222", vec_int_list2[1]);
            put("vector<int>: to List   ", "333", vec_int_list2[2]);
            // from array
            int[] vec_int_array = new int[] { 4444, 5555 };
            _intrinsic.vec_int.clear();
            _intrinsic.vec_int = vec_int_array;
            put("vector<int>: from array", "2", _intrinsic.vec_int.size());
            put("vector<int>: from array", "4444", _intrinsic.vec_int[0]);
            put("vector<int>: from array", "5555", _intrinsic.vec_int[1]);
            // to array
            int[] vec_int_array2;
            vec_int_array2 = _intrinsic.vec_int;
            put("vector<int>: to array  ", "2", vec_int_array2.Length);
            put("vector<int>: to array  ", "4444", vec_int_array2[0]);
            put("vector<int>: to array  ", "5555", vec_int_array2[1]);

	//  同じクラス/構造体の中に同じデータ型のvectorが複数あったときに区別が
	//  できるかのテスト
        // int2
            System.Console.WriteLine("---- [ int2 ] ----");
            // by member access
            for (int i = 0; i < 4; i++) {
                _intrinsic.vec_int2.push_back(11 * (i+5));
            }
            put("vector<int>: by member", "4", _intrinsic.vec_int2.size());
            put("vector<int>: by member", "55", _intrinsic.vec_int2[0]);
            put("vector<int>: by member", "66", _intrinsic.vec_int2[1]);
            put("vector<int>: by member", "77", _intrinsic.vec_int2[2]);
            put("vector<int>: by member", "88", _intrinsic.vec_int2[3]);
            // from List
            List<int> vec_int2_list = new List<int>(3);
            for (int i = 0; i < 3; i++) {
                vec_int2_list.Add(111 * (i+5));
            }
            _intrinsic.vec_int2.clear();
            _intrinsic.vec_int2 = vec_int2_list;
            put("vector<int>: from List ", "3", _intrinsic.vec_int2.size());
            put("vector<int>: from List ", "555", _intrinsic.vec_int2[0]);
            put("vector<int>: from List ", "666", _intrinsic.vec_int2[1]);
            put("vector<int>: from List ", "777", _intrinsic.vec_int2[2]);
            // to List
            List<int> vec_int2_list2;
            vec_int2_list2 = _intrinsic.vec_int2;
            put("vector<int>: to List   ", "3", vec_int2_list2.Count);
            put("vector<int>: to List   ", "555", vec_int2_list2[0]);
            put("vector<int>: to List   ", "666", vec_int2_list2[1]);
            put("vector<int>: to List   ", "777", vec_int2_list2[2]);
            // from array
            int[] vec_int2_array = new int[] { 5555, 6666 };
            _intrinsic.vec_int2.clear();
            _intrinsic.vec_int2 = vec_int2_array;
            put("vector<int>: from array", "2", _intrinsic.vec_int2.size());
            put("vector<int>: from array", "5555", _intrinsic.vec_int2[0]);
            put("vector<int>: from array", "6666", _intrinsic.vec_int2[1]);
            // to array
            int[] vec_int2_array2;
            vec_int2_array2 = _intrinsic.vec_int2;
            put("vector<int>: to array  ", "2", vec_int2_array2.Length);
            put("vector<int>: to array  ", "5555", vec_int2_array2[0]);
            put("vector<int>: to array  ", "6666", vec_int2_array2[1]);

            System.Console.WriteLine("---- [ verify ] ----");
            put("vector<int>: to array  ", "2", vec_int2_array2.Length);
            put("vector<int>: to array  ", "4444", vec_int_array2[0]);
            put("vector<int>: to array  ", "5555", vec_int_array2[1]);

        // unsigned int
            System.Console.WriteLine("---- [ unsigned int ] ----");
            // by member access
            _intrinsic.vec_unsigned_int.push_back(111);
            put("vector<unsigned int>: by member", "1", _intrinsic.vec_unsigned_int.size());
            put("vector<unsigned int>: by member", "111", _intrinsic.vec_unsigned_int[0]);
            // from List
            List<uint> vec_unsigned_int_list = new List<uint>(1) { 222 };
            _intrinsic.vec_unsigned_int.clear();
            _intrinsic.vec_unsigned_int = vec_unsigned_int_list;
            put("vector<unsigned int>: from List ", "1", _intrinsic.vec_unsigned_int.size());
            put("vector<unsigned int>: from List ", "222", _intrinsic.vec_unsigned_int[0]);
            // to List
            List<uint> vec_unsigned_int_list2;
            vec_unsigned_int_list2 = _intrinsic.vec_unsigned_int;
            put("vector<unsigned int>: to List   ", "1", vec_unsigned_int_list2.Count);
            put("vector<unsigned int>: to List   ", "222", vec_unsigned_int_list2[0]);
            // from array
            uint[] vec_unsigned_int_array = new uint[] { 333 };
            _intrinsic.vec_unsigned_int.clear();
            _intrinsic.vec_unsigned_int = vec_unsigned_int_array;
            put("vector<unsigned int>: from array", "1", _intrinsic.vec_unsigned_int.size());
            put("vector<unsigned int>: from array", "333", _intrinsic.vec_unsigned_int[0]);
            // to array
            uint[] vec_unsigned_int_array2;
            vec_unsigned_int_array2 = _intrinsic.vec_unsigned_int;
            put("vector<unsigned int>: to array  ", "1", vec_unsigned_int_array2.Length);
            put("vector<unsigned int>: to array  ", "333", vec_unsigned_int_array2[0]);

        // size_t
            System.Console.WriteLine("---- [ size_t ] ----");
            // by member access
            _intrinsic.vec_size_t.push_back(111);
            put("vector<sise_t>: by member", "1", _intrinsic.vec_size_t.size());
            put("vector<sise_t>: by member", "111", _intrinsic.vec_size_t[0]);
            // from List
            List<ulong> vec_size_t_list = new List<ulong>(1) { 222 };
            _intrinsic.vec_size_t.clear();
            _intrinsic.vec_size_t = vec_size_t_list;
            put("vector<sise_t>: from List ", "1", _intrinsic.vec_size_t.size());
            put("vector<sise_t>: from List ", "222", _intrinsic.vec_size_t[0]);
            // to List
            List<ulong> vec_size_t_list2;
            vec_size_t_list2 = _intrinsic.vec_size_t;
            put("vector<sise_t>: to List   ", "1", vec_size_t_list2.Count);
            put("vector<sise_t>: to List   ", "222", vec_size_t_list2[0]);
            // from array
            ulong[] vec_size_t_array = new ulong[] { 333 };
            _intrinsic.vec_size_t.clear();
            _intrinsic.vec_size_t = vec_size_t_array;
            put("vector<sise_t>: from array", "1", _intrinsic.vec_size_t.size());
            put("vector<sise_t>: from array", "333", _intrinsic.vec_size_t[0]);
            // to array
            ulong[] vec_size_t_array2;
            vec_size_t_array2 = _intrinsic.vec_size_t;
            put("vector<sise_t>: to array  ", "1", vec_size_t_array2.Length);
            put("vector<sise_t>: to array  ", "333", vec_size_t_array2[0]);

        // float
            System.Console.WriteLine("---- [ float ] ----");
            // by member access
            _intrinsic.vec_float.push_back(111.1F);
            put("vector<float>: by member", "1", _intrinsic.vec_float.size());
            put("vector<float>: by member", "111.1", _intrinsic.vec_float[0]);
            // from List
            List<float> vec_float_list = new List<float>(1) { 222.2F };
            _intrinsic.vec_float.clear();
            _intrinsic.vec_float = vec_float_list;
            put("vector<float>: from List ", "1", _intrinsic.vec_float.size());
            put("vector<float>: from List ", "222.2", _intrinsic.vec_float[0]);
            // to List
            List<float> vec_float_list2;
            vec_float_list2 = _intrinsic.vec_float;
            put("vector<float>: to List   ", "1", vec_float_list2.Count);
            put("vector<float>: to List   ", "222.2", vec_float_list2[0]);
            // from array
            float[] vec_float_array = new float[] { 333.3F };
            _intrinsic.vec_float.clear();
            _intrinsic.vec_float = vec_float_array;
            put("vector<float>: from array", "1", _intrinsic.vec_float.size());
            put("vector<float>: from array", "333.3", _intrinsic.vec_float[0]);
            // to array
            float[] vec_float_array2;
            vec_float_array2 = _intrinsic.vec_float;
            put("vector<float>: to array  ", "1", vec_float_array2.Length);
            put("vector<float>: to array  ", "333.3", vec_float_array2[0]);

        // double
            System.Console.WriteLine("---- [ double ] ----");
            // by member access
            _intrinsic.vec_double.push_back(111.1);
            put("vector<double>: by member", "1", _intrinsic.vec_double.size());
            put("vector<double>: by member", "111.1", _intrinsic.vec_double[0]);
            // from List
            List<double> vec_double_list = new List<double>(1) { 222.2 };
            _intrinsic.vec_double.clear();
            _intrinsic.vec_double = vec_double_list;
            put("vector<double>: from List ", "1", _intrinsic.vec_double.size());
            put("vector<double>: from List ", "222.2", _intrinsic.vec_double[0]);
            // to List
            List<double> vec_double_list2;
            vec_double_list2 = _intrinsic.vec_double;
            put("vector<double>: to List   ", "1", vec_double_list2.Count);
            put("vector<double>: to List   ", "222.2", vec_double_list2[0]);
            // from array
            double[] vec_double_array = new double[] { 333.3 };
            _intrinsic.vec_double.clear();
            _intrinsic.vec_double = vec_double_array;
            put("vector<double>: from array", "1", _intrinsic.vec_double.size());
            put("vector<double>: from array", "333.3", _intrinsic.vec_double[0]);
            // to array
            double[] vec_double_array2;
            vec_double_array2 = _intrinsic.vec_double;
            put("vector<double>: to array  ", "1", vec_double_array2.Length);
            put("vector<double>: to array  ", "333.3", vec_double_array2[0]);

        // string
/*** 参考
	/* string - by member access * /
	    descFemMeshNew.test_s.clear();
	    IntPtr pbstr_1 = Marshal.StringToBSTR("abc");
	    IntPtr pbstr_2 = Marshal.StringToBSTR("def");
	    descFemMeshNew.test_s.push_back(pbstr_1);
	    descFemMeshNew.test_s.push_back(pbstr_2);
	    Marshal.FreeBSTR(pbstr_1);
	    Marshal.FreeBSTR(pbstr_2);
            put("vec<string>", "3  ", descFemMeshNew.test_s.size());
            put("vec<string>", "abc", descFemMeshNew.test_s[0]);
            put("vec<string>", "def", descFemMeshNew.test_s[1]);
	    descFemMeshNew.test_s[0] = "abc";
	    descFemMeshNew.test_s[1] = "def";
            put("vec<string>", "2  ", descFemMeshNew.test_s.size());
            put("vec<string>", "abc", descFemMeshNew.test_s[0]);
            put("vec<string>", "def", descFemMeshNew.test_s[1]);
	/* double - by List * /
	    var csList_st = new List<string>();
	    csList_st.Add("abc");
	    csList_st.Add("def");
	    descFemMeshNew.test_s.clear();
	    descFemMeshNew.test_s = csList_st;
            put("List to vec<string>", "2  ", descFemMeshNew.test_s.size());
            put("List to vec<string>", "abc", descFemMeshNew.test_s[0]);
            put("List to vec<string>", "def", descFemMeshNew.test_s[1]);
	    csList_st = descFemMeshNew.test_s;
            put("vec to List<string>", "2  ", descFemMeshNew.test_s.size());
            put("vec to List<string>", "abc", descFemMeshNew.test_s[0]);
            put("vec to List<string>", "def", descFemMeshNew.test_s[1]);
***/
/*** 未対応
            System.Console.WriteLine("---- [ string ] ----");
            // by member access
            _intrinsic.vec_string.push_back("abc");
            put("vector<string>: by member", "1", _intrinsic.vec_string.size());
            put("vector<string>: by member", "abc", _intrinsic.vec_string[0]);
            // from List
            List<string> vec_string_list = new List<string>(1) { "def" };
            _intrinsic.vec_string.clear();
            _intrinsic.vec_string = vec_string_list;
            put("vector<string>: from List ", "1", _intrinsic.vec_string.size());
            put("vector<string>: from List ", "def", _intrinsic.vec_string[0]);
            // to List
            List<ststringring> vec_string_list2;
            vec_string_list2 = _intrinsic.vec_string;
            put("vector<string>: to List   ", "1", vec_string_list2.Count);
            put("vector<string>: to List   ", "def", vec_string_list2[0]);
            // from array
            string[] vec_string_array = new string[] { "ghi" };
            _intrinsic.vec_string.clear();
            _intrinsic.vec_string = vec_string_array;
            put("vector<string>: from array", "1", _intrinsic.vec_string.size());
            put("vector<string>: from array", "ghi", _intrinsic.vec_string[0]);
            // to array
            string[] vec_string_array2;
            vec_string_array2 = _intrinsic.vec_string;
            put("vector<string>: to array  ", "1", vec_string_array2.Length);
            put("vector<string>: to array  ", "ghi", vec_string_array2[0]);
***/
            put();

        // Test for structured data type.
            CSTestStruct _struct = new CSTestStruct();
            CSTestStructNested _nested1 = new CSTestStructNested();
            CSTestStructNested _nested2 = new CSTestStructNested();

        // simple struct
            System.Console.WriteLine("---- [ struct ] ----");

            Vec3d vec3d = new Vec3d(1.11, 2.22, 3.33);
            _struct.vec3d = vec3d;
            put("Vec3d: by struct assignment ", "(1.11, 2.22, 3.33)", _struct.vec3d, 2);
            _struct.vec3d.x = 4.44;
            _struct.vec3d.y = 5.55;
            _struct.vec3d.z = 6.66;
            put("Vec3d: by member assignment ", "(4.44, 5.55, 6.66)", _struct.vec3d, 2);

        // vector of struct
            // by member access
            Vec3d vec3d1 = new Vec3d(1.1, 2.2, 3.3);
            Vec3d vec3d2 = new Vec3d(4.4, 5.5, 6.6);
            _struct.vec_vec3d.push_back(vec3d1);
            _struct.vec_vec3d.push_back(vec3d2);
            put("vector<Vec3d>: by member  ", "2", _struct.vec_vec3d.size());
            put("vector<Vec3d>: by member  ", "(1.1, 2.2, 3.3)", _struct.vec_vec3d[0]);
            put("vector<Vec3d>: by member  ", "(4.4, 5.5, 6.6)", _struct.vec_vec3d[1]);
            // from List
            List<Vec3d> vec_vec3d_list = new List<Vec3d>(3);
            for (int i = 0; i < 3; i++) {
                vec_vec3d_list.Add(new Vec3d(11.1*(i+1), 22.2*(i+1), 33.3*(i+1)));
            }
            _struct.vec_vec3d.clear();
            _struct.vec_vec3d = vec_vec3d_list;
            put("vector<Vec3d>: from List ", "3", _struct.vec_vec3d.size());
            put("vector<Vec3d>: from List ", "(11.1, 22.2, 33.3)", _struct.vec_vec3d[0]);
            put("vector<Vec3d>: from List ", "(22.2, 44.4, 66.6)", _struct.vec_vec3d[1]);
            put("vector<Vec3d>: from List ", "(33.3, 66.6, 99.9)", _struct.vec_vec3d[2]);
            // to List
            List<Vec3d> vec_vec3d_list2;
            vec_vec3d_list2 = _struct.vec_vec3d;
            put("vector<Vec3d>: to List   ", "3", vec_vec3d_list2.Count);
            put("vector<Vec3d>: to List   ", "(11.1, 22.2, 33.3)", vec_vec3d_list2[0]);
            put("vector<Vec3d>: to List   ", "(22.2, 44.4, 66.6)", vec_vec3d_list2[1]);
            put("vector<Vec3d>: to List   ", "(33.3, 66.6, 99.9)", vec_vec3d_list2[2]);
            // from array
            Vec3d[] vec_vec3d_array = new Vec3d[1];
            vec_vec3d_array[0] = new Vec3d(111.1, 222.2, 333.3);
            _struct.vec_vec3d.clear();
            _struct.vec_vec3d = vec_vec3d_array;
            put("vector<Vec3d>: from array", "1", _struct.vec_vec3d.size());
            put("vector<Vec3d>: from array", "(111.1, 222.2, 333.3)", _struct.vec_vec3d[0]);
            // to array
            Vec3d[] vec_vec3d_array2 = new Vec3d[1];
            vec_vec3d_array2 = _struct.vec_vec3d;
            put("vector<Vec3d>: to array  ", "1", vec_vec3d_array2.Length);
            put("vector<Vec3d>: to array  ", "(111.1, 222.2, 333.3)", vec_vec3d_array2[0]);

        // nested struct
            System.Console.WriteLine("---- [ nested struct ] ----");

            // struct in struct member
            _struct.nested.vec3d = vec3d;
            put("nested.Vec3d: by struct assignment ", "(1.11, 2.22, 3.33)", _struct.nested.vec3d, 2);
            _struct.nested.vec3d.x = 4.44;
            _struct.nested.vec3d.y = 5.55;
            _struct.nested.vec3d.z = 6.66;
            put("nested.Vec3d: by member assignment ", "(4.44, 5.55, 6.66)", _struct.nested.vec3d, 2);

            // vector of struct in struct member
            _struct.nested.vec_vec3d.push_back(vec3d1);
            _struct.nested.vec_vec3d.push_back(vec3d2);
            put("nested.vector<Vec3d>: by member  ", "2", _struct.nested.vec_vec3d.size());
            put("nested.vector<Vec3d>: by member  ", "(1.1, 2.2, 3.3)", _struct.nested.vec_vec3d[0]);
            put("nested.vector<Vec3d>: by member  ", "(4.4, 5.5, 6.6)", _struct.nested.vec_vec3d[1]);
            // from List
            _struct.nested.vec_vec3d.clear();
            _struct.nested.vec_vec3d = vec_vec3d_list;
            put("nested.vector<Vec3d>: from List ", "3", _struct.nested.vec_vec3d.size());
            put("nested.vector<Vec3d>: from List ", "(11.1, 22.2, 33.3)", _struct.nested.vec_vec3d[0]);
            put("nested.vector<Vec3d>: from List ", "(22.2, 44.4, 66.6)", _struct.nested.vec_vec3d[1]);
            put("nested.vector<Vec3d>: from List ", "(33.3, 66.6, 99.9)", _struct.nested.vec_vec3d[2]);
            // to List
            vec_vec3d_list2 = _struct.nested.vec_vec3d;
            put("nested.vector<Vec3d>: to List   ", "3", vec_vec3d_list2.Count);
            put("nested.vector<Vec3d>: to List   ", "(11.1, 22.2, 33.3)", vec_vec3d_list2[0]);
            put("nested.vector<Vec3d>: to List   ", "(22.2, 44.4, 66.6)", vec_vec3d_list2[1]);
            put("nested.vector<Vec3d>: to List   ", "(33.3, 66.6, 99.9)", vec_vec3d_list2[2]);
            // from array
            _struct.nested.vec_vec3d.clear();
            _struct.nested.vec_vec3d = vec_vec3d_array;
            put("nested.vector<Vec3d>: from array", "1", _struct.nested.vec_vec3d.size());
            put("nested.vector<Vec3d>: from array", "(111.1, 222.2, 333.3)", _struct.nested.vec_vec3d[0]);
            // to array
            vec_vec3d_array2 = _struct.nested.vec_vec3d;
            put("nested.vector<Vec3d>: to array  ", "1", vec_vec3d_array2.Length);
            put("nested.vector<Vec3d>: to array  ", "(111.1, 222.2, 333.3)", vec_vec3d_array2[0]);

            // struct in vector of struct member
            Vec3d vec3d3 = new Vec3d(7.7, 8.8, 9.9);
            _nested1.vec_vec3d.push_back(vec3d1);
            _nested1.vec_vec3d.push_back(vec3d2);
            _nested2.vec_vec3d.push_back(vec3d3);
            _struct.vec_nested.push_back(_nested1);
            _struct.vec_nested.push_back(_nested2);
            put("vector.vector<Vec3d>: by member  ", "2", _struct.vec_nested[0].vec_vec3d.size());
            put("vector.vector<Vec3d>: by member  ", "(1.1, 2.2, 3.3)", _struct.vec_nested[0].vec_vec3d[0]);
            put("vector.vector<Vec3d>: by member  ", "(4.4, 5.5, 6.6)", _struct.vec_nested[0].vec_vec3d[1]);
            put("vector.vector<Vec3d>: by member  ", "1", _struct.vec_nested[1].vec_vec3d.size());
            put("vector.vector<Vec3d>: by member  ", "(7.7, 8.8, 9.9)", _struct.vec_nested[1].vec_vec3d[0]);
            // from List
            _struct.vec_nested[0].vec_vec3d.clear();
            _struct.vec_nested[0].vec_vec3d = vec_vec3d_list;
            put("vector.vector<Vec3d>: from List ", "3", _struct.vec_nested[0].vec_vec3d.size());
            put("vector.vector<Vec3d>: from List ", "(11.1, 22.2, 33.3)", _struct.vec_nested[0].vec_vec3d[0]);
            put("vector.vector<Vec3d>: from List ", "(22.2, 44.4, 66.6)", _struct.vec_nested[0].vec_vec3d[1]);
            put("vector.vector<Vec3d>: from List ", "(33.3, 66.6, 99.9)", _struct.vec_nested[0].vec_vec3d[2]);
            // to List
            vec_vec3d_list2 = _struct.vec_nested[0].vec_vec3d;
            put("vector.vector<Vec3d>: to List   ", "3", vec_vec3d_list2.Count);
            put("vector.vector<Vec3d>: to List   ", "(11.1, 22.2, 33.3)", vec_vec3d_list2[0]);
            put("vector.vector<Vec3d>: to List   ", "(22.2, 44.4, 66.6)", vec_vec3d_list2[1]);
            put("vector.vector<Vec3d>: to List   ", "(33.3, 66.6, 99.9)", vec_vec3d_list2[2]);
            // from array
            _struct.vec_nested[0].vec_vec3d.clear();
            _struct.vec_nested[0].vec_vec3d = vec_vec3d_array;
            put("vector.vector<Vec3d>: from array", "1", _struct.vec_nested[0].vec_vec3d.size());
            put("vector.vector<Vec3d>: from array", "(111.1, 222.2, 333.3)", _struct.vec_nested[0].vec_vec3d[0]);
            // to array
            vec_vec3d_array2 = _struct.vec_nested[0].vec_vec3d;
            put("vector.vector<Vec3d>: to array  ", "1", vec_vec3d_array2.Length);
            put("vector.vector<Vec3d>: to array  ", "(111.1, 222.2, 333.3)", vec_vec3d_array2[0]);
        }

        static void test_array() {
            test_name("array");

        // Test for intrinsic data type.
            CSTestIntrinsic _intrinsic = new CSTestIntrinsic();

	// int
            for (int i = 0; i < _intrinsic.ary_int.size(); i++) {
                _intrinsic.ary_int[i] = 11 * (i+1);
            }
            put("array<int>: by member", "3", _intrinsic.ary_int.size());
            for (int i = 0; i < _intrinsic.ary_int.size(); i++) {
            	int expected = 11 * (i+1);
            	put("array<int>: by member", expected, _intrinsic.ary_int[i]);
            }
            // from List
            List<int> ary_int_list = new List<int>(3);
            for (int i = 0; i < _intrinsic.ary_int.size(); i++) {
            	ary_int_list.Add(111 * (i+1));
            }
            _intrinsic.ary_int = ary_int_list;
            put("array<int>: from List", "3", _intrinsic.ary_int.size());
            for (int i = 0; i < _intrinsic.ary_int.size(); i++) {
            	int expected = 111 * (i+1);
            	put("array<int>: from List", expected, _intrinsic.ary_int[i]);
            }
            // to List
            List<int> ary_int_list2;
            ary_int_list2 = _intrinsic.ary_int;
            put("array<int>: to List", "3", _intrinsic.ary_int.size());
            for (int i = 0; i < ary_int_list2.Count; i++) {
            	int expected = 111 * (i+1);
            	put("array<int>: to List", expected, _intrinsic.ary_int[i]);
            }
            // from array
            int[] ary_int_array = new int[] { 1111, 2222, 3333 };
            _intrinsic.ary_int = ary_int_array;
            put("array<int>: from array", "3", _intrinsic.ary_int.size());
            for (int i = 0; i < _intrinsic.ary_int.size(); i++) {
            	int expected = 1111 * (i+1);
            	put("array<int>: from array", expected, _intrinsic.ary_int[i]);
            }
            // to array
            int[] ary_int_array2;
            ary_int_array2 = _intrinsic.ary_int;
            put("array<int>: to array", "3", ary_int_array2.Length);
            for (int i = 0; i < ary_int_array2.Length; i++) {
            	int expected = 1111 * (i+1);
            	put("array<int>: to List", expected, _intrinsic.ary_int[i]);
            }

	//  同じクラス/構造体の中に同じデータ型のvectorが複数あったときに区別が
	//  できるかのテスト
        // int2
            System.Console.WriteLine("---- [ int2 ] ----");
            // by member access
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
                _intrinsic.ary_int2[i] = 2222 * (i+1);
            }
            put("array<int>: by member", "3", _intrinsic.ary_int2.size());
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
            	int expected = 2222 * (i+1);
            	put("array<int>: by member", expected, _intrinsic.ary_int2[i]);
            }
            // from List
            List<int> ary_int2_list = new List<int>(3);
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
                ary_int2_list.Add(22222 * (i+1));
            }
            _intrinsic.ary_int2 = ary_int2_list;
            put("array<int>: from List ", "3", _intrinsic.ary_int2.size());
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
            	int expected = 22222 * (i+1);
            	put("array<int>: from List", expected, _intrinsic.ary_int2[i]);
            }
            // to List
            List<int> ary_int2_list2;
            ary_int2_list2 = _intrinsic.ary_int2;
            put("array<int>: to List   ", "3", _intrinsic.ary_int2.size());
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
            	int expected = 22222 * (i+1);
            	put("array<int>: from List", expected, _intrinsic.ary_int2[i]);
            }
            // from array
            int[] ary_int2_array = new int[] { 222222, 444444, 666666 };
            _intrinsic.ary_int2 = ary_int2_array;
            put("array<int>: from array", "3", _intrinsic.ary_int2.size());
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
            	int expected = 222222 * (i+1);
            	put("array<int>: from array", expected, _intrinsic.ary_int2[i]);
            }
            // to array
            int[] ary_int2_array2;
            ary_int2_array2 = _intrinsic.ary_int2;
            put("array<int>: to array  ", "3", ary_int2_array2.Length);
            for (int i = 0; i < _intrinsic.ary_int2.size(); i++) {
            	int expected = 222222 * (i+1);
            	put("array<int>: to array  ", expected, ary_int2_array2[i]);
            }

            System.Console.WriteLine("---- [ verify ] ----");
            put("array<int>: to array", "3", ary_int_array2.Length);
            for (int i = 0; i < ary_int_array2.Length; i++) {
            	int expected = 1111 * (i+1);
            	put("array<int>: to List", expected, _intrinsic.ary_int[i]);
            }

        // unsigned int
            System.Console.WriteLine("---- [ unsigned int ] ----");
            // by member access
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
                _intrinsic.ary_unsigned_int[i] = (uint) (111 * (i+1));
            }
            put("array<unsigned int>: by member", "3", _intrinsic.ary_unsigned_int.size());
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
            	int expected = 111 * (i+1);
            	put("array<unsigned int>: by member  ", expected, _intrinsic.ary_unsigned_int[i]);
            }
            // from List
            List<uint> ary_unsigned_int_list = new List<uint>(3) { 222, 444, 666 };
            _intrinsic.ary_unsigned_int = ary_unsigned_int_list;
            put("array<unsigned int>: from List ", "3", _intrinsic.ary_unsigned_int.size());
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
            	int expected = 222 * (i+1);
            	put("array<unsigned int>: from List  ", expected, _intrinsic.ary_unsigned_int[i]);
            }
            // to List
            List<uint> ary_unsigned_int_list2;
            ary_unsigned_int_list2 = _intrinsic.ary_unsigned_int;
            put("array<unsigned int>: to List   ", "3", ary_unsigned_int_list2.Count);
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
            	int expected = 222 * (i+1);
            	put("array<unsigned int>: from List  ", expected, ary_unsigned_int_list2[i]);
            }
            // from array
            uint[] ary_unsigned_int_array = new uint[] { 333, 666, 999 };
            _intrinsic.ary_unsigned_int = ary_unsigned_int_array;
            put("array<unsigned int>: from array   ", "3", _intrinsic.ary_unsigned_int.size());
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
            	int expected = 333 * (i+1);
            	put("array<unsigned int>: from array  ", expected, _intrinsic.ary_unsigned_int[i]);
            }
            // to array
            uint[] ary_unsigned_int_array2;
            ary_unsigned_int_array2 = _intrinsic.ary_unsigned_int;
            put("array<unsigned int>: to array   ", "3", ary_unsigned_int_array2.Length);
            for (int i = 0; i < _intrinsic.ary_unsigned_int.size(); i++) {
            	int expected = 333 * (i+1);
            	put("array<unsigned int>: to array  ", expected, ary_unsigned_int_array2[i]);
            }
 
        // size_t
            System.Console.WriteLine("---- [ size_t ] ----");
            // by member access
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
                _intrinsic.ary_size_t[i] = (ulong) (111 * (i+1));
            }
            put("array<size_t>: by member", "3", _intrinsic.ary_size_t.size());
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
            	int expected = 111 * (i+1);
            	put("array<size_t>: by member  ", expected, _intrinsic.ary_size_t[i]);
            }
            // from List
            List<ulong> ary_size_t_list = new List<ulong>(3) { 222, 444, 666 };
            _intrinsic.ary_size_t = ary_size_t_list;
            put("array<size_t>: from List ", "3", _intrinsic.ary_size_t.size());
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
            	int expected = 222 * (i+1);
            	put("array<size_t>: from List  ", expected, _intrinsic.ary_size_t[i]);
            }
            // to List
            List<ulong> ary_size_t_list2;
            ary_size_t_list2 = _intrinsic.ary_size_t;
            put("array<size_t>: to List   ", "3", _intrinsic.ary_size_t.size());
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
            	int expected = 222 * (i+1);
            	put("array<size_t>: from List  ", expected, ary_size_t_list2[i]);
            }
            // from array
            ulong[] ary_size_t_array = new ulong[] { 333, 666, 999 };
            _intrinsic.ary_size_t = ary_size_t_array;
            put("array<size_t>: from array   ", "3", _intrinsic.ary_size_t.size());
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
            	int expected = 333 * (i+1);
            	put("array<size_t>: from array  ", expected, _intrinsic.ary_size_t[i]);
            }
            // to array
            ulong[] ary_size_t_array2;
            ary_size_t_array2 = _intrinsic.ary_size_t;
            put("array<size_t>: to array   ", "3", ary_size_t_array2.Length);
            for (int i = 0; i < _intrinsic.ary_size_t.size(); i++) {
            	int expected = 333 * (i+1);
            	put("array<size_t>: to array  ", expected, ary_size_t_array2[i]);
            }
 
        // float
            System.Console.WriteLine("---- [ float ] ----");
            // by member access
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
                _intrinsic.ary_float[i] = (float) (111.1F * (i+1));
            }
            put("array<float>: by member", "3", _intrinsic.ary_float.size());
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
            	float expected = 111.1F * (i+1);
            	put("array<float>: by member  ", expected, _intrinsic.ary_float[i]);
            }
            // from List
            List<float> ary_float_list = new List<float>(3) { 222.2F, 444.4F, 666.6F };
            _intrinsic.ary_float = ary_float_list;
            put("array<float>: from List ", "3", _intrinsic.ary_float.size());
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
            	float expected = 222.2F * (i+1);
            	put("array<float>: from List  ", expected, _intrinsic.ary_float[i]);
            }
            // to List
            List<float> ary_float_list2;
            ary_float_list2 = _intrinsic.ary_float;
            put("array<float>: to List   ", "3", _intrinsic.ary_float.size());
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
            	float expected = 222.2F * (i+1);
            	put("array<float>: from List  ", expected, ary_float_list2[i]);
            }
            // from array
            float[] ary_float_array = new float[] { 333.3F, 666.6F, 999.9F };
            _intrinsic.ary_float = ary_float_array;
            put("array<float>: from array   ", "3", _intrinsic.ary_float.size());
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
            	float expected = 333.3F * (i+1);
            	put("array<float>: from array  ", expected, _intrinsic.ary_float[i]);
            }
            // to array
            float[] ary_float_array2;
            ary_float_array2 = _intrinsic.ary_float;
            put("array<float>: to array   ", "3", ary_float_array2.Length);
            for (int i = 0; i < _intrinsic.ary_float.size(); i++) {
            	float expected = 333.3F * (i+1);
            	put("array<float>: to array  ", expected, ary_float_array2[i]);
            }
 
        // double
            System.Console.WriteLine("---- [ double ] ----");
            // by member access
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
                _intrinsic.ary_double[i] = 111.1 * (i+1);
            }
            put("array<double>: by member", "3", _intrinsic.ary_double.size());
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
            	double expected = 111.1 * (i+1);
            	put("array<double>: by member  ", expected, _intrinsic.ary_double[i]);
            }
            // from List
            List<double> ary_double_list = new List<double>(3) { 222.2, 444.4, 666.6 };
            _intrinsic.ary_double = ary_double_list;
            put("array<double>: from List ", "3", _intrinsic.ary_double.size());
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
            	double expected = 222.2 * (i+1);
            	put("array<double>: from List  ", expected, _intrinsic.ary_double[i]);
            }
            // to List
            List<double> ary_double_list2;
            ary_double_list2 = _intrinsic.ary_double;
            put("array<double>: to List   ", "3", _intrinsic.ary_double.size());
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
            	double expected = 222.2 * (i+1);
            	put("array<double>: from List  ", expected, ary_double_list2[i]);
            }
            // from array
            double[] ary_double_array = new double[] { 333.3, 666.6, 999.9 };
            _intrinsic.ary_double = ary_double_array;
            put("array<double>: from array   ", "3", _intrinsic.ary_double.size());
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
            	double expected = 333.3 * (i+1);
            	put("array<double>: from array  ", expected, _intrinsic.ary_double[i]);
            }
            // to array
            double[] ary_double_array2;
            ary_double_array2 = _intrinsic.ary_double;
            put("array<double>: to array   ", "3", ary_double_array2.Length);
            for (int i = 0; i < _intrinsic.ary_double.size(); i++) {
            	double expected = 333.3 * (i+1);
            	put("array<double>: to array  ", expected, ary_double_array2[i]);
            }

        // Test for structured data type.
            CSTestStruct _struct = new CSTestStruct();
            CSTestStructNested _nested1 = new CSTestStructNested();
            CSTestStructNested _nested2 = new CSTestStructNested();
            CSTestStructNested _nested3 = new CSTestStructNested();

        // array of struct
            System.Console.WriteLine("---- [ array of struct ] ----");

            // by member access
            Vec3d vec3d1 = new Vec3d(1.1, 2.2, 3.3);
            Vec3d vec3d2 = new Vec3d(4.4, 5.5, 6.6);
            Vec3d vec3d3 = new Vec3d(7.7, 8.8, 9.9);
            _struct.ary_vec3d[0] = vec3d1;
            _struct.ary_vec3d[1] = vec3d2;
            _struct.ary_vec3d[2] = vec3d3;
            put("array of Vec3d: by member  ", "3", _struct.ary_vec3d.size());
            put("array of Vec3d: by member  ", "(1.1, 2.2, 3.3)", _struct.ary_vec3d[0]);
            put("array of Vec3d: by member  ", "(4.4, 5.5, 6.6)", _struct.ary_vec3d[1]);
            put("array of Vec3d: by member  ", "(7.7, 8.8, 9.9)", _struct.ary_vec3d[2]);
            // from List
            List<Vec3d> ary_vec3d_list = new List<Vec3d>(3);
            for (int i = 0; i < 3; i++) {
                ary_vec3d_list.Add(new Vec3d(11.1*(i+1), 22.2*(i+1), 33.3*(i+1)));
            }
            _struct.ary_vec3d = ary_vec3d_list;
            put("array of Vec3d: from List ", "3", _struct.ary_vec3d.size());
            put("array of Vec3d: from List ", "(11.1, 22.2, 33.3)", _struct.ary_vec3d[0]);
            put("array of Vec3d: from List ", "(22.2, 44.4, 66.6)", _struct.ary_vec3d[1]);
            put("array of Vec3d: from List ", "(33.3, 66.6, 99.9)", _struct.ary_vec3d[2]);
            // to List
            List<Vec3d> ary_vec3d_list2;
            ary_vec3d_list2 = _struct.ary_vec3d;
            put("array of Vec3d: to List   ", "3", ary_vec3d_list2.Count);
            put("array of Vec3d: to List   ", "(11.1, 22.2, 33.3)", ary_vec3d_list2[0]);
            put("array of Vec3d: to List   ", "(22.2, 44.4, 66.6)", ary_vec3d_list2[1]);
            put("array of Vec3d: to List   ", "(33.3, 66.6, 99.9)", ary_vec3d_list2[2]);
            // from array
            Vec3d[] ary_vec3d_array = new Vec3d[3];
            ary_vec3d_array[0] = new Vec3d(111.1, 222.2, 333.3);
            ary_vec3d_array[1] = new Vec3d(444.4, 555.5, 666.6);
            ary_vec3d_array[2] = new Vec3d(777.7, 888.8, 999.9);
            _struct.ary_vec3d = ary_vec3d_array;
            put("array of Vec3d: from array", "3", _struct.ary_vec3d.size());
            put("array of Vec3d: from array", "(111.1, 222.2, 333.3)", _struct.ary_vec3d[0]);
            put("array of Vec3d: from array", "(444.4, 555.5, 666.6)", _struct.ary_vec3d[1]);
            put("array of Vec3d: from array", "(777.7, 888.8, 999.9)", _struct.ary_vec3d[2]);
            // to array
            Vec3d[] ary_vec3d_array2 = new Vec3d[3];
            ary_vec3d_array2 = _struct.ary_vec3d;
            put("array of Vec3d: to array", "3", ary_vec3d_array2.Length);
            put("array of Vec3d: to array", "(111.1, 222.2, 333.3)", ary_vec3d_array2[0]);
            put("array of Vec3d: to array", "(444.4, 555.5, 666.6)", ary_vec3d_array2[1]);
            put("array of Vec3d: to array", "(777.7, 888.8, 999.9)", ary_vec3d_array2[2]);

        // array of struct in struct member

            // by member access
            Vec3d vec3d_111 = new Vec3d(1.1, 1.1, 1.1);
            Vec3d vec3d_112 = new Vec3d(2.2, 2.2, 2.2);
            Vec3d vec3d_113 = new Vec3d(3.3, 3.3, 3.3);
            Vec3d vec3d_121 = new Vec3d(4.4, 4.4, 4.4);
            Vec3d vec3d_122 = new Vec3d(5.5, 5.5, 5.5);
            Vec3d vec3d_123 = new Vec3d(6.6, 6.6, 6.6);
            _nested1.ary_vec3d[0] = vec3d_111;
            _nested1.ary_vec3d[1] = vec3d_112;
            _nested1.ary_vec3d[2] = vec3d_113;
            _struct.ary_nested[0] = _nested1;
            put("nested array of Vec3d: by member", "3", _struct.ary_nested[0].ary_vec3d.size());
            put("nested array of Vec3d: by member", "(1.1, 1.1, 1.1)", _struct.ary_nested[0].ary_vec3d[0]);
            put("nested array of Vec3d: by member", "(2.2, 2.2, 2.2)", _struct.ary_nested[0].ary_vec3d[1]);
            put("nested array of Vec3d: by member", "(3.3, 3.3, 3.3)", _struct.ary_nested[0].ary_vec3d[2]);
            // from List
            List<Vec3d> nested_ary_vec3d_list = new List<Vec3d>(3);
            for (int i = 0; i < 3; i++) {
                nested_ary_vec3d_list.Add(new Vec3d(11.1*(i+1), 11.1*(i+1), 11.1*(i+1)));
            }
            _struct.ary_nested[1].ary_vec3d = nested_ary_vec3d_list;
            put("nested array of Vec3d: from List ", "3", _struct.ary_vec3d.size());
            put("nested array of Vec3d: from List ", "(11.1, 11.1, 11.1)", _struct.ary_nested[1].ary_vec3d[0]);
            put("nested array of Vec3d: from List ", "(22.2, 22.2, 22.2)", _struct.ary_nested[1].ary_vec3d[1]);
            put("nested array of Vec3d: from List ", "(33.3, 33.3, 33.3)", _struct.ary_nested[1].ary_vec3d[2]);
            // to List
            List<Vec3d> nested_ary_vec3d_list2;
            nested_ary_vec3d_list2 = _struct.ary_nested[1].ary_vec3d;
            put("neseted array of Vec3d: to List   ", "3", nested_ary_vec3d_list2.Count);
            put("neseted array of Vec3d: to List   ", "(11.1, 11.1, 11.1)", nested_ary_vec3d_list2[0]);
            put("neseted array of Vec3d: to List   ", "(22.2, 22.2, 22.2)", nested_ary_vec3d_list2[1]);
            put("neseted array of Vec3d: to List   ", "(33.3, 33.3, 33.3)", nested_ary_vec3d_list2[2]);
            // from array
            Vec3d[] nested_ary_vec3d_array = new Vec3d[3];
            nested_ary_vec3d_array[0] = new Vec3d(111.1, 111.1, 111.1);
            nested_ary_vec3d_array[1] = new Vec3d(222.2, 222.2, 222.2);
            nested_ary_vec3d_array[2] = new Vec3d(333.3, 333.3, 333.3);
            _struct.ary_nested[2].ary_vec3d = nested_ary_vec3d_array;
            put("nesetd array of Vec3d: from array", "3", _struct.ary_nested[2].ary_vec3d.size());
            put("nesetd array of Vec3d: from array", "(111.1, 111.1, 111.1)", _struct.ary_nested[2].ary_vec3d[0]);
            put("nesetd array of Vec3d: from array", "(222.2, 222.2, 222.2)", _struct.ary_nested[2].ary_vec3d[1]);
            put("nesetd array of Vec3d: from array", "(333.3, 333.3, 333.3)", _struct.ary_nested[2].ary_vec3d[2]);
            // to array
            Vec3d[] nested_ary_vec3d_array2 = new Vec3d[3];
            nested_ary_vec3d_array2 = _struct.ary_nested[2].ary_vec3d;
            put("array of Vec3d: to array", "3", nested_ary_vec3d_array2.Length);
            put("array of Vec3d: to array", "(111.1, 111.1, 111.1)", nested_ary_vec3d_array2[0]);
            put("array of Vec3d: to array", "(222.2, 222.2, 222.2)", nested_ary_vec3d_array2[1]);
            put("array of Vec3d: to array", "(333.3, 333.3, 333.3)", nested_ary_vec3d_array2[2]);

            // verify
            put("verify nested[0]", "3", _struct.ary_nested[0].ary_vec3d.size());
            put("verify nested[0]", "(1.1, 1.1, 1.1)", _struct.ary_nested[0].ary_vec3d[0]);
            put("verify nested[0]", "(2.2, 2.2, 2.2)", _struct.ary_nested[0].ary_vec3d[1]);
            put("verify nested[0]", "(3.3, 3.3, 3.3)", _struct.ary_nested[0].ary_vec3d[2]);
            put("verify nested[1] ", "3", _struct.ary_vec3d.size());
            put("verify nested[1] ", "(11.1, 11.1, 11.1)", _struct.ary_nested[1].ary_vec3d[0]);
            put("verify nested[1] ", "(22.2, 22.2, 22.2)", _struct.ary_nested[1].ary_vec3d[1]);
            put("verify nested[1] ", "(33.3, 33.3, 33.3)", _struct.ary_nested[1].ary_vec3d[2]);
            put("verify nested[2]", "3", _struct.ary_nested[2].ary_vec3d.size());
            put("verify nested[2]", "(111.1, 111.1, 111.1)", _struct.ary_nested[2].ary_vec3d[0]);
            put("verify nested[2]", "(222.2, 222.2, 222.2)", _struct.ary_nested[2].ary_vec3d[1]);
            put("verify nested[2]", "(333.3, 333.3, 333.3)", _struct.ary_nested[2].ary_vec3d[2]);
        }

	// ------------------
	//  helper functions
	// ------------------
        static bool check_test(string chk) {
            return inc.Contains(chk);
        }
	static void test_name(string test_name) {
            System.Console.WriteLine("\n---[ " + test_name + " ]---");
	}
	static void put(string s = "") { System.Console.WriteLine(s); }

	static void put(string title, object expected, object result, int fraction=1) {
            //System.Console.Write(result.GetType()+" ");
            switch (result) {
                case SprCs.Vec3f v3f:
                    result = edit_vector(v3f);
                    break;
                case SprCs.Vec3d v3d:
                    result = edit_vector(v3d, fraction);
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
