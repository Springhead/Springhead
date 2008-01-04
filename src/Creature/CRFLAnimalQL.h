/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
///////////////////////////////////////////////////////
#ifndef CRFLANIMALQL_H
#define CRFLANIMALQL_H
///////////////////////////////////////////////////////

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>
#include <vector>
#include <ctime>
#include <cmath>

#include "CRController.h"
#include "CRFLAnimalGene.h"

////////////////////////////////////////////////////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief Q学習を行うクラス
*/
class CRFLAnimalQL: public CRFLAnimalGene{

private:
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//大域変数の定義
	// CRFLAnimalGeneから継承される変数（protected⇒private化）
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																										//
	// CRCreatureIf*			creature;		//< 学習を行うクリーチャー群								//
	// std::vector<CRBodyIf*>	crBody;			//< creatureに登録されているボディ情報（X体）を格納する配列	//
	//																										//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	std::vector<int>					action;			//< 各関節の指令が入る配列
	std::vector<int>					actionNumber;	//< 各関節の取り得るアクション数が入る配列
	std::vector<CRFLAnimalGeneData>		thisTermGene;	//< あるStep()での遺伝子
	
	double					learningRate;	//<	学習率
	double					decreaseRate;	//< 割引率（エージェントの忘却率）
	double					reward;			//< 報酬
	double					qValue;			//< Q値
	double					qValueDash;		//< 規格化されたQ値
	double					penalty;		//< 規格化の際に使用する罰の値

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//隠蔽される関数
	void		BoltzmannSelection();									//< ボルツマン選択
	void		EpsilonGreedySelection();								//< ε-greedy選択

	void		SetActionNumber(std::vector<CRFLAnimalGeneData> aGene); //< ある遺伝子の各関節が取り得る行動のパターン総数を設定する
	void		SelectAction(std::vector<CRFLAnimalGeneData> aGene);	//< 行動を決定する
	void		TakeAction(std::vector<CRFLAnimalGeneData> aGene);		//< アクションを実行する	
	void		EvaluativeFunc();										//< 評価関数
	void		CalcQValueMax();										//< actionを変動させた時のQ値の最大値を調べる
	void		CalcQValueMin();										//< actionを変動させた時のQ値の最小値を調べる
	void		UpdateQValues();										//< Q値の更新を行う
	void		StateValueMax();										//< 各状態の価値の最大値を求める
	void		StateValueMin();										//< 各状態の価値の最小値を求める
	void		CalcQValueDash();										//< Q値の規格化をすることでQ'を求める
	void		FitnessFromQValue();									//< Q値の高いものの適合度を高くする
	void		FitnessFromTimesOfUse();								//< 使用頻度の高いものの適合度を高くする

public:

	// 大域変数の定義
	//CRFLAnimalGeneから継承される変数(public)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																			   //
	// std::vector< std::vector<CRFLAnimalGeneData> > flAnimalGenes;	//< 遺伝子（std::vector<CRFLAnimalGeneData）の配列（std::vector<*******>） //
	//																																			   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//コンストラクタ
	CRFLAnimalQL(CRCreatureIf* charactors){
		creature = charactors;
		Init();
	}
	//デストラクタ
	~CRFLAnimalQL(){
	}
	
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		
	//インタフェースの実装

	// CRFLAnimalGeneから継承されてくるインタフェース
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																//
	// <<遺伝子操作をする関数>>																										//
	//																																//
	// @brief 一つのボディについて今現在の姿勢制御方向から遺伝子を作って返す														//
	// std::vector<CRFLAnimalGeneData> CreateGene(CRBodyIf* body);																	//
	//																																//
	// @brief あるボディの現在取っている姿勢を表す遺伝子を返す																		//
	// std::vector<CRFLAnimalGeneData> GetGene(CRBodyIf* body);																		//
	//																																//
	// @brief あるボディの一番新しい遺伝子の姿勢を指定したものに書き換える															//
	// void SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body);															//
	//																																//
	// @brief ある遺伝子Aと別のある遺伝子Bをある所から入れ替えて（一点交叉）新しい遺伝子を作る										//
	// std::vector<CRFLAnimalGeneData> MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB);		//
	//																																//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** @brief 初期化を行う
	*/
	void Init();
	
	/** @brief Q学習を進める
	*/
	void Step();	
	
};

#endif