#ifndef __THERMALCONDACTION_H_INCLUDED__
#define __THERMALCONDACTION_H_INCLUDED__

#include "Tetrahedron.h"
#include "MatrixCalc.h"
#include <iostream>
#include <vector>
#include "tetgen.h"


struct ElemAreas	//要素の各面の面積値の格納
{
	REAL facet1;
	REAL facet2;
	REAL facet3;
	REAL facet4;
};

struct OptionalPoint{
	REAL x;
	REAL y;
	REAL z;
	REAL Tp;
	REAL N1;
	REAL N2;
	REAL N3;
	REAL N4;
	
	int node1;
	int node2;
	int node3;
	int node4;
};
struct Belong{
	std::vector<int> element;
};

class ThermalFEM
{
private:
	//int elemNum;						//要素数を保存
	//int nodeNum;						//節点数を保存
	//int *elems;							//要素の構成を保存する
	//REAL *nodeCoordinates;			//節点座標を保存する
	REAL *mat;						//全体のマトリックスを保存するところ
	REAL *K1;							//熱伝導マトリックスの第一項(熱伝導に関する項)
	REAL *K2;							//熱伝導マトリックスの第二項(熱伝達率に関する項)
	REAL *K3;							//熱伝導マトリックスの第三項(輻射に関する項)
	REAL *CapaMat;					//熱容量マトリックス
	REAL *CGlobal;					//熱容量マトリックス×dt

	REAL *FluxMat;					//熱流束マトリックス
	REAL *F1;							//熱流束の式のうち内部発熱に関する項を保存
	REAL *F2;							//熱流束の式のうち熱流束q0に関する項を保存
	REAL *F3;							//熱流束の式のうち熱伝達率と周囲流体温度に関する項を保存
	REAL *F4;							//熱流束の式のうち輻射に関する項を保存

	REAL conductivity;				//熱伝導率
	REAL *Q;							//内部発熱率
	REAL density;						//密度
	REAL c;							//比熱
	REAL dt;							//時間刻み幅
	REAL q0;							//熱流束
	REAL h;							//熱輻射率
	REAL alpha;						//熱伝達率

	REAL Tfluid;						//周囲流体温度
	REAL Tout;						//周囲環境温度
	int qdirection;						//
	

	Tetrahedron tet;
	MatrixCalc mc;

	bool usedRadiantHeat;
	bool usedGenerationOfHeat;
	bool usedElem;
	bool usedNode;

	REAL eta0ds(int node1,int node2,int node3,int node4);
	REAL eta0ds(int nodes[]);

	void CreateK1Local(int elemNumber, REAL ktemp[]);		//K1のマトリックスを作成する。elemNumber:要素番号, ktemp:長さ16の配列

	void CreateK2Local(int node1, int node2, int node3, int node4, REAL ktemp[]);	//k2の局所座標系での行列を作成する。
																					//ktemp:長さ16の配列
	void CreateK3Local(int node1, int node2, int node3, int node4, REAL ktemp[]); //k3の局所座標系での行列を作成する。
																					//ktemp:長さ16の配列
	void CreateCapacityLocal(int elemNumber,REAL ktemp[]);						//局所座標系での熱容量マトリックス
																					//ktemp:長さ16の配列
	void CreateQVecLocal(int elemNumber, REAL temp[],int n[]);					//局所座標系での内部発熱のベクトルを作成する
	void CreateHeatFluxVecLocalDS(int nodeNums[],REAL temp[]);					//局所座標系での
	void checkSurface(int elemNumber,int node1,int node2,int node3,int n[]);		//どの面から熱流束が入っているか確認
	int  checkSurface(int elemNumber, int node1, int node2, int node3);				//戻り値k=0,1,2,3で(0,1,2),(0,1,3),(0,2,3),(1,2,3)のどの節点の組み合わせか分かる

	void GetNodeNumbers(int elemNumber,int n[]);									//要素を構成する節点の番号を取得する
	void sortInt(int n[],int num);

	//任意の点の値を取得する時に使う
	Belong *belong;		//所属している要素を保存する。
	OptionalPoint *optional;	//任意の点の値をとる際に用いる
	bool usedOptional;			//このあたりの関数を使ったかどうか
	int totalOP;				//任意点の総数
	bool InOrOut(REAL target[],REAL v1[],REAL v2[],REAL v3[],REAL v4[]);			//要素メッシュの中に入っているのかどうか
	void CreateShapeFunction(int elemNumber, REAL a[],REAL b[],REAL c[],REAL d[]);	//形状関数作成
	
	
	//面積計算
	void initializeElemAreas();
	bool usedBorderCode;
	bool usedElemAreas;

