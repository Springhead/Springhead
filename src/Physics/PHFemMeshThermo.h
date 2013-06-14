/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESHTHERMO_H
#define PHFEMMESHTHERMO_H

#include "PHFemMesh.h"
#include "PhysicsDecl.hpp"

namespace Spr{;

///	熱伝導FEM計算クラス
class PHFemMeshThermo: public PHFemMesh{
	SPR_OBJECTDEF(PHFemMeshThermo);
	SPR_DECLMEMBEROF_PHFemMeshThermoDesc;

public:
	//	メンバ変数宣言
	//	頂点
	struct StateVar{
		double temperature;
	};
	struct Coeff{
	};

	enum HEATING_MODE{
		OFF,
		WEEK,
		MIDDLE,
		HIGH
	};

	std::vector<StateVar> vertexVars;
	std::vector<Coeff> edgeCoeffs;

protected:

	
	//	行列の宣言
	//PHFemMeshThermo内のみで用いる計算

	//%%%%%%%%		行列の宣言・定義		%%%%%%%%//	
	//行列の生成　小文字は要素剛性行列、大文字は全体剛性行列
	//節点温度ベクトルの	温度の	TVec:要素（縦）ベクトル(4x1)	TVecall:全体（縦）ベクトル(nx1)
	//熱伝導マトリクスの	k:k1~k3の加算したもの,	k1:体積分項,	面積分の	k2:熱伝達境界条件,	k3:熱輻射境界条件	積分
	//熱容量マトリクスの	c:体積分項
	//熱流束ベクトルの		f:f1~f4の加算したもの,	体積分の	f1:内部発熱による項,	面積分の	f2:熱流束境界条件,	f3:熱伝達境界条件,	f4:熱輻射境界条件
	//体積分の場合:要素1つにつき1つの行列、面積分の場合:要素内の各面ごとに1つで計4つの行列なので配列に入れる
	//kやfに関しては、面ごとに計算した係数行列を格納する配列Mat(k/f)arrayを定義
	//	Col:列単位の行列	Row:行単位の行列	hogeVector:ベクトル

	//節点温度ベクトル
	PTM::TVector<4,double> TVec;				///>	要素の節点温度ベクトル		//_		//不要?
	PTM::VVector<double> TVecAll;				///>	全体の節点温度ベクトル

	//要素の係数行列
//	PTM::TMatrixRow<4,4,double> matk1;			//CreateMatk1k() / k1b				///	struct Tetへ移植
//	PTM::TMatrixRow<4,4,double> matk2;			//CreateMatk2()						///	
	//int Matk2array[4];						//matk2が入った配列		//CreateMatk2array()
	//Kmの3つの4×4行列の入れ物　Matk1を作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> matk1array[4];
	//k21,k22,k23,k24の4×4行列の入れ物　Matkを作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> matk2array[4];
	//k21,k22,k23,k24の4×4行列の入れ物　Matkを作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> matk3array[4];

	//Matk=Matk1+Matk2+Matk3	matk1~3を合成した要素剛性行列	CreateMatkLocal()
	PTM::TMatrixRow<4,4,double> matk;			
	PTM::TMatrixRow<4,4,double> matc;			//
//	PTM::TMatrixCol<4,1,double> Vecf3;			//f3:外側の面に面している面のみ計算する　要注意
//	PTM::TMatrixCol<4,1,double> Vecf3array[4];	//f31,f32,f33,f34の4×1ベクトルの入れ物		Matkを作るまでの間の一時的なデータ置場
//	PTM::TMatrixCol<4,1,double> Vecf;			//f1~f4を合算した縦ベクトル
	
	//f3:外側の面に面している面のみ計算する　要注意
	PTM::TVector<4,double> vecf3;				// 熱伝達境界条件
	//f31,f32,f33,f34の4×1ベクトルの入れ物		Matkを作るまでの間の一時的なデータ置場
	PTM::TVector<4,double> vecf3array[4];
	//f1~f4を合算した縦ベクトル
	PTM::TVector<4,double> vecf;				// 熱
	PTM::TVector<4,double> vecf2;				// 熱流束境界条件
	PTM::TVector<4,double> vecf2array[4];		//
//	PTM::VVector<double> Vechoge;
	//	変数は小文字　関数は大文字

