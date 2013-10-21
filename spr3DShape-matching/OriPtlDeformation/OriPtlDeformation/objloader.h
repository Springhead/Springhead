#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <Springhead.h>

using namespace Spr;
bool loadOBJ(
	const char * path, 
	std::vector<Vec3f> & draw_vertices,
	std::vector<Vec3f> & out_vertices, 
	std::vector<Vec2f> & out_uvs, 
	std::vector<Vec3f> & out_normals
);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<Vec3f> & vertices,
	std::vector<Vec2f> & uvs,
	std::vector<Vec3f> & normals
);

#endif