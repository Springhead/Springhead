#ifndef FLANIMALGENE_H
#define FLANIMALGENE_H

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>
#include <vector>

#include "CRController.h"
#include "CRFLAnimalGeneData.h"

// --- memo --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
// とりあえず、ひとつのボディに対して学習できるようにしか対応していません。
// 複数個体の学習が見たい場合には要素は取ってきてあるので自分で作って追加してください。
// flAnimalGenesは遺伝子の内部配列と外から見た時の配列ができているので、後はボディごとに
// これを持たせればいいと思います。
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
class CRFLAnimalGene{

private:
	CRCreatureIf*					creature;					//< 制御対象のクリーチャー群
	std::vector<CRBodyIf*>		    crBody;						//< creatureに登録されているボディ情報を格納する配列
	std::vector< std::vector<CRFLAnimalGeneData> > flAnimalGenes; //< 遺伝子（std::vector<CRFLAnimalGeneData）の配列（std::vector<*******>）
public:
	
	// コンストラクタ
	
	CRFLAnimalGene(CRCreatureIf* charactors){
		creature = charactors;
		Init();
	}
	~CRFLAnimalGene(){
	}

	std::vector<CRFLAnimalGeneData> CreateGene(CRBodyIf* body);
	std::vector<CRFLAnimalGeneData> GetGene(CRBodyIf* body);
	
	void SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body);
	void Init();
	void MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB);

};

#endif