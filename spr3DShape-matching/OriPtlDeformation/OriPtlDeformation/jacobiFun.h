#ifndef JACOBIFUN_H
#define JACOBIFUN_H
#include "m9Matrix.h"
#include "Math3d.h"
#include <cassert>
#include <climits>
#include <vector>
#include <cmath>
#include <map>

using namespace std;
class JacobiFun
{
	inline int sign(double number);
	void jacobi(Matrix < double >&matrix, int size, vector < double >&E,
        Matrix < double >&J);
	void rotate(Matrix < double >&matrix, int i, int j, bool & pass,
        Matrix < double >&J);
	void svd(Matrix < double >&A, Matrix < double >&U, Matrix < double >&V,
     vector < double >&E);
	int begin(int argc, char *argv[]);

};

#endif