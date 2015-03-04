/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_FEM_WOMOVE_H
#define PH_FEM_WOMOVE_H

#include "PHFemBase.h"
#include "../Foundation/Object.h"
#include "PhysicsDecl.hpp"

#define constRateDrying 0
#define fallRateDrying1st 1
#define fallRateDrying2nd 2

namespace Spr{;

class PHFemPorousWOMove: public PHFemPorousWOMoveDesc, public PHFemBase {
	SPR_OBJECTDEF(PHFemPorousWOMove);
	SPR_DECLMEMBEROF_PHFemPorousWOMoveDesc;

public:
	double tdt;
	
	struct StateVertex{
		double T;		//温度
		double preT;	//1step前の温度
		double Tc;		//節点周りの流体温度
		double rhoWInit;//水の空間密度の初期値
		double rhoOInit;//含油率の初期値
		double rhoW;	//水の空間密度
		double rhoO;	//油の空間密度
		double Pc;		//毛管ポテンシャル
		double muW;		//水の粘度
		double muO;		//油の粘度
		double vVolume;	//頂点が支配する体積
		double mw;		//頂点が持つ水の質量
		double mo;		//頂点が持つ油の質量
		double porosity;//間隙率
		double saturation;//飽和率
		double rhoS;		//固形分の密度
		double rhoS0;		//固形分の密度の初期値
		double preRhoS;		//1step前の固形分の密度
		double outflowWater;	//流出している水分量[kg]
		double outflowOil;		//流出している油分量[kg]
		Vec3d normal;
		bool denaturated;	//変性したかどうか
		double boundWaterMass;
	};

	struct StateEdge{
		double ww;
		double ow;
		double wo;
		double oo;
		double c;
		double b;
	};

	struct StateFace{
		int dryingStep;			//乾燥の段階
		double area;			//四面体の各面の面積
		double surroundFlux;	//周りの流体の速度
		double surroundHu;		//周りの湿度
		double vaporPress;		//周りの水蒸気圧
		double vaporCont;		//周りの水蒸気量(g/m^3)
		double maxVaporPress;	//表面温度での飽和水蒸気圧
		double maxVaporCont;	//表面温度での飽和水蒸気量(g/m^3)
		double K1;				//減率第1段乾燥期での比例定数
		bool evapoRateUpdated;	//蒸発速度定数が変化したときにtrue
		bool deformed;			//	属する頂点の移動により、変形されたとき
		Vec3d normal;
		Vec3d normal_origin;	//法線の始点
	};

	struct StateTet{
		double volume;
		double tetPorosity;	//四面体の間隙率：4頂点の間隙率の相加平均
		double preTetPorosity;
		double tetMuW;		//四面体中の水の粘度：4頂点の水の粘度の相加平均
		double tetMuO;		//四面体中の油の粘度：4頂点の油の粘度の相加平均
		double preTetMuW;	//1step前の水の粘度
		double preTetMuO;	//1step前の油の粘度
		double rhoS;		//固形分の密度
		double rhoS0;		//固形分の密度の初期値
		double preRhoS;		//1step前の固形分の密度
		double wFlux[4];
		double oFlux[4];
		bool sDensChanged;	//固形分の密度変化フラグ

		PTM::TMatrixRow<4,4,double> matWw;
		PTM::TMatrixRow<4,4,double> matOw;
		PTM::TMatrixRow<4,4,double> matWo;
		PTM::TMatrixRow<4,4,double> matOo;
		PTM::TMatrixRow<4,4,double> matC;
		PTM::TMatrixRow<4,4,double> matPcw;
		PTM::TMatrixRow<4,4,double> matPco;
		PTM::TVector<4,double> vecFw[2];
		PTM::TVector<4,double> vecFo[2];
		PTM::TVector<4,double> vecPc;
	};

	std::vector<StateVertex> vertexVars;
	std::vector<StateEdge> edgeVars;
	std::vector<StateFace> faceVars;
	std::vector<StateTet> tetVars;

protected:

