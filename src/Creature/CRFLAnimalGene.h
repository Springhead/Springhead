/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/////////////////////////////////////////////////////////
#ifndef FLANIMALGENE_H
#define FLANIMALGENE_H
/////////////////////////////////////////////////////////
#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>
#include <vector>
#include <ctime>
#include <cmath>

#include "CRController.h"
#include "CRFLAnimalGeneData.h"

// --- memo --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
// とりあえず、ひとつのボディに対して学習できるようにしか対応していません。
// 複数個体の学習が見たい場合には要素は取ってきてあるので自分で作って追加してください。
// flAnimalGenesは遺伝子の内部配列と外から見た時の配列ができているので、
// 後はボディごとにこれを持たせればいいと思います。
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

// std::vector<CRFLAnimalGeneData>: "ある遺伝子ひとつ"型

/////////////////////////////////////////////////////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 遺伝子関係のことを取り仕切るクラス
*/
class CRFLAnimalGene: public UTRefCount{

private:
	CRCreatureIf*					creature;						//< 制御対象のクリーチャー群
	std::vector<CRBodyIf*>		    crBody;							//< creatureに登録されているボディ情報(X体)を格納する配列
	
public:
	std::vector< std::vector<CRFLAnimalGeneData> > flAnimalGenes;	//< 遺伝子（std::vector<CRFLAnimalGeneData）の配列（std::vector<*******>）

	// コンストラクタ
	CRFLAnimalGene(){
		//DSTR << "This Constractor is called when a object which is CRFLAnimalQL class has created." << std::endl;
	}
	CRFLAnimalGene(CRCreatureIf* charactors){
		creature = charactors;
		Init();
	}
	//デストラクタ
	~CRFLAnimalGene(){
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// コンストラクタみたいなもの
	/** @brief クラス生成時に初期化を行う関数
	*/
	virtual void Init();

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//遺伝子操作をする関数

	/** @brief 一つのボディについて今現在の姿勢制御方向から遺伝子を作って返す
	*/
	std::vector<CRFLAnimalGeneData> CreateGene(CRBodyIf* body);

	/** @brief あるボディの現在取っている姿勢を表す遺伝子を返す
	*/
	std::vector<CRFLAnimalGeneData> GetGene(CRBodyIf* body);
	
	/** @brief あるボディの一番新しい遺伝子の姿勢を指定したものに書き換える
	*/
	void SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body);
	
	/** @brief ある遺伝子Aと別のある遺伝子Bをある所から入れ替えて（一点交叉）新しい遺伝子を作る
	*/
	std::vector<CRFLAnimalGeneData> MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB);

};

#endif