/* insert in include/Physics/SprPHScene.h */

#if 1111
struct SprCSTestDesc {
	Vec3d Arg1;
	std::vector<Vec3d> Arg2;
	Vec3d& Arg3 = Arg1;
	Vec3d Arg4[3];
	int Arg5;
	std::vector<int> Arg6;
	int& Arg7 = Arg5;
	int Arg8[3];
	SprCSTestDesc() {}
	SprCSTestDesc(Vec3d arg1, std::vector<Vec3d> arg2, Vec3d& arg3, Vec3d arg4[3],
			int arg5, std::vector<int> arg6, int& arg7, int arg8[3]) {
		Arg1 = arg1;
		Arg2 = arg2;
		Arg3 = arg3;
		Arg4[0] = arg4[0]; Arg4[1] = arg4[1]; Arg4[2] = arg4[2];
		Arg5 = arg5;
		Arg6 = arg6;
		Arg7 = arg7;
		Arg8[0] = arg8[0]; Arg8[1] = arg8[1]; Arg8[2] = arg8[2];
	}
};
struct SprCSTestIf : public ObjectIf {
	SprCSTestIf() {}
	const IfInfo* SPR_CDECL GetIfInfo() const { return NULL; }
	static const IfInfo* SPR_CDECL GetIfInfoStatic() { return NULL; }
};
struct SprCSTest {
	SprCSTest() {
		Vec3d arg1(1,2,3); Vec3d arg1b(11,22,33); Vec3d arg1c(111,222,333);
		std::vector<Vec3d> arg2;
		arg2.push_back(arg1);
		Vec3d& arg3 = arg1;
		Vec3d arg4[3] = { arg1 };
		int arg5 = 1; int arg5b = 11; int arg5c = 111;
		std::vector<int> arg6;
		arg6.push_back(arg5);
		int& arg7 = arg5;
		int arg8[3] = { arg5, arg5b, arg5c };
		desc = new SprCSTestDesc(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	SprCSTestDesc* desc;
	Vec3d R1()		{ return desc->Arg1; }
	std::vector<Vec3d> R2()	{ return desc->Arg2; }
	Vec3d& R3()		{ return desc->Arg3; }
	//Vec3d (&R4())[1]	{ return desc->Arg4; }
	int R5()		{ return desc->Arg5; }
	std::vector<int> R6()	{ return desc->Arg6; }
	int& R7()		{ return desc->Arg7; }
	//int (&R8())[1]		{ return desc->Arg8; }
};
#endif
