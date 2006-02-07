#if 0	//	this file is not for compile but for bin/typedesc.exe
struct Vec3f{ float x; float y; float z; };
struct Vec3d{ double x; double y; double z; };
struct Vec4f{ float x; float y; float z; float w;};
struct Vec4d{ double x; double y; double z; double w;};
struct Quaternionf{ float x; float y; float z; float w;};
struct Quaterniond{ double x; double y; double z; double w;};
struct Posef{ Quaternionf ori; Vec3f pos; }
struct Posed{ Quaterniond ori; Vec3d pos; }
struct Matrix3f{ float data[9]; }
struct Matrix3d{ double data[9]; }
#endif