	//仕様変更により追加になった部分
	void SetInit_relateNode(int totalNode);

	//20090515修正
	bool useTETGEN;

public:
	ThermalFEM();					//コンストラクタ
	~ThermalFEM();					//デストラクタ

	

	REAL *temperature;					//温度を保存する

	//使うかどうかの判定用
	bool useRadiantHeat;					//輻射の項を使うかどうかの設定。true:使う, false:使わない
	bool useGenerationOfHeat;					//内部発熱を使うかどうかの設定。true:使う, false:使わない

	//条件の設定関係
	void SetTotalNodes(int totalNode);									//総節点数を保存する
	void SetTotalElems(int totalElem);									//総要素数を保存する
	void SetElemAndNodeNum(int totalElem,int TotalNode);				//要素数と節点数を保存する
	void SetInitialTemperature(REAL temp[]);							//初期の温度を設定する
	void SetConductivity(REAL conduct);								//熱伝導率を設定する
	void SetQ(REAL *qi);												//内部発熱率を設定する
	void SetDensity(REAL dens);										//密度を設定する
	void SetSpecificTemp(REAL st);									//比熱を設定する
	void SetDT(REAL dt0);												//刻み幅を設定する
	void SetHeatflux(REAL flux);										//熱流束の値を設定する。
	void SetHeatflux(REAL flux,int direction);						//熱流束の値を設定する。direction:-1 外から解析対象内部へ向かう方向,+1:解析対象内部から外へ向かう方向
	void SetTransRatio(REAL ratio);									//熱伝達率の値を設定する
	void SetRadiantHeat(REAL radiantHeat);							//熱輻射率と輻射のある要素の設定
	void SetTfluid(REAL tfluid);										//周囲流体温度の設定
	void SetTout(REAL tout);											//周囲環境温度の設定
	void SetTemperature(REAL temp,int nodeNumber);					//nodeNumber番の節点の温度を設定する
	void SetHeatfluxDirection(int direction);							//流速の方向を設定する direction:-1 外から解析対象内部へ向かう方向,+1:解析対象内部から外へ向かう方向

	//標示用
	void showGlobalMatrix();						//全体マトリックスを表示する
	void showTemperature();				//温度ベクトルを表示する
	void showK1();
	void showK2();
	void showK3();
	void showCapacityMat();
	void showF1();
	void showF2();
	void showF3();
	void showF4();
	void showGlobalFluxVector();

	void initializeK1();
	void initializeK2();												//熱伝導マトリックス第2項(熱伝達率に関する項)を初期化する
	void CreateK2(int elemNumber, int node1, int node2, int node3);		//熱伝導マトリックス第2項(熱伝達率に関する項)を作成する
	void initializeK3();												//熱伝導マトリックス第3項(輻射に関する項)を初期化する
	void CreateK3(int elemNumber, int node1, int node2, int node3);		//熱伝導マトリックス第3項(輻射に関する項)を作成する
	void CreateK1();												//熱伝導マトリックス第1項(熱伝導率に関する項)を作成する
	void CreateCapacityMat();										//全体座標系での熱容量マトリックスを作成する
	void initializeCapacityMat();
	void CreateHeatFluxVecNo1();										//熱流束ベクトルの式第1項を作成する
	void CreateHeatFluxVecNo1(int elem);								//熱流束ベクトルの式第1項を作成する
	void CreateHeatFluxVecNo1(int elem, REAL genHeat);				//熱流束ベクトルの式第1項を作成する. 
	void CreateHeatFluxVecNo2(int elem,int node1,int node2,int node3);	//熱流束ベクトルの式第2項を作成する
	void CreateHeatFluxVecNo2(int elem,int node1,int node2,int node3,REAL qelem);	//熱流束ベクトルの式第2項を作成する
	void CreateHeatFluxVecNo3(int elem,int node1,int node2,int node3);	//熱流束ベクトルの式第3項を作成する
	void CreateHeatFluxVecNo3(int elem,int node1,int node2,int node3,REAL tfruid);	//熱流束ベクトルの式第3項を作成する
	void CreateHeatFluxVecNo4(int elem,int node1,int node2,int node3);	//熱流束ベクトルの式第4項を作成する
	void CreateHeatFluxVecNo4(int elem,int node1,int node2,int node3,REAL tout);	//熱流束ベクトルの式第4項を作成する
	void initializeFluxVecNo1();
	void initializeFluxVecNo2();
	void initializeFluxVecNo3();
	void initializeFluxVecNo4();

