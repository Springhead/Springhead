#include "Physics.h"
#pragma hdrstop
// PHMagneticEngine.cpp: PHMagneticEngine クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "PHMagneticEngine.h"
#include <Base/BaseDebug.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
//#include <Framework/FWGraphics.h>

#include <Base/TMatrix.h>    //  行列ライブラリのインクルードする．
#include <Base/TVector.h>

#define MAG_NUM 5.0f

namespace Spr{;
SGOBJECTIMP(PHMagnet, SGObject);		//派生関係を示すためのマクロ

bool PHMagnet::AddChildObject(SGObject *o, SGScene *s)
{
	if (DCAST(PHSolid, o)){
		solid=(PHSolid*)o;
		return true;
	}
	return false;
}

void PHMagnet::Clear()
{
	solid=NULL;
}


//セイバーとローダー
class PHMagnetLoader:public FIObjectLoader<PHMagnet>{
public:
	PHMagnetLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XMagnet);
	}
	virtual bool LoadData(FILoadScene* ctx, PHMagnet* magnet){
		ctx->docs.Top()->GetWholeData(magnet->magn);
		return true;
	}
};

class PHMagnetSaver:public FIObjectSaver<PHMagnet>{
	public:
	virtual UTString GetType() const{ return "PHMagnet"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHMagnet* magnet){
			doc->SetWholeData(magnet->magn);
			doc->AddChild(ctx->CreateDocNode("REF", magnet->solid));
	}
};
DEF_REGISTER_BOTH(PHMagnet);	//ローダー,セイバーの登録

//派生関係を示すためのマクロ
SGOBJECTIMP(PHMagneticEngine, SGBehaviorEngine);

//////////////////////////////////////////////////////////////////////
//磁力の物理エンジン
//////////////////////////////////////////////////////////////////////
PHMagneticEngine::PHMagneticEngine(){}

//PHMagneticEngineの1ステップの処理
void PHMagneticEngine::Step(SGScene *s)
{
	int size=mags.size();
	//普通の磁石の計算
	for(int i=0;i<size-1;i++){
		for(int j=i+1;j<size;j++){
			MagnetForce(mags[i], mags[j], s);
		}
	}
	/*
	//単磁極磁石の計算
	for(int i=0;i<size-1;i++){
		for(int j=i+1;j<size;j++){
			AddForce(mags[i], mags[j], -1);
		}
	}
	*/
	//地球の磁力を加える、磁石であるオブジェクトの強調
	for(int i=0;i<size;i++){
		//DSTR << size;
		EarthPower(mags[i]);
		LineDraw(mags[i]->solid->GetCenterPosition(),s);
	}

	 //  行列クラスはPTM名前空間の中で宣言されている．
	PTM::TMatrixCol<2,2,float> mat;     //  2行2列の行列を宣言
    mat[0][0] = 1;  mat[0][1] = 2;
    mat[1][0] = 3;  mat[1][1] = 4;
	const Vec2f vec(1,0);       //  2次元のベクトルを宣言
    //vec[0] = 1; vec[1] = 0;
	PTM::TVector<2,float> xx;

	//PTM::gauss(mat,xx,vec,ip);
	DSTR << mat*vec;
    
}
//磁石が持つソリッドのポインタを取得するための関数
bool PHMagneticEngine::AddChildObject(SGObject *o, SGScene *s)
{
	if (DCAST(PHMagnet, o)){
		mags.push_back((PHMagnet*)o);
		return true;
	}else{
		if (DCAST(GRMesh,o)){
			mesh=(GRMesh*)o;
			return true;
		}
	}
	return false;
}
void PHMagneticEngine::EarthPower(PHMagnet *one)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one中心点を取得
	Quaterniond quat1	= one->solid->GetOrientation();		//one向きを取得

	//すべての磁極の座標の計算
	Vec3f onePosiS = onePosi+quat1*one->magn.sPos;
	Vec3f onePosiN = onePosi+quat1*one->magn.nPos;

	one->solid->AddForce(mag.earthMagnet,onePosiS);
	one->solid->AddForce(-mag.earthMagnet,onePosiN);

}
//1つ磁極を持つ磁石の力の計算
void PHMagneticEngine::AddForce(PHMagnet *one, PHMagnet *two,int flag)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one中心点を取得
	Vec3f twoPosi		= two->solid->GetCenterPosition();	//two中心点を取得

	Vec3f distance;
	float dis;
	Vec3f power;

	distance = onePosi-twoPosi;
	dis		 = distance*distance;
	//磁力の計算
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce((float)flag*power);
	two->solid->AddForce(-(float)flag*power);
}
//2つ磁極を持つ磁石の力の計算
void PHMagneticEngine::MagnetForce(PHMagnet *one, PHMagnet *two, SGScene *s)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one中心点を取得
	Vec3f twoPosi		= two->solid->GetCenterPosition();	//two中心点を取得
	Quaterniond quat1	= one->solid->GetOrientation();		//one向きを取得
	Quaterniond quat2	= two->solid->GetOrientation();		//two向きを取得
	
	Vec3f power;
	Vec3f hyouji;
	Vec3f distance;
	float dis;
	Vec3f onePosiS, twoPosiS;
	Vec3f onePosiN, twoPosiN;
	//すべての磁極の座標の計算
	onePosiS = onePosi+quat1*one->magn.sPos;
	twoPosiS = twoPosi+quat2*two->magn.sPos;
	onePosiN = onePosi+quat1*one->magn.nPos;
	twoPosiN = twoPosi+quat2*two->magn.nPos;