	//行列kの計算用の係数行列
	PTM::TMatrixRow<3,3,double> mata;
	PTM::TMatrixRow<3,3,double> matb;
	PTM::TMatrixRow<3,3,double> matcc;
	PTM::TMatrixRow<3,3,double> matd;


	// 全体の係数行列	//SciLabで使用
	// ..[K]の全体剛性行列		//CreateMatKall()
	PTM::VMatrixRow<double> matKAll;
	// ..[C]
	PTM::VMatrixRow<double> matCAll;

	PTM::VMatrixRow<double> tempMat;

	// 全体の剛性行列の代わり
	// ..全体剛性行列Kの対角成分になるはずの値をそのまま代入		実装中での初期化の仕方	DMatKAll.resize(1,vertices.size());
	PTM::VMatrixRow<double> dMatKAll;
	// ..全体剛性行列Cの対角成分
	PTM::VMatrixRow<double> dMatCAll;
	// ..全体剛性行列KとCの対角成分の定数倍和の逆数をとったもの	ガウスザイデルの計算に利用する
	PTM::VMatrixRow<double> _dMatAll;
	// ..ガウスザイデルの計算に用いる定数行列bの縦ベクトル	Rowである必要はあるのか？⇒Colにした
	PTM::VMatrixCol<double> bVecAll;
	PTM::VMatrixCol<double> bVecAll_IH;
		//double *constb;								//ガウスザイデルの係数bを入れる配列のポインタ	後で乗り換える
	//	..{F}の 全体剛性行列(ベクトル)
	PTM::VVector<double> vecFAllSum;			//	PTM::VMatrixCol<double> vecFAll;	// から変更				
	PTM::VVector<double> vecFAll[4];			// 全体剛性ベクトルを格納しておくベクトル
	PTM::VMatrixCol<double> vecFAll_f2IHw;		// 弱火ベクトル
	PTM::VMatrixCol<double> vecFAll_f2IHm;		// 中火
	PTM::VMatrixCol<double> vecFAll_f2IHs;		// 強火
	PTM::VMatrixCol<double> vecFAll_f2IH[4];		// 全体剛性ベクトルを弱火、中火、強火の順に入れる配列
	PTM::VMatrixCol<double> vecFAll_f3;			// f3

	// 表示用	//デバッグ
	Vec2d IHLineVtxX;		//	IH加熱の境界線を引く	x軸の最小地、最大値が格納

public:
	//%%%		関数の宣言		%%%%%%%%//
	//%%%%%%		熱伝導計算本体		%%%//
	//	熱伝達境界条件の時はすべての引数を満たす必要がある。
	//	温度固定境界条件を用いたいときには、熱伝達率（最後の引数）を入力しない。また、毎Step実行時に特定節点の温度を一定温度に保つようにする。
	void SetInitThermoConductionParam(
		double thConduct,		// thConduct:熱伝導率
		double rho,				// roh:密度
		double specificHeat,	// specificHeat:比熱 J/ (K・kg):1960
		double heatTrans		// heatTrans:熱伝達率 W/(m^2・K)
		);

	void SetThermalEmissivityToVtx(unsigned id,double thermalEmissivity);			///	熱放射率を節点 id に設定する関数
	void SetThermalEmissivityToVerticesAll(double thermalEmissivity);					///	熱放射率を全節点に設定

	void SetHeatTransRatioToAllVertex();	//SetInit で設定している熱伝達係数を、節点(FemVertex)の構造体のメンバ変数に代入

	void InitCreateMatC();			///	行列作成で用いる入れ物などの初期化
	void InitVecFAlls();			///	Vecfの作成前に実行する初期化処理 modify@ 2012.10.4
	void InitCreateMatk();			///	Matkの作成前に実行する初期化処理

	///	熱伝達率が変化した時などの再計算用の初期化関数
	void InitCreateVecf_();				
	void InitCreateMatk_();

	//	[K]:熱伝導マトリクスを作る関数群
	//	..kimura式を参考にして(惑いながら)導出した計算法			//>	k1ktに改称する
	void CreateMatk1k(unsigned id);
	//	..yagawa1983の計算法の3次元拡張した計算法 b:book の意味	//>	k1btに改称する
	void CreateMatk1b(unsigned id);
	//	..四面体ごとに作るので、四面体を引数に取る 内外すべての四面体について行う
	void CreateMatk2(unsigned id,Tet tets);
	//void CreateMatk2f(Face faces);		// 四面体ごとに作る式になっているが、外殻の三角形face毎に作る　facesのf
	//	..四面体ごとに作る　tetsのt
	void CreateMatk2t(unsigned id);
	//	..四面体(tets)のt 毎に生成
	void CreateMatk3t(unsigned id);

