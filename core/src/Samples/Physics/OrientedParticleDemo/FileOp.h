#ifndef FILEOP_H
#define FILEOP_H

#include   <shlobj.h> 
#include <conio.h>
#include <Physics\PHOpObj.h>
#include <Physics\PHOpGroup.h>
#include <Physics\PHOpParticle.h>

using namespace std;
using namespace Spr;

class FileOp
{
	 float fileVersion;
	


public:
	struct MeshName{
		string name;
	};
	 vector<MeshName> objNames;
		FileOp()
		{
			loaded = false;
			loadSeq = 0; 
			fileVersion = 2.0f;
		}

		void reset()
		{
			loaded = false;
			loadSeq = 0; 
			fileVersion = 2.0f;
		}
		
		void loadDfmScene(char *filename)
		{
			FILE *f;
			
			fopen_s(&f,filename, "r");
			if (!f) {
				std::cout<<" file dir can not open"<<endl;
				_getch();
				fclose(f);
				exit(0);
			}
			const int len = 100;
			char *s = new char[len +1];
			
			fgets(s, len, f);
			fgets(s, len, f);
			float *loadfvf = new float;
			sscanf_s(s, "%f", loadfvf);
			if(*loadfvf!=fileVersion)
			{
				std::cout<<"Failed please use correct version of file"<<endl;
				_getch();
				fclose(f);
				exit(0);
			}
			fgets(s, len, f);
			
			//char **name = new char[strlen(s)-1];
			//for(int i=0;i<strlen(s)-1;i++)
			int i=0;
			while(fgets(s, len, f)&&i<100)
			{
				if(s[0]!='/')
					{
						
						MeshName mn;
						mn.name = s;
						//mn.name = new char[strlen(s)];
						/*for(int j=0;j<strlen(s);j++)
							{
								
									mn.name[j] = s[j]; 
						}*/
						objNames.push_back(mn); 
				}
				i++;
			}
			delete s;
			delete loadfvf;
			int u=0;
			fclose(f);
		}