//	FWGraphics graph(s);
//	graph.DrawVec(onePosiS,Vec3f(0.1,0.1,0.1),Vec4f(1,0,0,1));
//	graph.DrawVec(onePosiN,Vec3f(0.1,0.1,0.1),Vec4f(0,1,0,1));

//---S極とS極の計算--------------------------
	distance = onePosiS-twoPosiS;
	dis		 = distance*distance;
	//磁力の計算
	dis=(float)sqrt(dis);
	//エラー回避のため距離を加算する
	dis+=0.1f;
	dis=dis*dis*dis;
	//力を計算し、ソリッドに加える
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(power,onePosiS);
	two->solid->AddForce(-power,twoPosiS);

//---N極とS極の計算--------------------------
	distance = onePosiN-twoPosiS;
	dis		 = distance*distance;
	//磁力の計算
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(-power,onePosiN);
	two->solid->AddForce(power,twoPosiS);
	
//---S極とN極の計算--------------------------
	distance = onePosiS-twoPosiN;
	dis		 = distance*distance;
	//磁力の計算
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(-power,onePosiS);
	two->solid->AddForce(power,twoPosiN);

//---N極とN極の計算--------------------------
	distance = onePosiN-twoPosiN;
	dis		 = distance*distance;
	//磁力の計算
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(power,onePosiN);
	two->solid->AddForce(-power,twoPosiN);
}

void PHMagneticEngine::LineDraw(Vec3f center,SGScene *s)
{
//描写の設定/////
	GRRender* render;
	s->GetRenderers().Find(render);
	if (render->CanDraw()){
		Affinef afw;
		SGFrame* baseFrame=s->GetWorld();
		afw = baseFrame->GetWorldPosture();
		afw.Ex().unitize();
		afw.Ey().unitize();
		afw.Ez().unitize();
		render->SetModelMatrix(afw);
	}
	GRMaterialData mat1(Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1), 0);
	mat1.ambient = Vec4f(1, 1, 0, 1);
	mat1.diffuse = Vec4f(1, 1, 0, 1);
	mat1.specular = Vec4f(1, 1, 0, 1);
	mat1.emissive = Vec4f(1, 1, 0, 1);
	mat1.power = 0;
	render->SetDepthFunc(GRRender::DF_ALWAYS);//Zバッファのチェックをしない．
	render->SetLineWidth(8);                  //線の太さ(OpenGLで有効)，点の大きさ(DirectXで有効)
	render->SetMaterial(mat1);                //マテリアル設定
/////////////////
	//メッシュの情報があったときだけ描写を行う
	if(mesh)
	for(int i=0;i<mesh->triangles.size()-1;i++){
		Vec3f vec[2];
		vec[0]=mesh->vertices[mesh->triangles[i]]+center;
		vec[1]=mesh->vertices[mesh->triangles[i+1]]+center;
		render->DrawDirect(GRRender::LINES, vec, vec+2);
	}
	render->SetDepthFunc(GRRender::DF_LESS);  //Zバッファのチェックを戻す
}
///////////////////////////////////////////////
//磁力エンジンのローダーとセイバー
//
//Xファイルからシーングラフに変換、また逆の変換を行うためのクラス
class PHMagneticEngineLoader:public FIObjectLoader<PHMagneticEngine>{
public:
	PHMagneticEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_RECORD_PROTO(XMagneticEngine);
	}
	virtual bool LoadData(FILoadScene* ctx, PHMagneticEngine* magnet){
		ctx->docs.Top()->GetWholeData(magnet->mag);
		return true;
	}
};

class PHMagneticEngineSaver:public FIObjectSaver<PHMagneticEngine>{
public:
	virtual UTString GetType() const{ return "PHMagneticEngine"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHMagneticEngine* magnet){
			doc->SetWholeData(magnet->mag);
			for(PHMagnets::iterator it = magnet->mags.begin(); it != magnet->mags.end(); it++){
				doc->AddChild(ctx->CreateDocNode("REF", *it));
			}
	}
};
DEF_REGISTER_BOTH(PHMagneticEngine);	//ローダー,セイバーの登録マクロ

}	//	namespace Spr