	void CreateMatk2array();
	void CreateMatkLocal(unsigned i);			//	edgesに入れつつ、チェック用の全体剛性行列も、ifdefスイッチで作れる仕様
//	void CreateDumMatkLocal();					//	全要素が0のダミーk
	void CreateMatKall();
	void CreateMatKAll();						//	Kの全体剛性行列	//	SciLab	で用いる
	

	//	[C]:熱容量マトリクスを作る関数
	void CreatedMatCAll(unsigned id);			//	matC1,C2,C3・・・毎に分割すべき？
	void CreateMatc(unsigned id);				// cの要素剛性行列を作る関数

	void CreateMatCAll();						//	Cの全体剛性行	//	SciLab	で用いる

	//	{F}:熱流束ベクトルを作る関数
	void CreateVecFAll(unsigned id);				//	四面体メッシュのIDを引数に
	void CreateVecf3(unsigned id);					//	熱伝達率は相加平均、周囲流体温度は各々を形状関数に？
	void CreateVecf3_(unsigned id);					//	熱伝達率も、周囲流体温度も相加平均
	void CreateVecf2(unsigned id);					//	四面体のIDを引数に
	void CreateVecf2surface(unsigned id);			//> 四面体IDに含まれるfaceの内、表面のfaceについてだけ計算 加熱量は弱火モードで設定
	void CreateVecf2surface(unsigned id,unsigned mode);			//> 同上　加えて、vecFAll_f2IH[num]に格納、OFF、弱火、中火。強火の時は、mode = 4
	void CreateVecF2surfaceAll();					//	IH等の加熱条件設定から、全体剛性ベクトル(・行列)(何×何？)を作る関数　2012.08.30追記
		//CreateVecfLocal(unsigned id);を改造
	void CreateVecF3surfaceAll();
	

	//	{T}:節点温度ベクトルを作る関数
	void CreateTempMatrix();					//節点の温度が入った節点配列から、全体縦ベクトルを作る。	この縦行列の節点の並び順は、i番目の節点IDがiなのかな
	void CreateLocalMatrixAndSet();				//K,C,Fすべての行列・ベクトルについて要素剛性行列を作って、エッジに入れる	又は	全体剛性行列を作る関数
	
	//	初期化
	void InitTcAll(double temp);							//	Tcの温度を初期化

public:
	//	毎Step呼び出す
	void UpdateIHheat(unsigned heatingMODE);	// 毎Step呼ぶ：熱流束ベクトル{F}を生成・保存	//.heatingMODEは加熱強さ：4段階（0:OFF・1:弱火(WEEK)・2:中火(MIDDLE)・3:強火(HIGH)）
	void UpdateVecF_frypan();			// 方程式を解く前に、熱流束ベクトルをロードして、結合するなどベクトルを作る。modeには加熱モードを入れる
	void UpdateVecF();		// 食材用

protected:
	//	何用に用いる？	行列作成の関数をまとめるだけ？
	void CreateMatrix();					

	//%%%		k,c,f共通で用いる計算関数		%%%//
	// エッジや頂点にk,c,fの要素剛性行列の係数を設定	すべての四面体について要素剛性行列を求め、k,c,fに値を入れると、次の要素について処理を実行
	void SetkcfParam(Tet tets);
	// 頂点ID　３点から成る三角形の求積　(四面体の三角形面積分等で利用)
	double CalcTriangleArea(int id0, int id2, int id3);		
	double CalcTetrahedraVolume(Tet tets);			// 四面体のIDを入れると、その体積を計算してくれる関数
	//void PrepareStep();							// Step()で必要な変数を計算する関数
	//double CalcbVec(int vtxid,
	
