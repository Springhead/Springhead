#include "Base.h"
#ifdef _WIN32
#pragma hdrstop
#endif
#include "TQuaternion.h"



namespace Spr{;

class TAffineTest: public TAffine<float>{
public:
#undef TYPENAME
#define TYPENAME
	typedef TAffine<float> base_type;
//	DEF_TMATRIX_BASIC_MEMBER(TAffineTest);
//	DEF_TAFFINE_CONSTRUCTORS(TAffineTest);
//	DEF_TAFFINE_CONSTRUCTORS2(TAffineTest);
};
/*
	struct A{
		int a;
		A(){}
	};
	union TestU{
		TestU(){}
		TestU(const TestU& t){}
		A a;
		int i;
	};
*/
	static void Test(){
	TPose<float> pose;
	}

};
