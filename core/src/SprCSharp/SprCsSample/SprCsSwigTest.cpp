#include <Physics/PHFemMeshNew.h>

namespace Spr{;

CSTestFunction::CSTestFunction() {}

// intrinsic type
int CSTestFunction::intrinsic_int(int arg) {
	i = 9999;
	i = arg;
	return i;
}
unsigned int CSTestFunction::intrinsic_unsigned_int(unsigned int arg) {
	u = 9999;
	u = arg;
	return u;
}
size_t CSTestFunction::intrinsic_size_t(size_t arg) {
	s = 9999;
	s = arg;
	return s;
}
float CSTestFunction::intrinsic_float(float arg) {
	f = 9999.9F;
	f = arg;
	return f;
}
double CSTestFunction::intrinsic_double(double arg) {
	d = 9999.9;
	d = arg;
	return d;
}
std::string CSTestFunction::intrinsic_string(std::string arg) {
	str = "abc";
	str = arg;
	return str;
}

// struct
Vec3d CSTestFunction::struct_vec3d(Vec3d arg) {
	Vec3d* defval = new Vec3d(1.1, 2.2, 3.3);
	vec3d = *defval;
	vec3d = arg;
	return vec3d;
}

// std::vector
std::vector<int> CSTestFunction::vector_int(std::vector<int> arg) {
	for (int i = 0; i < 3; i++) {
		vec_int.push_back(arg[i]);
	}
	return vec_int;
}
std::vector<Vec3d> CSTestFunction::vector_vec3d(std::vector<Vec3d> arg) {
	for (int i = 0; i < 3; i++) {
		vec_vec3d.push_back(arg[i]);
	}
	return vec_vec3d;
}

// array
int* CSTestFunction::array_int(int arg[]) {
	for (int i = 0; i < 3; i++) {
		ary_int[i] = arg[i];
	}
	return ary_int;
}
Vec3d* CSTestFunction::array_vec3d(Vec3d arg[]) {
	for (int i = 0; i < 3; i++) {
		ary_vec3d[i] = arg[i];
	}
	return ary_vec3d;
}

} // namespace Spr