	//	（クランクニコルソン法を用いた）ガウスザイデル法で熱伝導を計算
	void CalcHeatTransUsingGaussSeidel(
		unsigned NofCyc,	// NofCyc:繰り返し計算回数
		double dt			// dt:ステップ時間
		);
	// （前進・クランクニコルソン・後退積分に対応)ガウスザイデル法で熱伝導を計算 
	void CalcHeatTransUsingGaussSeidel(
		unsigned NofCyc,		// NofCyc:繰り返し計算回数
		double dt,				// dt:ステップ時間
		double eps				// eps:積分の種類 0.0:前進積分,0.5:クランクニコルソン差分式,1.0:後退積分・陰解法
		);

	void SetTempAllToTVecAll(unsigned size);		//	TVecAllに全節点の温度を設定する関数
	void SetTempToTVecAll(unsigned vtxid);			//	TVecAllに特定の節点の温度を設定する関数
	void UpdateVertexTemp(unsigned vtxid);			//	計算結果としての温度をTVecAllから節点に更新する
	void UpdateVertexTempAll(unsigned size);		//	計算結果としての温度をTVecAllから全節点に更新する

	//	SciLab
	void ScilabTest();								//	Scilabを使ってみる関数



	void TexChange(unsigned id,double tz);		//		GRへ移植　やり方を先生に聞く
	void SetTexZ(unsigned id,double tz);		//	テクスチャ座標を変更

	void HeatTransFromPanToFoodShell();									//	伝導伝熱：食材の外殻のフライパンや鉄板に近い節点から熱が伝わる		//	食材、調理器具を引数に
	
	//	放射伝熱を有効にしたい食材のFWFemMesh？のIf、加熱器具のFWFemMeshのIf,熱輻射の浸透深さを代入
	//	..放射伝熱：食材の外殻から数ミリ以内の節点に対して、加熱器具から熱が伝わる
	void HeatRadientToFood(char foodID,char panID,double fluxdepth);
												//
	PTM::TMatrixRow<4,4,double> Create44Mat21();	//共通で用いる、4×4の2と1でできた行列を返す関数
	//あるいは、引数を入れると、引数を変えてくれる関数
	PTM::TMatrixCol<4,1,double> Create41Vec1();		//共通で用いる、4×1の1でできた行列(ベクトル)を返す関数

	//%%%%%%%%		バイナリスイッチの宣言		%%%%%%%%//
	///	PHFemMeshに属する構造体、クラスで定義されている同様のboolが更新されたら、こちらも更新する
	///	条件利用後にはfalseに戻す。初期値はtrue	for	初期化
	bool deformed;					///	形状が変わったかどうか		///	構造体に持っていてほしい
	bool alphaUpdated;				///	熱伝達率が更新、matk2,Vecf3等が更新されたか
	bool doCalc;					///	ガウスザイデル計算するフラグ
	
	std::ofstream templog;		// 頂点の温度変化
	std::ofstream cpslog;		//	CPSの変化


	//%%%	アプリケーション	%%%//

	PHSolidIf* phFloor;
	PHSolidIf* phFood;
	//FWObjectIf					fwpan;
	//FWObjectIf*					fwpan;
	//UTRef<FWObjectIf*>			fwpan_;


public:
	//	PHFemMEsh.h内のクラスから熱伝導特有のパラメータを継承して使う

	//	継承例　以下のように継承して、熱伝導特有のパラメータを用いる
	//class thFemVertex :public FemVertex{
	//	double heatflux2;
	//};
	//std::vector<thFemVertex> thvertices;
	//	上記継承で近日中に、熱計算パラメータを継承する。振動FEM解析でも、上位クラス・構造体が使えるように準備しておく

	/////	FemMeshThermo
	//struct FemMeshThermo{		
	//	bool alphaUpdated;			///	メッシュ内の一つでも、節点の熱伝達率が更新されたかどうか→K2,F3に影響
	//	bool hUpdated;				///	熱輻射率が更新されたか
	//};

	
	// 加熱計算等を呼び出す Set系関数
	PHFemMeshThermo(const PHFemMeshThermoDesc& desc=PHFemMeshThermoDesc(), SceneIf* s=NULL);
	void AfterSetDesc();		//	伝熱行列の計算など、IH加熱モードは次の関数で設定
	/// 熱伝導シミュレーションでエンジンが用いるステップをオーバーライド		
	void Step(double dt);
	//（節点温度の行列を作成する前に）頂点の温度を設定する（単位摂氏℃）
	void SetVerticesTempAll(double temp);
	// 節点iの温度をtemp度に設定し、それをTVEcAllに反映
	void SetVertexTemp(unsigned i,double temp);
	//	接点iの周囲の節点温度をtemp度に設定
	void SetLocalFluidTemp(unsigned i,double temp);
	///	頂点の熱伝達率を更新し、行列を作り直す
	void UpdateheatTransRatio(unsigned id,double heatTransRatio);