	void CreateGlobalMatrixAndVec();									//全体行列と全体ベクトルを作成する
	void CalcStartGaussJordan();										//ガウスジョルダン法で計算する
	void CalcStartGaussJordanUsePivot();								//ピボット付きガウスジョルダン法で計算する
	void CalcStartLUSolve();											//LU分解をして計算する
	void FixTemperature(REAL temp, int nodeNumber);					//温度を固定して計算する

	//境界検索
	void detectBorder(int s[]);		//要素数の長さのポインタを格納
	void detectBorder();

	//値を返す
	void GetGlobalMatrix(REAL ret[]);
	void GetK1Matrix(REAL ret[]);
	void GetK2Matrix(REAL ret[]);
	void GetK3Matrix(REAL ret[]);
	void GetCapacityMatrix(REAL ret[]);
	void GetGlobalFluxVector(REAL ret[]);
	void GetHeatFlux1Vector(REAL ret[]);
	void GetHeatFlux2Vector(REAL ret[]);
	void GetHeatFlux3Vector(REAL ret[]);
	void GetHeatFlux4Vector(REAL ret[]);
	void GetTemperature(REAL ret[]);
	void GetAnElementMember(int elementNumber,int n[]);
	REAL GetElemVolume(int elemNumber);				//elemnumberは１以上.接点の数と要素の構成、接点の座標と要素の構成の設定が終わった後に使用できる。全体の流れの②番まで終わった後.

	//節点座標、要素構成の格納
	void SetANodeCordinates(int nodeNumber, REAL x,REAL y,REAL z);
	void SetAnElementMember(int elementNumber,int node1,int node2,int node3, int node4);
	void SetTetrahedronlistStartWithZero(int tetrahedronlist[]);		//すべての要素構成を保存する
	void SetTetrahedronlist(int tetrahedronlist[]);						//すべての要素構成を保存する
	void SetPointlist(REAL pointlist[]);								//すべての節点座標を保存する
	
	//その他の機能
	void GetExteriorProduct(int elementNumber,int node1,int node2,int node3, REAL ret[]);//要素内のある面の外積を求める
	void GetElementFace(int elemNumber,int faceNumber,int n[]);//要素の4つの面のfaceNumber番目の面の構成を返す。faceNumberは1～4。n[]は長さ3の配列。

	//任意の点の値をとるための関数達	//TetGen()で作ったメッシュ頂点から
	int SerchNearestPoint(REAL x,REAL y,REAL z);
	void PrepareforGetOptionalPoint(int totalofOptionalPoint);						//1)
	void SetOptionalPointCoordinates(int number,REAL x,REAL y,REAL z);		//2)任意の点の座標を指定する関数群３つのうちどれでも良い.numberは０以上で、一続きの番号
	void SetOptionalPointCoordinates(int number,REAL xyz[]);						//[]
	void SetOptionalPointCoordinates(REAL AllCoodinates[]);						//[]
	void SerchAnOptionalPointBelong(int optionalPointNumber);						//
	void CreateOptionalPointParameters(int elemNumber,int optionalPointNumber);		//
	void SerchAllOptionalPointBelong();												//
	REAL GetOptionalPointTemperature(int OptionalPointNumber);					//OptionalPointNumber:特定の任意の点を１つ引数に入れる
	void GetOptionalPointTemperature(REAL optionalPointTemperatures[]);			//
	
	//面積計算
	int *borderCode;//面の情報を保存
	ElemAreas *elemFaceAreas;	//初期化
	void calcElemFaceArea(int elemNumber, int code);	//外殻の特定の要素だけ計算　elemNumber 1以上の要素番号	//code 0x(4桁の数字) 外側の境界面の面積を計算
	void calcElemFaceArea(int code[]);					//外殻の全部の要素で計算
	void calcElemFaceArea();							//全要素の全面を計算

	tetgenio out,in;
	void TFEMTetrahedralize(char *switches);
	void TFEMTetrahedralize(char *switches,tetgenio in);
	void TFEMTetrahedralize(char *switches,tetgenio in,tetgenio out);
	
};


#endif