	bool loaded;
	int loadSeq;
		void saveToFile(PHOpObj &dfobj,char *filename, char *meshname)
	{
		std::cout<<" Save to file...";
		/*if(filename[0] == 'D')
			{
				filename = new char[255];
				SHGetSpecialFolderPathA(0,filename,CSIDL_DESKTOPDIRECTORY,0);
				char plus[100] = "\\opPHOpObj1.dfOp";
				strcat(filename,plus);
		}*/
		/*char plus[100] = "opPHOpObj1.dfOp";
		filename = plus;*/

		FILE *f;
		fopen_s(&f,filename, "w");
		if (!f) {
			std::cout<<" file dir can not open"<<endl;
			return;
		}
		
		if(dfobj.objUnNormalobj)
		{
			std::cout<<" do not save haptic controller"<<endl;
			std::cout<<" Save Failed"<<endl;
			fclose(f);
			return;
		}
		fprintf(f, "FileVersion:\n");
		fprintf(f, "%f\n",fileVersion);
		fprintf(f, "ModelName\n");
		/*int namelen;
		if(strlen(dfobj.targetMesh->GetName())=='/n')
			namelen = strlen(dfobj.targetMesh->GetName()) - 1;
		else namelen = strlen(dfobj.targetMesh->GetName());
			char *name = new char[namelen];
		for(int i = 0;i<namelen;i++)
		{
			name[i] = dfobj.targetMesh->GetName()[i];
		}*/
		fprintf(f, "%s\n", meshname);
		fprintf(f, "GNum\n");
		fprintf(f, "%i\n", dfobj.assGrpNum);
		fprintf(f, "PNum\n");
		fprintf(f, "%i\n", dfobj.assPsNum);
		fprintf(f, "vertices\n");
		
		//if(dfobj.useTetgen)
		//{
			//vertices
			for(int i = 0;i< dfobj.objTargetVtsNum;i++)
			{
				fprintf(f, "%f %f %f\n", dfobj.objTargetVts[i].x, dfobj.objTargetVts[i].y, dfobj.objTargetVts[i].z);
			}
		
		fprintf(f, "group\n");


		//group
		for (int i = 0; i < dfobj.assGrpNum; i++) 
		{
			//fprintf(f, "%f %f %f\n", dfobj.objGArr[i].gMyIndex, mOriginalPos[i].y, mMasses[i]);
			fprintf(f, "gMyIndex\n");
			fprintf(f, "%i\n", dfobj.objGArr[i].gMyIndex);
			fprintf(f, "gCurrCenter\n");
			fprintf(f, "%f %f %f\n", dfobj.objGArr[i].gCurrCenter.x,dfobj.objGArr[i].gCurrCenter.y,dfobj.objGArr[i].gCurrCenter.z);
			fprintf(f, "gNptcl\n");
			fprintf(f, "%i\n", dfobj.objGArr[i].gNptcl);
			fprintf(f, "gOrigCenter\n");
			fprintf(f, "%f %f %f\n", dfobj.objGArr[i].gOrigCenter.x,dfobj.objGArr[i].gOrigCenter.y,dfobj.objGArr[i].gOrigCenter.z);
			fprintf(f, "gtotalMass\n");
			fprintf(f, "%f\n", dfobj.objGArr[i].gtotalMass);
			fprintf(f, "gPInd\n");
			for(int j=0;j<dfobj.objGArr[i].gNptcl;j++)
			{
				fprintf(f, "%i\n", dfobj.objGArr[i].gPInd[j]);
			}



		}
		fprintf(f, "ptcl\n");
		//ptcl
		for (int i = 0; i < dfobj.assPsNum; i++) 
		{
			fprintf(f, "pPId\n");
			fprintf(f, "%i\n", dfobj.objPArr[i].pPId);
			fprintf(f, "pNvertex\n");
			fprintf(f, "%i\n", dfobj.objPArr[i].pNvertex);
			fprintf(f, "pVertArr\n");
			for(int j=0;j<dfobj.objPArr[i].pNvertex;j++)
				fprintf(f, "%i\n", dfobj.objPArr[i].pVertArr[j]);
			
			fprintf(f, "pOrigCtr\n");
			fprintf(f, "%f %f %f\n", dfobj.objPArr[i].pOrigCtr.x,dfobj.objPArr[i].pOrigCtr.y,dfobj.objPArr[i].pOrigCtr.z);
			fprintf(f, "pOrigOrint\n");
			fprintf(f, "%f %f %f %f\n", dfobj.objPArr[i].pOrigOrint.x,dfobj.objPArr[i].pOrigOrint.y,dfobj.objPArr[i].pOrigOrint.z,dfobj.objPArr[i].pOrigOrint.w);
			fprintf(f, "pCurrCtr\n");
			fprintf(f, "%f %f %f\n", dfobj.objPArr[i].pCurrCtr.x,dfobj.objPArr[i].pCurrCtr.y,dfobj.objPArr[i].pCurrCtr.z);
			fprintf(f, "pCurrOrint\n");
			fprintf(f, "%f %f %f %f\n", dfobj.objPArr[i].pCurrOrint.x,dfobj.objPArr[i].pCurrOrint.y,dfobj.objPArr[i].pCurrOrint.z,dfobj.objPArr[i].pCurrOrint.w);

			fprintf(f, "pTotalMass\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pTotalMass);
			fprintf(f, "isFixed\n");
			fprintf(f, "%i\n",  dfobj.objPArr[i].isFixed);

			//fprintf(f, "covMatrixList.size\n");
		//	fprintf(f, "%i\n",  dfobj.objPArr[i].pcovMatrixList.size());
		//	fprintf(f, "covMatrixList\n");
		//	for(int k=0;k<dfobj.objPArr[i].covMatrixList.size();k++)
		//		fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].covMatrixList[k].xx,dfobj.objPArr[i].covMatrixList[k].xy,dfobj.objPArr[i].covMatrixList[k].xz,dfobj.objPArr[i].covMatrixList[k].yx,dfobj.objPArr[i].covMatrixList[k].yy,dfobj.objPArr[i].covMatrixList[k].yz,dfobj.objPArr[i].covMatrixList[k].zx,dfobj.objPArr[i].covMatrixList[k].zy,dfobj.objPArr[i].covMatrixList[k].zz);
			fprintf(f, "pMomentInertia\n");
			fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].pMomentInertia.xx,dfobj.objPArr[i].pMomentInertia.xy,dfobj.objPArr[i].pMomentInertia.xz,dfobj.objPArr[i].pMomentInertia.yx,dfobj.objPArr[i].pMomentInertia.yy,dfobj.objPArr[i].pMomentInertia.yz,dfobj.objPArr[i].pMomentInertia.zx,dfobj.objPArr[i].pMomentInertia.zy,dfobj.objPArr[i].pMomentInertia.zz);
			fprintf(f, "pInverseOfMomentInertia\n");
			fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].pInverseOfMomentInertia.xx,dfobj.objPArr[i].pInverseOfMomentInertia.xy,dfobj.objPArr[i].pInverseOfMomentInertia.xz,dfobj.objPArr[i].pInverseOfMomentInertia.yx,dfobj.objPArr[i].pInverseOfMomentInertia.yy,dfobj.objPArr[i].pInverseOfMomentInertia.yz,dfobj.objPArr[i].pInverseOfMomentInertia.zx,dfobj.objPArr[i].pInverseOfMomentInertia.zy,dfobj.objPArr[i].pInverseOfMomentInertia.zz);

			fprintf(f, "pTempSingleVMass\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pTempSingleVMass);
			fprintf(f, "pMomentR\n");
			fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].pMomentR.xx,dfobj.objPArr[i].pMomentR.xy,dfobj.objPArr[i].pMomentR.xz,dfobj.objPArr[i].pMomentR.yx,dfobj.objPArr[i].pMomentR.yy,dfobj.objPArr[i].pMomentR.yz,dfobj.objPArr[i].pMomentR.zx,dfobj.objPArr[i].pMomentR.zy,dfobj.objPArr[i].pMomentR.zz);
			fprintf(f, "pSmR\n");
			fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].pSmR.xx,dfobj.objPArr[i].pSmR.xy,dfobj.objPArr[i].pSmR.xz,dfobj.objPArr[i].pSmR.yx,dfobj.objPArr[i].pSmR.yy,dfobj.objPArr[i].pSmR.yz,dfobj.objPArr[i].pSmR.zx,dfobj.objPArr[i].pSmR.zy,dfobj.objPArr[i].pSmR.zz);
		//	fprintf(f, "ellAff\n");
			//fprintf(f, "%f %f %f %f  %f %f %f %f\n", dfobj.objPArr[i].ellAff.xx,dfobj.objPArr[i].ellAff.xy,dfobj.objPArr[i].ellAff.xz,dfobj.objPArr[i].ellAff.xw,dfobj.objPArr[i].ellAff.yx,dfobj.objPArr[i].ellAff.yy,dfobj.objPArr[i].ellAff.yz,dfobj.objPArr[i].ellAff.yw);
			//fprintf(f, "%f %f %f %f  %f %f %f %f\n", dfobj.objPArr[i].ellAff.zx,dfobj.objPArr[i].ellAff.zy,dfobj.objPArr[i].ellAff.zz,dfobj.objPArr[i].ellAff.zw,dfobj.objPArr[i].ellAff.px,dfobj.objPArr[i].ellAff.py,dfobj.objPArr[i].ellAff.pz,dfobj.objPArr[i].ellAff.pw);
			//fprintf(f, "dotCoeffMatrix\n");
			//fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].dotCoeffMatrix.xx,dfobj.objPArr[i].dotCoeffMatrix.xy,dfobj.objPArr[i].dotCoeffMatrix.xz,dfobj.objPArr[i].dotCoeffMatrix.yx,dfobj.objPArr[i].dotCoeffMatrix.yy,dfobj.objPArr[i].dotCoeffMatrix.yz,dfobj.objPArr[i].dotCoeffMatrix.zx,dfobj.objPArr[i].dotCoeffMatrix.zy,dfobj.objPArr[i].dotCoeffMatrix.zz);
			fprintf(f, "ellipRotMatrix\n");
			fprintf(f, "%f %f %f  %f %f %f  %f %f %f\n", dfobj.objPArr[i].ellipRotMatrix.xx,dfobj.objPArr[i].ellipRotMatrix.xy,dfobj.objPArr[i].ellipRotMatrix.xz,dfobj.objPArr[i].ellipRotMatrix.yx,dfobj.objPArr[i].ellipRotMatrix.yy,dfobj.objPArr[i].ellipRotMatrix.yz,dfobj.objPArr[i].ellipRotMatrix.zx,dfobj.objPArr[i].ellipRotMatrix.zy,dfobj.objPArr[i].ellipRotMatrix.zz);

			fprintf(f, "pMyGroupInd\n");
			fprintf(f, "%i\n",  dfobj.objPArr[i].pMyGroupInd);
			fprintf(f, "pObjId\n");
			fprintf(f, "%i\n",  dfobj.objPArr[i].pObjId);
			fprintf(f, "pRadii\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pRadii);
			//fprintf(f, "pMaybestableRadius\n");
			//fprintf(f, "%f\n",  dfobj.objPArr[i].pMaybestableRadius);
			fprintf(f, "pMainRadius\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pMainRadius);
			fprintf(f, "pSecRadius\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pSecRadius);
			fprintf(f, "pThrRadius\n");
			fprintf(f, "%f\n",  dfobj.objPArr[i].pThrRadius);

			fprintf(f, "pMainRadiusVec\n");
			fprintf(f, "%f %f %f\n", dfobj.objPArr[i].pMainRadiusVec.x,dfobj.objPArr[i].pMainRadiusVec.y,dfobj.objPArr[i].pMainRadiusVec.z);
			fprintf(f, "pSecRadiusVec\n");
			fprintf(f, "%f %f %f\n", dfobj.objPArr[i].pSecRadiusVec.x,dfobj.objPArr[i].pSecRadiusVec.y,dfobj.objPArr[i].pSecRadiusVec.z);
			fprintf(f, "pThrRadiusVec\n");
			fprintf(f, "%f %f %f\n", dfobj.objPArr[i].pThrRadiusVec.x,dfobj.objPArr[i].pThrRadiusVec.y,dfobj.objPArr[i].pThrRadiusVec.z);

			fprintf(f, "pInGrpList.size\n");
			fprintf(f, "%i\n",  dfobj.objPArr[i].pInGrpList.size());
			fprintf(f, "pInGrpList\n");
			for(unsigned int k=0;k<dfobj.objPArr[i].pInGrpList.size();k++)
			{
				int grpId = dfobj.objPArr[i].pInGrpList[k];
				fprintf(f, "%i\n",  grpId);
			}
		}
		fprintf(f, "weight\n");
		
		//weight
		for(int j = 0;j< dfobj.assPsNum;j++)
			{
				PHOpGroup &pg =  dfobj.objGArr[ dfobj.objPArr[j].pMyGroupInd];
				int gpSize = pg.gNptcl;
				for(int k=0;k< dfobj.objPArr[j].pNvertex;k++)
				 {
					  int vertind =  dfobj.objPArr[j].pVertArr[k];
					  for(int jm=1;jm < gpSize;jm++)
							{
						fprintf(f, "%f\n",  dfobj.objBlWeightArr[vertind][jm]);
					  }
				}
		}
		

		//params
		fprintf(f, "params\n");
		//fprintf(f, "vertMass\n");
		//fprintf(f, "%f\n",  dfobj.parobj);
		fprintf(f, "timeStep\n");
		fprintf(f, "%f\n", dfobj.params.timeStep);
		fprintf(f, "gravity\n");
		fprintf(f, "%f %f %f\n", dfobj.params.gravity.x, dfobj.params.gravity.y,dfobj.params.gravity.z);
		fprintf(f, "alpha\n");
		fprintf(f, "%f\n", dfobj.params.alpha);
		fprintf(f, "beta\n");
		fprintf(f, "%f\n", dfobj.params.beta);
		fprintf(f, "veloDamping\n");
		fprintf(f, "%f\n", dfobj.params.veloDamping);
		
		//fprintf(f, "%i\n", dfobj.params.volumeConservation);

		//bind faceinfo in particles
		fprintf(f, "fInPBindInfo\n");
		
		for(int i=0;i<dfobj.assPsNum;i++)
			{
				fprintf(f, "%i\n",dfobj.objPArr[i].pFaceInd.size());
				for(unsigned int j=0;j<dfobj.objPArr[i].pFaceInd.size();j++)
					fprintf(f, "%i\n", dfobj.objPArr[i].pFaceInd[j]);
		}
		fclose(f);
		std::cout<<" Success"<<endl;
	}
	
		
	void loadFromFile(PHOpObj &dfobj,char *filename, char *meshname)
	{
		std::cout<<" Load from file...";
		/*if(filename[0] == 'D')
			{
				filename = new char[255];
				SHGetSpecialFolderPathA(0,filename,CSIDL_DESKTOPDIRECTORY,0);
				char plus[100] = "\\opPHOpObj1.dfOp";
				strcat(filename,plus);
		}*/
		/*char plus[100] = "opPHOpObj1.dfOp";
		filename = plus;*/
		FILE *f;
		fopen_s(&f,filename, "r");
		if (!f)
			{
				std::cout<<" file dir can not open"<<endl;
				return;
		}

		if(dfobj.objUnNormalobj)
		{
			std::cout<<" do not load on to haptic controller"<<endl;
			fclose(f);
			return;
		}

		const int len = 100;
		char s[len+1];
		//int i;
		/*m2Vector pos;
		m2Real mass;
		int i;

		reset();*/
		//int numVerts;
		fgets(s, len, f);
		fgets(s, len, f);
		float *loadfvf = new float;
		sscanf_s(s, "%f", loadfvf);
		if(*loadfvf!=fileVersion)
		{
			std::cout<<"Failed please use correct version of file"<<endl;
			fclose(f);
			return;
		}
		fgets(s, len, f);
		fgets(s, len, f);
		bool sameName = true;
		int ln = strlen(s);
		int ln2 = strlen(meshname);
		if(ln>ln2)ln = ln2;
		for(int j=0;j<ln;j++)
			if (meshname[j] != s[j])
			{/*
			
			int ln2 = strlen(s);
			if(ln + 1==ln2)*/
				sameName = false;
			}
			
		if (sameName)
			;// dfobj.objTargetMesh->SetName(s);
		else {
			std::cout<<"Failed please use same model"<<endl;
			fclose(f);
			return;
		}
		fgets(s, len, f);
		if(s[0]!='G')fgets(s, len, f);
		fgets(s, len, f); 
		sscanf_s(s, "%i", &dfobj.assGrpNum);
		fgets(s, len, f);
		fgets(s, len, f); 
		sscanf_s(s, "%i", &dfobj.assPsNum);
		fgets(s, len, f);

		//if(dfobj.useTetgen)
		{

			//vertices
			for(int i = 0;i< dfobj.objTargetVtsNum;i++)
			{
				fgets(s, len, f); 
				sscanf_s(s, "%f %f %f", &dfobj.objTargetVts[i].x, &dfobj.objTargetVts[i].y, &dfobj.objTargetVts[i].z);
			}
		}
		
		fgets(s, len, f);
		//group
		dfobj.objGArr =  new PHOpGroup[dfobj.assGrpNum];
		for (int i = 0; i < dfobj.assGrpNum; i++) 
		{
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i", &dfobj.objGArr[i].gMyIndex);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objGArr[i].gCurrCenter.x,&dfobj.objGArr[i].gCurrCenter.y,&dfobj.objGArr[i].gCurrCenter.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i", &dfobj.objGArr[i].gNptcl);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objGArr[i].gOrigCenter.x,&dfobj.objGArr[i].gOrigCenter.y,&dfobj.objGArr[i].gOrigCenter.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f", &dfobj.objGArr[i].gtotalMass);
			fgets(s, len, f); 
			dfobj.objGArr[i].gPInd.clear();
			for(int j=0;j<dfobj.objGArr[i].gNptcl;j++)
			{
				dfobj.objGArr[i].gPInd.push_back(i);
				fgets(s, len, f); 
				sscanf_s(s,  "%i", &dfobj.objGArr[i].gPInd[j]);
			}
			
		}

		fgets(s, len, f);
		//ptcl
		dfobj.objPArr = new PHOpParticle[dfobj.assPsNum];
		for (int i = 0; i < dfobj.assPsNum; i++) 
		{
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s,  "%i", &dfobj.objPArr[i].pPId);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s,  "%i", &dfobj.objPArr[i].pNvertex);
			fgets(s, len, f); 
			dfobj.objPArr[i].pVertArr = new int[dfobj.objPArr[i].pNvertex];
			for(int j=0;j<dfobj.objPArr[i].pNvertex;j++)
			{	

				fgets(s, len, f); 
				sscanf_s(s, "%i", &dfobj.objPArr[i].pVertArr[j]);
			}
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objPArr[i].pOrigCtr.x,&dfobj.objPArr[i].pOrigCtr.y,&dfobj.objPArr[i].pOrigCtr.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f %f", &dfobj.objPArr[i].pOrigOrint.x,&dfobj.objPArr[i].pOrigOrint.y,&dfobj.objPArr[i].pOrigOrint.z,&dfobj.objPArr[i].pCurrOrint.w);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objPArr[i].pCurrCtr.x,&dfobj.objPArr[i].pCurrCtr.y,&dfobj.objPArr[i].pCurrCtr.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f %f", &dfobj.objPArr[i].pCurrOrint.x,&dfobj.objPArr[i].pCurrOrint.y,&dfobj.objPArr[i].pCurrOrint.z,&dfobj.objPArr[i].pCurrOrint.w);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pTotalMass);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i",  &dfobj.objPArr[i].isFixed);

			//fgets(s, len, f); 
			//int* incovSize = new int;
			//fgets(s, len, f); sscanf_s(s, "%i", incovSize);
			
			/*fgets(s, len, f); 
			for(int k=0;k<*incovSize;k++)
				{
					dfobj.objPArr[i].covMatrixList.push_back(Matrix3f());
					fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].covMatrixList[k].xx,&dfobj.objPArr[i].covMatrixList[k].xy,&dfobj.objPArr[i].covMatrixList[k].xz,&dfobj.objPArr[i].covMatrixList[k].yx,&dfobj.objPArr[i].covMatrixList[k].yy,&dfobj.objPArr[i].covMatrixList[k].yz,&dfobj.objPArr[i].covMatrixList[k].zx,&dfobj.objPArr[i].covMatrixList[k].zy,&dfobj.objPArr[i].covMatrixList[k].zz);
			}*/
			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].pMomentInertia.xx,&dfobj.objPArr[i].pMomentInertia.xy,&dfobj.objPArr[i].pMomentInertia.xz,&dfobj.objPArr[i].pMomentInertia.yx,&dfobj.objPArr[i].pMomentInertia.yy,&dfobj.objPArr[i].pMomentInertia.yz,&dfobj.objPArr[i].pMomentInertia.zx,&dfobj.objPArr[i].pMomentInertia.zy,&dfobj.objPArr[i].pMomentInertia.zz);
			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].pInverseOfMomentInertia.xx,&dfobj.objPArr[i].pInverseOfMomentInertia.xy,&dfobj.objPArr[i].pInverseOfMomentInertia.xz,&dfobj.objPArr[i].pInverseOfMomentInertia.yx,&dfobj.objPArr[i].pInverseOfMomentInertia.yy,&dfobj.objPArr[i].pInverseOfMomentInertia.yz,&dfobj.objPArr[i].pInverseOfMomentInertia.zx,&dfobj.objPArr[i].pInverseOfMomentInertia.zy,&dfobj.objPArr[i].pInverseOfMomentInertia.zz);

			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pTempSingleVMass);
			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].pMomentR.xx,&dfobj.objPArr[i].pMomentR.xy,&dfobj.objPArr[i].pMomentR.xz,&dfobj.objPArr[i].pMomentR.yx,&dfobj.objPArr[i].pMomentR.yy,&dfobj.objPArr[i].pMomentR.yz,&dfobj.objPArr[i].pMomentR.zx,&dfobj.objPArr[i].pMomentR.zy,&dfobj.objPArr[i].pMomentR.zz);
			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].pSmR.xx,&dfobj.objPArr[i].pSmR.xy,&dfobj.objPArr[i].pSmR.xz,&dfobj.objPArr[i].pSmR.yx,&dfobj.objPArr[i].pSmR.yy,&dfobj.objPArr[i].pSmR.yz,&dfobj.objPArr[i].pSmR.zx,&dfobj.objPArr[i].pSmR.zy,&dfobj.objPArr[i].pSmR.zz);
			//fgets(s, len, f);
			//fgets(s, len, f); sscanf_s(s, "%f %f %f %f  %f %f %f %f", &dfobj.objPArr[i].ellAff.xx,&dfobj.objPArr[i].ellAff.xy,&dfobj.objPArr[i].ellAff.xz,&dfobj.objPArr[i].ellAff.xw,
			//	&dfobj.objPArr[i].ellAff.yx,&dfobj.objPArr[i].ellAff.yy,&dfobj.objPArr[i].ellAff.yz,&dfobj.objPArr[i].ellAff.yw);

			//fgets(s, len, f); sscanf_s(s, "%f %f %f %f  %f %f %f %f",
			//	&dfobj.objPArr[i].ellAff.zx,&dfobj.objPArr[i].ellAff.zy,&dfobj.objPArr[i].ellAff.zz,&dfobj.objPArr[i].ellAff.zw,
			//	&dfobj.objPArr[i].ellAff.px,&dfobj.objPArr[i].ellAff.py,&dfobj.objPArr[i].ellAff.pz,&dfobj.objPArr[i].ellAff.pw);
			//fgets(s, len, f);
			//fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].dotCoeffMatrix.xx,&dfobj.objPArr[i].dotCoeffMatrix.xy,&dfobj.objPArr[i].dotCoeffMatrix.xz,&dfobj.objPArr[i].dotCoeffMatrix.yx,&dfobj.objPArr[i].dotCoeffMatrix.yy,&dfobj.objPArr[i].dotCoeffMatrix.yz,&dfobj.objPArr[i].dotCoeffMatrix.zx,&dfobj.objPArr[i].dotCoeffMatrix.zy,&dfobj.objPArr[i].dotCoeffMatrix.zz);
			fgets(s, len, f);
			fgets(s, len, f); sscanf_s(s, "%f %f %f  %f %f %f  %f %f %f", &dfobj.objPArr[i].ellipRotMatrix.xx,&dfobj.objPArr[i].ellipRotMatrix.xy,&dfobj.objPArr[i].ellipRotMatrix.xz,&dfobj.objPArr[i].ellipRotMatrix.yx,&dfobj.objPArr[i].ellipRotMatrix.yy,&dfobj.objPArr[i].ellipRotMatrix.yz,&dfobj.objPArr[i].ellipRotMatrix.zx,&dfobj.objPArr[i].ellipRotMatrix.zy,&dfobj.objPArr[i].ellipRotMatrix.zz);


			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i",  &dfobj.objPArr[i].pMyGroupInd);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i",  &dfobj.objPArr[i].pObjId);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pRadii);
			//fgets(s, len, f); 
			//fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pMaybestableRadius);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pMainRadius);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pSecRadius);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f",  &dfobj.objPArr[i].pThrRadius);

			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objPArr[i].pMainRadiusVec.x,&dfobj.objPArr[i].pMainRadiusVec.y,&dfobj.objPArr[i].pMainRadiusVec.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objPArr[i].pSecRadiusVec.x,&dfobj.objPArr[i].pSecRadiusVec.y,&dfobj.objPArr[i].pSecRadiusVec.z);
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.objPArr[i].pThrRadiusVec.x,&dfobj.objPArr[i].pThrRadiusVec.y,&dfobj.objPArr[i].pThrRadiusVec.z);

			int* ingrpSize = new int;
			fgets(s, len, f); 
			fgets(s, len, f); sscanf_s(s, "%i", ingrpSize);
			fgets(s, len, f); 
			for(int k=0;k<*ingrpSize;k++)
			{
				int grpId = 0;
				fgets(s, len, f);sscanf_s(s, "%i",  &grpId);
				dfobj.objPArr[i].pInGrpList.push_back(grpId);

			}


			//Vec3f *varr;varr = new Vec3f[dfobj.objPArr[i].pNvertex];
			//	for(int j = 0;j < dfobj.objPArr[i].pNvertex; j++)
			//		//varr[j] = targetDV.mPos[dParticleArr[i].pVertInd[j]];// * floatErrKill;//kill
			//		varr[j] = dfobj.targetMesh->vertices[dfobj.objPArr[i].pVertArr[j]];
			//	dfobj.objPArr[i].buildParticleCenter(varr);
		}
		fgets(s, len, f);

		//weight
		for(int j = 0;j< dfobj.assPsNum;j++)
			{
				PHOpGroup pg =  dfobj.objGArr[ dfobj.objPArr[j].pMyGroupInd];
				int gpSize = pg.gNptcl;
				for(int k=0;k< dfobj.objPArr[j].pNvertex;k++)
				{
					  int vertind =  dfobj.objPArr[j].pVertArr[k];
					  for(int jm=1;jm < gpSize;jm++)
					  {
						 fgets(s, len, f);
					  sscanf_s(s,"%f",  &dfobj.objBlWeightArr[vertind][jm]);
					  }
				}
		}
		
		 
		
		//for (int i = 0; i < numVerts; i++) {
		//	fgets(s, len, f); sscanf_s(s, "%f %f %f", &pos.x, &pos.y, &mass);
		//	addVertex(pos, mass);
		//	addColliedP();////
		//}
		fgets(s, len, f);
		//fgets(s, len, f); 
		//fgets(s, len, f); sscanf_s(s, "%f", &dfobj.vertMass);
		fgets(s, len, f); 
		fgets(s, len, f); sscanf_s(s, "%f", &dfobj.params.timeStep);
		fgets(s, len, f); 
		fgets(s, len, f); sscanf_s(s, "%f %f %f", &dfobj.params.gravity.x, &dfobj.params.gravity.y,&dfobj.params.gravity.z);
		fgets(s, len, f); 
		fgets(s, len, f); sscanf_s(s, "%f", &dfobj.params.alpha);
		fgets(s, len, f); 
		fgets(s, len, f); sscanf_s(s, "%f", &dfobj.params.beta);
		fgets(s, len, f); 
		fgets(s, len, f); sscanf_s(s, "%f", &dfobj.params.veloDamping);

		//fgets(s, len, f); sscanf_s(s, "%i", &i); dfobj.params.quadraticMatch = i;
		//fgets(s, len, f); sscanf_s(s, "%i", &i); dfobj.params.volumeConservation = i;
		//fgets(s, len, f); sscanf_s(s, "%i", &i); dfobj.params.allowFlip = i;

		//bind face info in particles
		//bind face size
		fgets(s, len, f);
		for(int i=0;i<dfobj.assPsNum;i++)
		{
			fgets(s, len, f);
			int *fsize = new int;
			sscanf_s(s, "%i",fsize);
			for(int j=0;j<*fsize;j++)
			{
				dfobj.objPArr[i].pFaceInd.size();
				fgets(s, len, f);
				int *index = new int;
				sscanf_s(s, "%i", index);
				dfobj.objPArr[i].pFaceInd.push_back(*index);
			}

		}

		fclose(f);
		dfobj.BuildBlendWeight();
		dfobj.buildDisWeightForVsinP();
		std::cout<<" Success"<<endl;
		//created = true;
	}
};

#endif