	//節点含水率ベクトル
	PTM::TVector<4,double> rhoWVecEle;
	PTM::VMatrixRow<double> rhoWVecAll;
	//節点含油率ベクトル
	PTM::TVector<4,double> rhoOVecEle;
	PTM::VMatrixRow<double> rhoOVecAll;
	//節点毛管ポテンシャルベクトル
	PTM::TVector<4,double> PcEle;
	PTM::VMatrixRow<double> PcVecAll;

	//全体の係数行列
	PTM::VMatrixRow<double> matWwAll;
	PTM::VMatrixRow<double> matOwAll;
	PTM::VMatrixRow<double> matWoAll;
	PTM::VMatrixRow<double> matOoAll;
	PTM::VMatrixRow<double> matCAll;
	PTM::VMatrixRow<double> matPcwAll;
	PTM::VMatrixRow<double> matPcoAll;

	PTM::VMatrixRow<double> vecFwAll;
	PTM::VMatrixRow<double> vecFoAll;

	PTM::VMatrixRow<double> vecFwFinal;	//vecFw + vecPcw * matPc
	PTM::VMatrixRow<double> vecFoFinal;	//vecFo + vecPco * matPc
	
	//全体剛性行列の代わり
	PTM::VMatrixRow<double> dMatWwAll;
	PTM::VMatrixRow<double> dMatOwAll;
	PTM::VMatrixRow<double> dMatWoAll;
	PTM::VMatrixRow<double> dMatOoAll;
	PTM::VMatrixRow<double> dMatCAll;
	PTM::VMatrixRow<double> _dMatWAll;
	PTM::VMatrixRow<double> _dMatOAll;
	PTM::VMatrixCol<double> bwVecAll;
	PTM::VMatrixCol<double> boVecAll;

	//Kmの3つの4×4行列の入れ物　Matk1を作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> matk1array[4];
	//k21,k22,k23,k24の4×4行列の入れ物　Matkを作るまでの間の一時的なデータ置場
	PTM::TMatrixRow<4,4,double> matk2array[4];
	//matc作成用
	PTM::TMatrixRow<4,4,double> matCTemp;
	//Fの4×1ベクトルの入れ物
	PTM::TVector<4,double> vecFarray[4];
	//単位行列 vertex数×vertex数
	PTM::VMatrixRow<double> idMat;
	//dt用の行列(1×1)
	PTM::TMatrixRow<1,1,double> dtMat;

	Vec3d gravity;	//重力加速度ベクトル

	//%%%%%%%%		バイナリスイッチの宣言		%%%%%%%%//
	bool doCalc;
	bool matWOPcVecF2Changed;
	bool matCChanged;
	bool vecFChanged;
	bool matVecChanged;
	bool keisuChanged;

public:

	PHFemPorousWOMove(const PHFemPorousWOMoveDesc& desc=PHFemPorousWOMoveDesc(), SceneIf* s=NULL);
	virtual void Init();
	virtual void Step();
	void Step(double dt);

	double eps;
	//関数の宣言

	void InitMatWO();
	void InitMatC();
	void InitMatPc();
	void InitVecF();

	//[Ww],[Ow],[Wo],[Oo]を作る
	void CreateMatWOPcVecF2Local(unsigned tetid);
	////[K1W2]を作る
	//void CreateMatk2(unsigned id);
	//void CreateMatWOLocal(unsigned i);			//	edgesに入れつつ、チェック用の全体剛性行列も、ifdefスイッチで作れる仕様
	//void CreateMatWOPcVecF2All();

	//[C]を作る
	//void CreateMatCAll();			//Cの全体剛性行列を作る関数
	void CreateMatCLocal(unsigned tetid);	//Cの要素剛性行列を作る関数

	//{FWw},{Fo1}を作る
	void CreateVecF1Local(unsigned tetid);
	////{F2W}を作る
	//void CreateVecF2(unsigned tetid);
	//void CreateVecFLocal(unsigned id);
	//void CreateVecFAll();				//	四面体メッシュのIDを引数に

	void CreateMatVecAll();

	void CreateRhoWVecAll();
	void CreateRhoOVecAll();
	void InitPcVecAll();

	void InitTcAll(double temp);							//	Tcの温度を初期化

	void SetTimeStep(double dt){ tdt = dt; }
	double GetTimeStep(){ return tdt; }


