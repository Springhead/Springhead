/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifdef SWIGSPR	//	this file is not for compile but for bin/typedesc.exe
namespace Spr{
//struct Vec2f{ float x; float y; };
//struct Vec2d{ double x; double y; };
struct Vec2f{ float x; float y;
	      float square(); float norm(); void unitize();
};
struct Vec2d{ double x; double y;
	      double square(); double norm(); void unitize();
};
//struct Vec3f{ float x; float y; float z; };
//struct Vec3d{ double x; double y; double z; };
struct Vec3f{ float x; float y; float z;
	      float square(); float norm(); void unitize();
};
struct Vec3d{ double x; double y; double z;
	      double square(); double norm(); void unitize();
};
//struct Vec4f{ float x; float y; float z; float w;};
//struct Vec4d{ double x; double y; double z; double w;};
struct Vec4f{ float x; float y; float z; float w;
	      float square(); float norm(); void unitize();
};
struct Vec4d{ double x; double y; double z; double w;
	      double square(); double norm(); void unitize();
};
//struct Quaternionf{ float x; float y; float z; float w; };
//struct Quaterniond{ double x; double y; double z; double w; };
struct Quaternionf{ float x; float y; float z; float w;
		    float W(); float X(); float Y(); float Z(); Vec3f V();
		    Vec3f RotationHalf(); Vec3f Rotation(); Vec3f Axis(); float Theta();
		    static Quaternionf Rot(float angle, Vec3f axis);
		    static Quaternionf Rot(float angle, char axis);
		    static Quaternionf Rot(Vec3f& rot);
		    Quaternionf Inv();
		    void Conjugate(); Quaternionf Conjugated();
		    void RotationArc(Vec3f& lhs, Vec3f& rhs);
		    void Euler(float heading, float attitude, float bank);
};
struct Quaterniond{ double x; double y; double z; double w;
		    double W(); double X(); double Y(); double Z(); Vec3d V();
		    Vec3d RotationHalf(); Vec3d Rotation(); Vec3d Axis(); double Theta();
		    static Quaterniond Rot(double angle, Vec3d axis);
		    static Quaterniond Rot(double angle, char axis);
		    static Quaterniond Rot(Vec3d& rot);
		    Quaterniond Inv();
		    void Conjugate(); Quaterniond Conjugated();
		    void RotationArc(Vec3d& lhs, Vec3d& rhs);
		    void Euler(double heading, double attitude, double bank);
};
//struct Posef{ float w; float x; float y; float z; float px; float py; float pz; };
//struct Posed{ double w; double x; double y; double z; double px; double py; double pz; };
struct Posef{ float w; float x; float y; float z; float px; float py; float pz;
	      Vec3f Pos(); Quaternionf Ori();
	      float W(); float X(); float Y(); float Z(); float Px(); float Py(); float Pz();
	      float PosX(); float PosY(); float PosZ();
	      float OriX(); float OriY(); float OriZ(); float OriW();
	      Posef Inv();
	      static Posef Unit();
	      static Posef Trn(float px, float py, float pz);
	      static Posef Trn(Vec3f& v);
	      static Posef Rot(float wi, float xi, float yi, float zi);
	      static Posef Rot(float angle, Vec3f axis);
	      static Posef Rot(float angle, char axis);
	      static Posef Rot(Vec3f& rot);
	      static Posef Rot(Quaternionf& q);
};
struct Posed{ double w; double x; double y; double z; double px; double py; double pz;
	      Vec3d Pos(); Quaterniond Ori();
	      double W(); double X(); double Y(); double Z(); double Px(); double Py(); double Pz();
	      double PosX(); double PosY(); double PosZ();
	      double OriX(); double OriY(); double OriZ(); double OriW();
	      Posed Inv();
	      static Posed Unit();
	      static Posed Trn(double px, double py, double pz);
	      static Posed Trn(Vec3f& v);
	      static Posed Rot(double wi, double xi, double yi, double zi);
	      static Posed Rot(double angle, Vec3f axis);
	      static Posed Rot(double angle, char axis);
	      static Posed Rot(Vec3d& rot);
	      static Posed Rot(Quaterniond& q);
};

#ifdef SWIG_CS_SPR
// for C#
//struct Vec2i{ int x; int y; };
//struct Vec3i{ int x; int y; int z; };
//struct Vec4i{ int x; int y; int z; int w; };
//struct Vec6f{ float vx; float vy; float vz; float wx; float wy; float wz; };
//struct Vec6d{ double vx; double vy; double vz; double wx; double wy; double wz; };
struct Vec2i{ int x; int y;
	      int square(); int norm(); void unitize();
};
struct Vec3i{ int x; int y; int z;
	      int square(); int norm(); void unitize();
};
struct Vec4i{ int x; int y; int z; int w;
	      int square(); int norm(); void unitize();
};
struct Vec6f{ float vx; float vy; float vz; float wx; float wy; float wz;
	      float square(); float norm(); void unitize();
};
struct Vec6d{ double vx; double vy; double vz; double wx; double wy; double wz;
	      double square(); double norm(); void unitize();
};
struct Curve3f{};
//struct Matrix3f{ float xx,xy,xz, yx,yy,yz, zx,zy,zz; };
//struct Matrix3d{ double xx,xy,xz, yx,yy,yz, zx,zy,zz; };
struct Matrix3f{ float xx, xy, xz, yx, yy, yz, zx, zy, zz;
		 size_t height(); size_t width();
		 void resize(size_t h, size_t w);
		 size_t stride(); void clear();
};
struct Matrix3d{ double xx, xy, xz, yx, yy, yz, zx, zy, zz;
		 size_t height(); size_t width();
		 void resize(size_t h, size_t w);
		 size_t stride(); void clear();
};
struct Affinef{ float xx,xy,xz,xw, yx,yy,yz,yw, zx,zy,zz,zw, px,py,pz,pw; };
struct Affined{ double xx,xy,xz,xw, yx,yy,yz,yw, zx,zy,zz,zw, px,py,pz,pw; };
struct IfInfo{ bool Inherit(const IfInfo* info); const char* ClassName(); };

#else
struct Matrix3f{ float data[9]; };
struct Matrix3d{ double data[9]; };
struct Affinef{ float data[16]; };
struct Affined{ double data[16]; };
#endif
}
#endif
