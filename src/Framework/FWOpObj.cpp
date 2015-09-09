
#include "Framework.h"
#include "FWOpObj.h"
namespace Spr{;
	

FWOpObj::FWOpObj(const FWOpObjDesc& d) :grMesh(NULL){
	SetDesc(&d);
	fwPSize = d.fwPSize;
}
ObjectIf* FWOpObj::GetOpObj()
{
	return opObj->Cast();
}
ObjectIf* FWOpObj::GetGRMesh()
{
	return grMesh->Cast();
}
void FWOpObj::Sync()
{
	Blend();
}
void FWOpObj::CreateOpObj()
{
	grMesh->EnableAlwaysCreateBuffer();

	//Decide Particle size
	//Vec3f beginP, diameterP;
	Vec3f tbeginP, tdiameterP;
	int vtsNum = grMesh->NVertex();
	//Vec3f beginP = grMesh->vertices[72],diameterP =grMesh->vertices[73];//only for monkey
	//Vec3f beginP = grMesh->vertices[392],diameterP =grMesh->vertices[493];//only for monkey
	//Vec3f beginP = grMesh->vertices[107],diameterP =grMesh->vertices[112];//only for monkey
	//Vec3f beginP = grMesh->vertices[405],diameterP =grMesh->vertices[495];//only for monkey
	

	//モデルからparticle粒子探索半径やいろいろを決める
	if (vtsNum == 1)
	{

	}
	else if (grMesh->GetName()[0] == 'h')//for hptst
	{
		if (grMesh->GetName()[9] == '4')
		{
			//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
			//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
			tbeginP = grMesh->vertices[0];	tdiameterP = grMesh->vertices[1];
			//params.SetBound(30.0f);
		
		}
	}
	else if (grMesh->GetName()[0] == 'a')//for artery
	{
		//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
		//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
		tbeginP = grMesh->vertices[61];	tdiameterP = grMesh->vertices[90];
		opObj->params.SetBound(30.0f);
	
	}
	else if (grMesh->GetName()[0] == 'c')//for clipMesh
	{
		//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
		//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
		tbeginP = grMesh->vertices[61];	tdiameterP = grMesh->vertices[90];
		opObj->params.SetBound(30.0f);
	
	}
	else if (grMesh->GetName()[0] == 'f')//for fabicrabbit
	{
		//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
		//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
		if (grMesh->GetName()[2] == 'c')
		{//for faceCube
			tbeginP = grMesh->vertices[208];	tdiameterP = grMesh->vertices[310];
		
		}
		else{

			tbeginP = grMesh->vertices[61];	tdiameterP = grMesh->vertices[65];
		
		}
	}
	else if (grMesh->GetName()[0] == 's')//for fabicrabbit
	{
		//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
		//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
		tbeginP = grMesh->vertices[61];	tdiameterP = grMesh->vertices[65];
	
	}
	else if (grMesh->GetName()[0] == 'r')//for rabbit
	{
		//tbeginP = grMesh->vertices[165];	tdiameterP =grMesh->vertices[404];
		//best stiffness for no tegen//tbeginP = grMesh->vertices[61];	tdiameterP =grMesh->vertices[63];
		tbeginP = grMesh->vertices[61];	tdiameterP = grMesh->vertices[65];
	
	}
	else if (grMesh->GetName()[0] == 'm')//only for monkey
	{
		//tbeginP = grMesh->vertices[102];	tdiameterP =grMesh->vertices[130];//ideal......
		tbeginP = grMesh->vertices[14];	tdiameterP = grMesh->vertices[20];
		//tbeginP = grMesh->vertices[193];
		//tdiameterP =grMesh->vertices[198];
	
	}
	else if (grMesh->GetName()[0] == 'b')//only for bar
	{
		tbeginP = grMesh->vertices[0];	tdiameterP = grMesh->vertices[4];
		opObj->objGrouplinkCount = 3;
	
	}
	else if (grMesh->GetName()[0] == 'p')
	{
		tbeginP = grMesh->vertices[0];	tdiameterP = grMesh->vertices[5];
	
	}
	else if (grMesh->GetName()[0] == 'd')
	{
		if (grMesh->GetName()[1] == 'u')
		{
			tbeginP = grMesh->vertices[12]; tdiameterP = grMesh->vertices[42];
		}
		else{
			tbeginP = grMesh->vertices[4161];	tdiameterP = grMesh->vertices[4160];

		}
	
	}
	else if (grMesh->GetName()[0] == 'g')
	{
		tbeginP = grMesh->vertices[0];	tdiameterP = grMesh->vertices[1];

	}
	else if (grMesh->GetName()[0] == 'e')
	{
		if (grMesh->GetName()[8] == 'T')
		{
			tbeginP = grMesh->vertices[9];	tdiameterP = grMesh->vertices[57];
		
		}
		else {
			//tbeginP = grMesh->vertices[44];	tdiameterP =grMesh->vertices[37];
			tbeginP = grMesh->vertices[44];	tdiameterP = grMesh->vertices[40];
		
		}
	}
	else{ tbeginP = grMesh->vertices[0];	tdiameterP = grMesh->vertices[2]; }//only for box

	
	tdiameterP = tbeginP - tdiameterP;
	float objPtclDiameter = fabs(tdiameterP.norm());
	//objPtclRadiusSpecialForUnstabledeform = objPtclDiameter / 2;//no kill

	opObj->initialPHOpObj(&grMesh->vertices[0], grMesh->NVertex(), objPtclDiameter);
	//opObj->SetGRMesh(grMesh);
	opObj->targetMesh = grMesh;
	opObj->SetName(grMesh->GetName());
}
void FWOpObj::CreateOpObjWithRadius(float r)
{
	grMesh->EnableAlwaysCreateBuffer();
	opObj->initialPHOpObj(&grMesh->vertices[0], grMesh->NVertex(), r);
	//opObj->SetGRMesh(grMesh);
	opObj->targetMesh = grMesh;
	opObj->SetName(grMesh->GetName());
}

void FWOpObj::Blend()
{

	int count = 0;
	
	count = grMesh->NVertex();

	for (int j = 0; j<opObj->assPsNum; j++)
	{

		PHOpGroup &pg = opObj->objGArr[opObj->objPArr[j].pMyGroupInd];
		//頂点は粒子グループのメンバーに対して重みをかけて位置を計算する
		for (int k = 0; k<opObj->objPArr[j].pNvertex; k++)
		{
			int vertind = opObj->objPArr[j].pVertArr[k];
			Vec3f &vert = opObj->objOrigPos[vertind];
			Vec3f u; u = u.Zero();
			
			int startFromLinkP = 0;
			for (int jm = startFromLinkP; jm<pg.gNptcl; jm++)
			{

				PHOpParticle &dp = opObj->objPArr[pg.getParticleGlbIndex(jm)];
				Spr::TPose<float> &ctrpose = opObj->objOrinPsPoseList[pg.getParticleGlbIndex(jm)];
				Spr::TPose<float> pose = TPose<float>(dp.pCurrCtr, dp.pCurrOrint);
			
				pose.x = -pose.x;
				pose.y = -pose.y;
				pose.z = -pose.z;
				
				u += opObj->objBlWeightArr[vertind][jm] * (pose * (ctrpose.Pos() + vert));// (taM *

			}

			grMesh->vertices[vertind] = u;
			
		}
	}

	//	 calc normal
	// 頂点を共有する面の数
	if (opObj->updateNormals)
	{
		std::vector<int> nFace(grMesh->vertices.size(), 0);

		for (unsigned i = 0; i < grMesh->triFaces.size(); i += 3){
			Vec3f n = (grMesh->vertices[grMesh->triFaces[i + 1]] - grMesh->vertices[grMesh->triFaces[i]])
				% (grMesh->vertices[grMesh->triFaces[i + 2]] - grMesh->vertices[grMesh->triFaces[i]]);
			n.unitize();

			grMesh->normals[grMesh->triFaces[i]] += n;
			grMesh->normals[grMesh->triFaces[i + 1]] += n;
			grMesh->normals[grMesh->triFaces[i + 2]] += n;
			nFace[grMesh->triFaces[i]] ++;
			nFace[grMesh->triFaces[i + 1]] ++;
			nFace[grMesh->triFaces[i + 2]] ++;
		}

		for (unsigned i = 0; i < grMesh->normals.size(); ++i)
			grMesh->normals[i] /= nFace[i];
	}
}

size_t FWOpObj::NChildObject() const{  //debug

	return FWObject::NChildObject() + (grMesh ? 1 : 0) + (opObj ? 1 : 0);
}
ObjectIf* FWOpObj::GetChildObject(size_t pos){
	if (pos < FWObject::NChildObject()){
		return FWObject::GetChildObject(pos);
	}
	else{
		pos -= FWObject::NChildObject();
	}
	if (grMesh){
		if (pos == 0) return grMesh->Cast();
		else pos--;
	}
	if (opObj){
		if (pos == 0) return opObj->Cast();
		else pos--;
	}
	
	return NULL;
}
bool FWOpObj::AddChildObject(ObjectIf* o){
	
	PHOpObj* obj = DCAST(PHOpObj, o);
	if (obj){
		opObj = obj;
		return true;
	}
	GRMesh* gro = DCAST(GRMesh, o);
	if (gro){
		grMesh = gro;
		return true;
	}
	
	return FWObject::AddChildObject(o);
	
}

}