	PTM::TMatrixRow<4,4,double> Create44Mat21();	//共通で用いる、4×4の2と1でできた行列を返す関数
	PTM::TMatrixCol<4,1,double> Create41Vec1();


	PTM::VMatrixRow<double> keisuW;			//	直接法で計算時のWw(t+dt)係数行列
	PTM::VMatrixRow<double> keisuWInv;
	PTM::VMatrixRow<double> keisuO;			//	直接法で計算時のWo(t+dt)係数行列
	PTM::VMatrixRow<double> keisuOInv;
	PTM::VVector<double> uhenW;
	PTM::VVector<double> uhenO;

	PTM::VMatrixRow<double> leftKeisuW;		//含水率の漸化式左辺のWw(t+dt)の係数
	PTM::VMatrixRow<double> rightKeisuWWw;	//含水率の漸化式右辺のWw(t)の係数
	PTM::VMatrixRow<double> rightKeisuWWo;	//含水率の漸化式右辺のWo(t)の係数
	PTM::VMatrixRow<double> rightKeisuWFw;	//含水率の漸化式右辺のFwの係数
	PTM::VMatrixRow<double> rightKeisuWFo;	//含水率の漸化式右辺のFoの係数
	PTM::VMatrixRow<double> leftKeisuO;		//含油率の漸化式左辺のWo(t+dt)の係数
	PTM::VMatrixRow<double> rightKeisuOWw;	//含油率の漸化式右辺のWw(t)の係数
	PTM::VMatrixRow<double> rightKeisuOWo;	//含油率の漸化式右辺のWo(t)の係数
	PTM::VMatrixRow<double> rightKeisuOFw;	//含油率の漸化式右辺のFwの係数
	PTM::VMatrixRow<double> rightKeisuOFo;	//含油率の漸化式右辺のFoの係数


	std::ofstream matWwAllout;
	std::ofstream matOwAllout;
	std::ofstream matWoAllout;
	std::ofstream matOoAllout;
	std::ofstream matCAllout;
	std::ofstream matPcwAllout;
	std::ofstream matPcoAllout;
	std::ofstream vecFwAllout;
	std::ofstream vecFoAllout;
	std::ofstream checkRhoWVecAllout;
	std::ofstream checkRhoOVecAllout;
	std::ofstream FEMLOG;
	std::ofstream keisuWLog;
	std::ofstream keisuOLog;
	std::ofstream invCheck;
	std::ofstream wAll;
	std::ofstream oAll;
	std::ofstream SAll;
	std::ofstream topS;
	std::ofstream sideS;
	std::ofstream bottomS;
	std::ofstream internalS;
	std::ofstream topOutflowWater;
	std::ofstream topOutflowOil;
	std::ofstream sideOutflowWater;
	std::ofstream sideOutflowOil;
	std::ofstream bottomOutflowWater;
	std::ofstream bottomOutflowOil;
	std::ofstream PcAll;
	std::ofstream tempAll;
	std::ofstream paramout;
	std::ofstream tempAndWater;
	unsigned long long COUNT;
	
	unsigned long StepCount;			//	Step数カウントアップの加算係数	
	unsigned long StepCount_;			//	Step数カウントアップの加算係数	StepCountが何週目かを表すカウント 

	/// dtを定数倍する
	unsigned Ndt;
	double dNdt;

	double initSolidMass;

protected:

	double CalcTriangleArea(int id0, int id1, int id2);
	double CalcTetrahedraVolume(FemTet tet); 		// 四面体のIDを入れると、その体積を計算してくれる関数
	void CalcWOContentDirect(double dt, double eps);			// dt:ステップ時間
	void CalcWOContentDirect2(double dt, double eps);
	void CalcWOContentUsingGaussSeidel(unsigned NofCyc, double dt, double eps);
	void CalcWOContentUsingScilab(double dt);
	void SetRhoWAllToRhoWVecAll();			//WwVecAllに全接点の含水率を設定
	void SetRhoOAllToRhoOVecAll();			//WoVecAllに全接点の含油率を設定
	void InitAllVertexRhoW();
	void InitAllVertexRhoO();

public:

