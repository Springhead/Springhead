#ifndef PRIMITIVE_MESHSHAPE_H
#define PRIMITIVE_MESHSHAPE_H

#include <Springhead.h>
using namespace Spr;
using namespace PTM;

Matrix3d CalcBoxInertia(double mass, Vec3f s){
	double a,b,c;
	a = pow(s[0], 2);
	b = pow(s[1], 2);
	c = pow(s[2], 2);
	Matrix3d mat = Matrix3d();
	mat[0][0] = mass * (b + c)/3;
	mat[1][1] = mass * (c + a)/3;
	mat[2][2] = mass * (a + b)/3;	
	return mat;
}

CDConvexMeshDesc CreateMeshBoxDesc(Vec3f s){
	CDConvexMeshDesc md;
	float x, y, z;
	x = s[0]; y = s[1]; z = s[2];
	md.vertices.push_back(Vec3f(-x,-y,-z));
	md.vertices.push_back(Vec3f(-x,-y, z));	
	md.vertices.push_back(Vec3f(-x, y,-z));	
	md.vertices.push_back(Vec3f(-x, y, z));
	md.vertices.push_back(Vec3f( x,-y,-z));	
	md.vertices.push_back(Vec3f( x,-y, z));
	md.vertices.push_back(Vec3f( x, y,-z));
	md.vertices.push_back(Vec3f( x, y, z));
	return md;
}


#endif