	// If としての実装
	void PHFemMeshThermo::AddvecFAll(unsigned id,double dqdt);
	// AddVecFAllが完成したら、消す
	void PHFemMeshThermo::SetvecFAll(unsigned id,double dqdt);

	//%%%%%%%%		熱伝導境界条件の設定関数の宣言		%%%%%%%%//
	void SetThermalBoundaryCondition();				//	熱伝導境界条件の設定

	//%%%%%%	使い方		%%%%%//
	//	温度境界条件　を設定関数群
	//	..温度固定境界条件:S_1		指定節点の温度を変える
	void UsingFixedTempBoundaryCondition(unsigned id,double temp);
	//	..熱伝達境界条件:S_3		指定節点の周囲流体温度を変える
	void UsingHeatTransferBoundaryCondition(unsigned id,double temp);
	//	熱伝達率も設定可能な関数	//>	名前を変えるべき　要改善
	void UsingHeatTransferBoundaryCondition(unsigned id,double temp,double heatTransratio);
	
	//	以下、考え途中
	//	改称案
	//SetVtxtoFixedBC
	//SetVtxtoTranferBC
	//SetVtxHTRtoTranferBC
	//	温度を加えたい節点や節点周りの流体は、その節点のx,y,z座標から特定すればいい？	温度境界条件Tcの設定など

	///	凡例->wiki
	//BC=BoundaryCondition
	//HTR=HeatTransferRatio

	int GetSurfaceVertex(int id){return surfaceVertices[id];};
	int NSurfaceVertices(){return int(surfaceVertices.size());};
	//	周囲流体温度を更新	熱伝達率は変えない
	void SetVertexTc(int id,double temp){
		// ...PHFemMeshThermo::SetLocalFluidTemp()で周囲流体温度の設定、CreateVecfLocal()の更新
		UsingHeatTransferBoundaryCondition(id,temp);
	};
	///	周囲流体温度を更新		熱伝達率を変更できるIf
	void SetVertexTc(int id,double temp,double heatTransRatio){
		//	..熱伝達境界条件を設定：熱伝達率を含む行列が対象
		UsingHeatTransferBoundaryCondition(id,temp,heatTransRatio);	
	};
	// 節点の熱流束を設定する
	void SetVertexHeatFlux(int id,double heatFlux);
	// 全節点の熱流束を設定する
	void SetVtxHeatFluxAll(double heatFlux);

	Vec3d GetPose(unsigned id){ return vertices[id].pos; };
	Vec3d GetSufVtxPose(unsigned id){return vertices[surfaceVertices[id]].pos; };
	// メッシュ節点の温度を取得
	double GetVertexTemp(unsigned id){ return vertices[id].temp; };
	// メッシュ表面の節点温度を取得
	double GetSufVertexTemp(unsigned id){ return vertices[surfaceVertices[id]].temp; };

	//%%%	Stepカウントのための変数、関数定義		%%%//
	unsigned long StepCount;			//	Step数カウントアップの加算係数	
	unsigned long StepCount_;			//	Step数カウントアップの加算係数	StepCountが何週目かを表すカウント 
	//	StepCount数カウントアップ
	unsigned long GetStepCount(){return StepCount;};
	//	StepCountの周回カウントアップ
	unsigned long GetStepCountCyc(){return StepCount_;};


	//	ガウスザイデル計算で用いる関数群

	///	メッシュ表面節点の原点からの距離を計算して、struct FemVertex の disFromOrigin に格納
	void CalcVtxDisFromOrigin();
	void CalcVtxDisFromVertex(Vec2d originVertexIH);
	Vec2d originVertexIH;

