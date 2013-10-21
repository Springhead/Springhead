#include "m9Matrix.h"
#include<cmath>
#include<map>
  
#define EPSILON 1e-8
#define JACOBI_ITERATIONS 30


//---------------------------------------------------------------------
void m9Matrix::jacobiRotate(m9Matrix &A, m9Matrix &R, int p, int q)
//---------------------------------------------------------------------
{
	// rotates A through phi in pq-plane to set A(p,q) = 0
	// rotation stored in R whose columns are eigenvectors of A
	float d = (A(p,p) - A(q,q))/(2.0f*A(p,q));
	float t = 1.0f / (fabs(d) + sqrt(d*d + 1.0f));
	if (d < 0.0f) t = -t;
	float c = 1.0f/sqrt(t*t + 1);
	float s = t*c;
	A(p,p) += t*A(p,q);
	A(q,q) -= t*A(p,q);
	A(p,q) = A(q,p) = 0.0f;
	// transform A
	int k;
	for (k = 0; k < 9; k++) {
		if (k != p && k != q) {
			float Akp = c*A(k,p) + s*A(k,q);
			float Akq =-s*A(k,p) + c*A(k,q);
			A(k,p) = A(p,k) = Akp;
			A(k,q) = A(q,k) = Akq;
		}
	}
	// store rotation in R
	for (k = 0; k < 9; k++) {
		float Rkp = c*R(k,p) + s*R(k,q);
		float Rkq =-s*R(k,p) + c*R(k,q);
		R(k,p) = Rkp;
		R(k,q) = Rkq;
	}
}

void m9Matrix::jacobiRotate3D(m9Matrix &A, m9Matrix &R, int p, int q)
//---------------------------------------------------------------------
{
	// rotates A through phi in pq-plane to set A(p,q) = 0
	// rotation stored in R whose columns are eigenvectors of A
	float d = (A(p,p) - A(q,q))/(2.0f*A(p,q));
	float t = 1.0f / (fabs(d) + sqrt(d*d + 1.0f));
	if (d < 0.0f) t = -t;
	float c = 1.0f/sqrt(t*t + 1);
	float s = t*c;
	A(p,p) += t*A(p,q);
	A(q,q) -= t*A(p,q);
	A(p,q) = A(q,p) = 0.0f;
	// transform A
	int k;
	for (k = 0; k < 9; k++) {
		if (k != p && k != q) {
			float Akp = c*A(k,p) + s*A(k,q);
			float Akq =-s*A(k,p) + c*A(k,q);
			A(k,p) = A(p,k) = Akp;
			A(k,q) = A(q,k) = Akq;
		}
	}
	// store rotation in R
	for (k = 0; k < 9; k++) {
		float Rkp = c*R(k,p) + s*R(k,q);
		float Rkq =-s*R(k,p) + c*R(k,q);
		R(k,p) = Rkp;
		R(k,q) = Rkq;
	}
}

//---------------------------------------------------------------------
void m9Matrix::eigenDecomposition(m9Matrix &A, m9Matrix &R)
//---------------------------------------------------------------------
{
	// only for symmetric matrices!
	// A = R A' R^T, where A' is diagonal and R orthonormal

	R.id();	// unit matrix
	int iter = 0;
	while (iter < JACOBI_ITERATIONS) {	// 10 off diagonal elements//现在是20
		// find off diagonal element with maximum modulus
		int p,q;
		float a,max;
		max = -1.0f;
		for (int i = 0; i < 8; i++) {     ///not original why 4 in origin?
			for (int j = i+1; j < 9; j++) {
				a = fabs(A(i,j));
				if (max < 0.0f || a > max) {
					p = i; q = j; max = a;
				}
			}
		}
		// all small enough -> done
//		if (max < EPSILON) break;  debug
		if (max <= 0.0f) break;
		// rotate matrix with respect to that element
		jacobiRotate(A, R, p,q);
		iter++;
	}
}


//----------------------------------------------------------------------------
void m9Matrix::invert()
//----------------------------------------------------------------------------
{
	m9Matrix R, A = *this;
	eigenDecomposition(A, R);

	float d[9];
	int i,j,k;

	for (i = 0; i < 9; i++) {
		d[i] = A(i,i);
		if (d[i] != 0.0f) d[i] = 1.0f / d[i];
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			m2Real &a = (&r00)[i*9+j];
			a = 0.0f;
			for (k = 0; k < 9; k++)
				a += d[k]*R(i,k)*R(j,k);
		}
	}
}



using namespace std;
  
const double THRESHOLD = 1E-8;
const int ITERATION = 30;   //迭代次数的上限
  
