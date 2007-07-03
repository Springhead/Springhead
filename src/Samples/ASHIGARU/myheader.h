//
// 関数，構造体ヘッダファイル
//
// 最終更新日：2006/7/2
//

#include <math.h>

#define PI		3.1415926538

// ダイアログ作成や識別のための構造体
typedef struct{
	bool	flag;
	HWND	hDlg;
} DLG_INFO;

// 各脚の足先座標用構造体
typedef struct{
	double x;
	double y;
	double z;
} D3POINT;

// 各脚の関節角度用構造体
typedef struct{
	double theta1;
	double theta2;
	double theta3;
} JointAngle;

// ラジアンを度に変換
double Rad2Deg(double rad){

	return rad * 180.0 / PI;

}
// 度をラジアンに変換
double Deg2Rad(double deg){

	return deg * PI / 180.0;

}

// 足先座標からθ1を算出
double CalcTheta1(double x, double y){

	//return Rad2Deg( atan(y / x) );
	return atan(y / x);
}

// 足先座標からθ2を算出
double CalcTheta2(double x, double y, double z, double L1, double L2, double L3){

	double	m = sqrt(x*x + y*y) - L1;

	//double	theta2 = atan(z / m) - acos( (L2*L2 + m*m + z*z - L3*L3) / (2 * L2 * sqrt(m*m + z*z)) );
	double	theta2 = atan(z / m) + acos( (L2*L2 + m*m + z*z - L3*L3) / (2 * L2 * sqrt(m*m + z*z)) );

	//return	Rad2Deg( theta2 );
	return	theta2;

}

// 足先座標からθ3を算出
double CalcTheta3(double x, double y, double z, double L1, double L2, double L3){

	double	m = sqrt(x*x + y*y) - L1;

	//double	theta3 = PI - acos( (L2*L2 + L3*L3 - m*m - z*z) / (2 * L2 * L3) );
	double	theta3 = - PI + acos( (L2*L2 + L3*L3 - m*m - z*z) / (2 * L2 * L3) );

	//return	Rad2Deg( theta3 );
	return	theta3;

}