	//	IHによ四面体のface面の熱流束加熱のための行列成分計算関数
	void CalcIHdqdt(double r,double R,double dqdtAll,unsigned num);				//	IHヒーターの設定
	void CalcIHdqdt_atleast(double r,double R,double dqdtAll,unsigned num);		//	face面での熱流束量を計算：少しでも円環領域にかかっていたら、そのfaceの面積全部にIH加熱をさせる
	void CalcIHdqdtband(double xS,double xE,double dqdtAll,unsigned num);		//	帯状に加熱、x軸で切る
	void CalcIHdqdtband_(double xS,double xE,double dqdtAll,unsigned num);		//	帯状に加熱、x軸で切る mayIHheatedを使わない
	void CalcIHdqdt2(double r,double R,double dqdtAll,unsigned num);				//	IHヒーターの設定  numは火力別(0:week, 1:middle, 2:high )
	void CalcIHdqdt3(double r,double R,double dqdtAll,unsigned num);				//	IHヒーターの設定	1頂点でも領域内に入っているときには、それをIH計算の領域に加算する
	void CalcIHdqdt4(double radius,double Radius,double dqdtAll,unsigned num);	//	IHヒーターの設定	2よりも、対応できる三角形の場合が幅広い。しかし、三角形の大きさの割に、加熱円半径が小さい場合は、考慮しない。
	//	face頂点のIH加熱時の行列成分を計算	半径10cm程度の円弧と、円環幅数cm幅をまたぐ程度の三角形サイズを想定
	void CalcIHarea(double radius,double Radius,double dqdtAll);
	//	DSTR に交点計算結果を表示する
	void ShowIntersectionVtxDSTR(unsigned faceID,unsigned faceVtxNum,double radius);
	//	△分割前のvectorから三角形分割面積和を求積
	double CalcDivTriArea(std::vector<double> trivec);

	// face頂点を物体原点から近い順に並べかえ、faceクラス変数:ascendVtx[0~2]に近い順の頂点IDを格納;ascendVtx[0]が原点最寄り	
	void ArrangeFacevtxdisAscendingOrder(int faceID);

	// 1点と交わることを想定:id0.id1を結んだ線分と、半径rまたはRの円が交わる交点を求める。(ｓ，ｙ)座標を返す
	Vec2d CalcIntersectionPoint(unsigned id0,unsigned id1,double r,double R);
	// 半径r,Rの円弧と2点で作る線分の交点を求める
	//	...r、Rの2円弧と交わる2交点
	std::vector<Vec2d> CalcIntersectionPoint2(unsigned id0,unsigned id1,double r,double R);
	// 半径rの円弧と線分の交点座標を１つ計算
	Vec2d CalcIntersectionOfCircleAndLine(
		unsigned id0,	//	線分の両端点の頂点番号（0 ~ vertices.size()）
		unsigned id1,
		double radius	//	半径
		);
	//Vec4d CalcIntersectionPoint4(unsigned id0,unsigned id1,double r,double R);		/// 4点と交わることを想定:			同上
	///	メッシュ固有のパラメータの設定
	void SetRhoSpheat(double rho,double Cp);	// 素材固有の密度、比熱の設定

	/// dtを定数倍する
	unsigned Ndt;
	double dNdt;


	//エッジの描画
	void DrawEdge(unsigned id0, unsigned id1);

	//	x軸と垂直なIH加熱の帯領域に線を引くために必要
	void SetIHbandDrawVtx(double xS, double xE);
	Vec2d GetIHbandDrawVtx();

	//時間表示
	double		cpstime;		//[sec] 　1　/　PHFemMeshThermo呼び出し回数[1/sec]

public:
	void DecrMoist();
	void DecrMoist_vel(double dt);
	void DecrMoist_velo(double vel);
	void InitMoist();
	void InitAllVertexTemp();

	void SetParamAndReCreateMatrix(double thConduct0,double roh0,double specificHeat0);	//熱伝達率以外（熱伝達率は現行main.cppで行っているため
	double GetArbitraryPointTemp(Vec3d temppos);							//	四面体内任意点の温度を取得	temppose:知りたい点のローカル座標
	Vec3d GetDistVecDotTri(Vec3d Dotpos,Vec3d trivtx[3]);					//	点から三角形面(2辺のベクトルで定義)へのベクトルを計算
	double GetVtxTempInTets(Vec3d temppos);											//	任意点の四面体内外判定：tempposがあるfaceIDを返す。見つから無ければ、-1を返す。
	double CalcTempInnerTets(unsigned id,PTM::TVector<4,double> N);			//	与えられた形状関数での四面体内の温度を返す

	std::ofstream matCAllout;
	std::ofstream matKAllout;

};




}	//	namespace Spr
#endif