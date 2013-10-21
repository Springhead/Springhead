#include "m9Matrix.h"
#include "jacobiFun.h"
#include "Math3d.h"
//#include <cassert>
#include <climits>
#include <vector>
#include <cmath>
#include <map>

using namespace std;
  
const double THRESHOLD = 1E-8;
const int ITERATION = 30;   //迭代次数的上限
  
inline int JacobiFun::sign(double number)
{
    if (number < 0)
        return -1;
    else
        return 1;
}
  
void JacobiFun::rotate(Matrix < double >&matrix, int i, int j, bool & pass,
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
  
void JacobiFun::jacobi(Matrix < double >&matrix, int size, std::vector < double >&E,
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
    std::cout << "迭代次数：" << ITERATION - iteration << std::endl;
    for (int i = 0; i < size; ++i) {
        E[i] = matrix.get(i, i);
        if (E[i] < THRESHOLD)
            E[i] = 0.0;
    }
}
  
void JacobiFun::svd(Matrix < double >&A, Matrix < double >&U, Matrix < double >&V,
     std::vector < double >&E)
{
    int rows = A.getRows();
    int columns = A.getColumns();
   /* assert(rows <= columns);
    assert(U.getRows() == rows);
    assert(U.getColumns() == rows);
    assert(V.getRows() == columns);
    assert(V.getColumns() == columns);
    assert(E.size() == columns);*/
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
  
    //assert(num_eig <= rows);
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
  
int JacobiFun::begin(int argc, char *argv[])
{
    const int ROW = 2;
    const int COL = 3;
    //assert(ROW <= COL);
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