inline int sign(double number)
{
    if (number < 0)
        return -1;
    else
        return 1;
}
  
void rotate(Matrix < double >&matrix, int i, int j, bool & pass,
        Matrix < double >&J)
{
    double ele = matrix.get(i, j);
    if (fabs(ele) < THRESHOLD) {
        return;
    }
    pass = false;
    double ele1 = matrix.get(i, i);
    double ele2 = matrix.get(j, j);
    int size=matrix.getRows();
    double tao = (ele1 - ele2) / (2 * ele);
    double tan = sign(tao) / (fabs(tao) + sqrt(1 + pow(tao, 2)));
    double cos = 1 / sqrt(1 + pow(tan, 2));
    double sin = cos * tan;
    Matrix < double >G(IdentityMatrix < double >(size, size));
    G.put(i, i, cos);
    G.put(i, j, -1 * sin);
    G.put(j, i, sin);
    G.put(j, j, cos);
    matrix = G.getTranspose() * matrix * G;
    J *= G;
}
  
void jacobi(Matrix < double >&matrix, int size, vector < double >&E,
        Matrix < double >&J)
{
  
    int iteration = ITERATION;
    while (iteration-- > 0) {
        bool pass = true;
        for (int i = 0; i < size; ++i) {
            for (int j = i+1; j < size; ++j) {
                rotate(matrix, i, j, pass, J);
            }
        }
        if (pass)   //当非对角元素全部变为0时迭代退出
            break;
    }
    cout << "迭代次数：" << ITERATION - iteration << endl;
    for (int i = 0; i < size; ++i) {
        E[i] = matrix.get(i, i);
        if (E[i] < THRESHOLD)
            E[i] = 0.0;
    }
}
  
void svd(Matrix < double >&A, Matrix < double >&U, Matrix < double >&V,
     vector < double >&E)
{
    int rows = A.getRows();
    int columns = A.getColumns();
    assert(rows <= columns);
    assert(U.getRows() == rows);
    assert(U.getColumns() == rows);
    assert(V.getRows() == columns);
    assert(V.getColumns() == columns);
    assert(E.size() == columns);
    Matrix < double >B = A.getTranspose() * A;    //A的转置乘以A，得到一个对称矩阵B
    Matrix < double >J(IdentityMatrix < double >(columns, columns));
    vector < double >S(columns);
    jacobi(B, columns, S, J);   //求B的特征值和特征向量
    for (int i = 0; i < S.size(); ++i)
        S[i] = sqrt(S[i]);  //B的特征值开方后得到A的奇异值
  
    /*奇异值按递减排序，对应的V中的特征向量也要重排序 */
    multimap < double, int >eigen;
    for (int i = 0; i < S.size(); ++i)   //在multimap内部自动按key进行排序
        eigen.insert(make_pair(S[i], i));
    multimap < double, int >::const_iterator iter = --eigen.end();
    int num_eig = 0;    //记录非0奇异值的个数
    for (int i = 0; i < columns; ++i, iter--) {  //反向遍历multimap,使奇异值从大到小排序
        int index = iter->second;
        E[i] = S[index];
        if (E[i] > THRESHOLD) {
            num_eig++;
        }
        for (int row = 0; row < columns; ++row)
            V.put(row, i, J.get(row, index));
    }
  
    assert(num_eig <= rows);
    for (int i = 0; i < num_eig; ++i) {
        Matrix < double >vi = V.getColumn(i); //获取V的第i列
        double sigma = E[i];
        Matrix < double >ui(rows, 1);
        ui = A * vi;
        for (int j = 0; j < rows; ++j) {
            U.put(j, i, ui.get(j, 0) / sigma);
        }
    }
      
    //U矩阵的后(rows-none_zero)列就不计算了，采用默认值0。因为这后几列对应的奇异值为0,在做数据压缩时用不到这几列   
}
  
int begin(int argc, char *argv[])
{
    const int ROW = 2;
    const int COL = 3;
    assert(ROW <= COL);
    double arr[ROW * COL] = {1,1,0,0,0,1};
    Matrix < double >A(ROW, COL);
    A = arr;
  
    Matrix < double >U(ROW, ROW);
    Matrix < double >V(COL, COL);
    vector < double >E(COL);
    svd(A, U, V, E);
    Matrix < double >Sigma(ROW, COL);
    for (int i = 0; i < ROW; ++i)
        Sigma.put(i, i, E[i]);
  
    cout << "U=" << endl << U;
    cout << "SIGMA=" << endl << Sigma;
    cout << "V^T=" << endl << V.getTranspose();
  
    Matrix < double >A_A = U * Sigma * V.getTranspose();
    cout << "reset A=" << endl << A_A;
  
    return 0;
}