	void UpdateVertexRhoWAll();
	void UpdateVertexRhoOAll();

	//頂点がもつ変数へアクセス
	void SetVertexTemp(unsigned vtxid, double temp);
	void SetVertexTc(unsigned vtxid, double tc);
	void SetVertexRhoW(unsigned vtxid, double rhow);
	void SetVertexRhoWInit(unsigned vtxid, double rhowInit);
	void SetVertexRhoO(unsigned vtxid, double rhoo);
	void SetVertexRhoOInit(unsigned vtxid, double rhooInit);
	void SetVertexMw(unsigned vtxid, double mw);
	void SetVertexMo(unsigned vtxid, double mo);
	void CalcVertexWOMu(unsigned vtxid);
	void CalcVertexVolume(unsigned vtxid);
	void CalcVertexMwo(unsigned vtxid);
	void CalcVertexSaturation(unsigned vtxid);
	void CalcVertexPc(unsigned vtxid);
	void CalcRhoWVecFromVertexMw(unsigned vtxid);
	double GetVertexTemp(unsigned vtxid);
	double GetVertexTc(unsigned vtxid);
	double GetVertexRhoW(unsigned vtxid);
	double GetVertexRhoWInit(unsigned vtxid);
	double GetVertexRhoO(unsigned vtxid);
	double GetVertexRhoOInit(unsigned vtxid);
	double GetVertexMw(unsigned vtxid);
	double GetVertexMo(unsigned vtxid);
	
	//面が持つ変数へアクセス
	void CalcFaceArea(unsigned faceid);
	void SetFaceEvaporationRate(unsigned faceid, double evapoRate);
	void SetFaceSurroundFlux(unsigned faceid, double surroundFlux);
	void SetFaceVaporPress(unsigned faceid, double vaporPress);
	void CalcFaceMaxVaporPress(unsigned faceid);
	void CalcFaceMaxVaporCont(unsigned faceid);
	double GetFaceArea(unsigned faceid);
	double GetFaceEvaporationRate(unsigned faceid);
	double GetFaceSurroundFlux(unsigned faceid);
	double GetFaceVaporPress(unsigned faceid);
	double GetFaceMaxVaporPress(unsigned faceid);

	//四面体が持つ変数へアクセス
	void CalcTetVolume(unsigned tetid);
	void CalcTetPorosity(unsigned tetid);
	void CalcTetWOMu(unsigned tetid);
	void CalcTetRhoS(unsigned tetid);
	void SetTetSolidDensity(unsigned tetid, double rhos);
	void SetTetContractionRate(unsigned tetid, double contRate);
	double GetTetVolume(unsigned tetid);
	double GetTetSolidDensity(unsigned tetid);

	PTM::VMatrixRow<double> inv(PTM::VMatrixRow<double> mat);
	PTM::VMatrixRow<double> inv2(PTM::VMatrixRow<double> mat);

	double GetVtxWaterInTets(Vec3d temppos);
	double GetVtxOilInTets(Vec3d temppos);
	double CalcWaterInnerTets(unsigned id,PTM::TVector<4,double> N);
	double CalcOilInnerTets(unsigned id,PTM::TVector<4,double> N);

	float calcGvtx(std::string fwfood, int pv, unsigned texture_mode);
	
	void matWwOut();
	void matWoOut();
	void matOwOut();
	void matOoOut();
	void matPcwOut();
	void matPcoOut();
	void matCOut();
	void vecFwAllOut();
	void vecFoAllOut();
	void vecFwFinalOut();
	void vecFoFinalOut();
	void vecPcAllOut();

	std::vector<unsigned> topVertices;
	std::vector<unsigned> sideVertices;
	std::vector<unsigned> bottomVertices;
	std::vector<unsigned> internalVertices;

	void setGravity(Vec3d g){gravity = g;}
	void vertexDenatProcess(unsigned vtxid);
	void tetDenatProcess(unsigned vtxid);
	void outflowOverSaturation(unsigned vtxid);

	double decideWetValue(unsigned vtxid);
	double GetVtxSaturation(unsigned vtxid){return vertexVars[vtxid].saturation;}

	void decrhoW(double dec);
};
}	//	namespace Spr

#endif