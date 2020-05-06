/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemEngine.h>

using namespace std;

int phfemmode=1;	//Thermo: mode=0 ;  Vibration : mode=1; //Default:1


namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// FEMEngine
PHFemEngine::PHFemEngine(){
	fdt = 0.02;
}

void PHFemEngine::SetPHFemMode(int mode){
	//Thermo: mode=0 ;  Vibration : mode=1;
	phfemmode=mode;
	if(phfemmode==THERMO_MODE){
		std::cout<<"PHFemMode: Thermo"<<std::endl;
		std::cout<<"ContactInterface Disabled "<<std::endl;
	}
	else if(phfemmode==VIBRATION_MODE){
		std::cout<<"PHFemMode: Vibration"<<std::endl;
		std::cout<<"ContactInterface Enabled "<<std::endl;
	}
}

void PHFemEngine::Step(){
	/// FEM Interface for vibration transmission
	if(phfemmode==VIBRATION_MODE){
		if (meshes_n.size() > 1) {
			this->ContactInterface();
		}
	}
	/// 旧メッシュの更新
	for(size_t i = 0; i < meshes.size(); ++i){
		meshes[i]->Step(GetTimeStep());
	}

	/// 新メッシュの更新
	for(size_t i = 0; i < meshes_n.size(); ++i){
		meshes_n[i]->Step(GetTimeStep());
	}
}

void PHFemEngine::Clear(){}

void PHFemEngine::SetTimeStep(double dt){
	fdt = dt;
}

double PHFemEngine::GetTimeStep(){
	return fdt;
}

bool PHFemEngine::AddChildObject(ObjectIf* o){
	PHFemMesh* mesh = o->Cast();
	if(mesh){
		meshes.push_back(mesh);
		return true;
	}
	PHFemMeshNew* mesh_n = o->Cast();
	if(mesh_n){
		mesh_n->femIndex = (int) meshes_n.size();
		meshes_n.push_back(mesh_n);
		for (int i = 0; i < (int)meshes_n.size(); ++i) {
			meshes_n[i]->contactVector.resize(meshes_n.size());
		}
		return true;
	}
	return false;
}

///Used to find the PHFemMesh object using the solid's name
PHFemMeshNew* PHFemEngine::GetMeshByName(std::string name){
	int nm = (int) meshes_n.size();
	for (int i=0; i<nm  ;i++) {
		if (meshes_n[i]->GetPHSolid()->GetName() == name)
			return meshes_n[i];
	}
	return NULL;
}

///Used to match the solids with their respective FEM Mesh
void PHFemEngine::FEMSolidMatchRefresh() {
	int nm = (int) meshes_n.size();
	PHSolid *temp;
	for (int i=0; i<nm  ;i++) {
		temp = meshes_n[i]->GetPHSolid()->Cast();
		temp->femIndex = meshes_n[i]->femIndex;
	}

	//This block initializes the contact matrix
	contactMatrix = new bool*[nm];
	for (int j=0; j< nm ;j++)
		contactMatrix[j] = new bool(false);
}

/// This function encapulates the FEM vibration transmission interface
void PHFemEngine::ContactInterface() {
	PHSceneIf* phscene = GetScene();
	int nc = phscene->NContacts();

	//For all the contacts in the scene
	for (int j=0; j<nc ;j++) 	{
		PHContactPoint *cp =  phscene->GetContact(j)->Cast();
		std::string s0, s1;
		data face[2];

		s0 = cp->solid[0]->GetName();
		s1 = cp->solid[1]->GetName();
		PHFemMeshNew *mesh[2];
		PHSolid *solid[2]; 
		mesh[0] = meshes_n[cp->solid[0]->femIndex];
		mesh[1] = meshes_n[cp->solid[1]->femIndex];
		assert(mesh[0] != NULL);
		assert(mesh[1] != NULL);

		solid[0] = mesh[0]->GetPHSolid()->Cast();
		solid[1] = mesh[1]->GetPHSolid()->Cast();
			
		bool c0 = mesh[0]->contactVector[mesh[1]->femIndex];   //check if this is slow
		bool c1 = mesh[1]->contactVector[mesh[0]->femIndex];

		//Checks if the contact point is between 2 objects that have already been visited 
		if ((mesh[0] != NULL && mesh[1] != NULL) && !( c0 && c1)) 	
		{
			mesh[0]->contactVector[mesh[1]->femIndex] = true;
			mesh[1]->contactVector[mesh[0]->femIndex] = true;

			data tpos[2];
			Vec3d contactNormal;
			contactNormal = cp->shapePair->normal.unit();

			//This block handles the initial impact of both FEM objects
			//Remember this block has variables that are initialized on BuildScene() 
			if (cp->shapePair->state == CDShapePair::NEW)  
			{
				int faceId[2];
				PHFemVibration *fvib[2];
				Vec3d surfacePoint[2];
				Vec3d pivotPoint;

				//Creates a new FEM collision object to handle the FEM interface calculations
				PHFemCollision *femcol;
				DSTR << "NEW " << mesh[0]->femIndex << "-" <<mesh[1]->femIndex << std::endl;   //DEBUG
				if (femColArray[mesh[0]->femIndex][mesh[1]->femIndex] == NULL) {
					femcol = new PHFemCollision;
					femcol->fdt = fdt;
						
					//DEBUG FILE BLOCK
					//std::ostringstream oss;
					//oss << "debuggy" << mesh[0]->femIndex << "-" << mesh[1]->femIndex <<".csv";
					//femcol->myDebugFile.open(oss.str());
					//femcol->myDebugFile << setprecision(16);
					//DEBUG FILE BLOCK
						
					femColArray[mesh[1]->femIndex][mesh[0]->femIndex] = femcol;
					femColArray[mesh[0]->femIndex][mesh[1]->femIndex] = femcol;
					DSTR << "OBJETO creado..." << mesh[0]->GetName() << "-" << mesh[1]->GetName() << std::endl;  //DEBUG
				}
				else
					femcol = femColArray[mesh[0]->femIndex][mesh[1]->femIndex];

				pivotPoint = cp->shapePair->center;
				mesh[0]->debugPivotPoint = pivotPoint;  //DEBUG
				mesh[1]->debugPivotPoint = pivotPoint;  //DEBUG 

				//rutine to find the closest tetrahedra from the pivot point 
				//THI BLOCK CAN BE OPTIMIZED USING THE KDTREE AND THE CONTACT NORMAL ORIENTATION
				fvib[0] = mesh[0]->GetPHFemVibration()->Cast();
				fvib[1] = mesh[1]->GetPHFemVibration()->Cast();
				if (mesh[0]->femIndex < mesh[1]->femIndex) { // The normal is on mesh[0] surface
					fvib[0]->searchSurfaceTetra(pivotPoint, cp->shapePair->normal, faceId[0], surfacePoint[0], false);
					fvib[1]->searchSurfaceTetra(pivotPoint, -cp->shapePair->normal, faceId[1], surfacePoint[1], false);
				} else {                                     // The normal is on mesh[1] 
					fvib[0]->searchSurfaceTetra(pivotPoint, -cp->shapePair->normal, faceId[0], surfacePoint[0], false);
					fvib[1]->searchSurfaceTetra(pivotPoint, cp->shapePair->normal, faceId[1], surfacePoint[1], false);
				}
				int *t1 = mesh[0]->faces[faceId[0]].vertexIDs;
				int *t2 = mesh[1]->faces[faceId[1]].vertexIDs;
				double tempDist = 0;
				double minDist = DBL_MAX;

				//This block finds the closest vertex from the founded 
				//closest tetrahedra face 
				for (int k=0; k< 3; k++){ 
					for (int l=0; l<3; l++) {
						//tempDist = dist( cp->shapePair->shapePoseW[0] * mesh[0]->vertices[ t1[k] ].pos , cp->shapePair->shapePoseW[1] * mesh[1]->vertices[ t2[l] ].pos ); 
						tempDist = ((cp->shapePair->shapePoseW[0] * mesh[0]->vertices[t1[k]].pos) - (cp->shapePair->shapePoseW[1] * mesh[1]->vertices[ t2[l] ].pos)).norm(); 
						if (tempDist < minDist) {
							femcol->vertexPair[0] = t1[k];   femcol->vertexPair[1] = t2[l];
							minDist = tempDist;
						}
					}
				}
				mesh[0]->debugFoundPoint = cp->shapePair->shapePoseW[0] * mesh[0]->vertices[femcol->vertexPair[0]].pos;  //DEBUG
				mesh[1]->debugFoundPoint = cp->shapePair->shapePoseW[1] * mesh[1]->vertices[femcol->vertexPair[1]].pos;  //DEBUG

				//Checks the contact in the contact matrix
				contactMatrix[mesh[1]->femIndex][mesh[0]->femIndex] = true;
				contactMatrix[mesh[0]->femIndex][mesh[1]->femIndex] = true;

				femcol->firstContactImpact(mesh, cp);
			}
			else if (cp->shapePair->state == CDShapePair::CONTINUE) {
				//This block handles the vibration transmition after the first impact contact
				
				//If the collision object haven't been created it yet
				PHFemCollision *femcol;
				if (femColArray[mesh[0]->femIndex][mesh[1]->femIndex] == NULL) {
					femcol = new PHFemCollision;
					femcol->fdt = fdt;
						
					femColArray[mesh[1]->femIndex][mesh[0]->femIndex] = femcol;
					femColArray[mesh[0]->femIndex][mesh[1]->femIndex] = femcol;
				}
				else
					femcol = femColArray[mesh[0]->femIndex][mesh[1]->femIndex];

				//Checks the contact in the contact matrix
				contactMatrix[mesh[1]->femIndex][mesh[0]->femIndex] = true;
				contactMatrix[mesh[0]->femIndex][mesh[1]->femIndex] = true;
				femcol->VibrationInterface(mesh, cp);
			}
			else if (cp->shapePair->state == CDShapePair::NONE) {
				DSTR << "NONE " << mesh[0]->femIndex << "-" <<mesh[1]->femIndex << std::endl;   //DEBUG
			}
		}
	}
	//clears contact matrix
	if(nc){		
		this->clearContacts();
	}
}

///Initilizes the contacts between the objects 
void PHFemEngine::InitContacts() {

	//This block initializes the contact matrix
	int nfems = (int) meshes_n.size();
	contactMatrix = new bool*[nfems];
	for (int j=0; j< nfems ;j++)
		contactMatrix[j] = new bool(false);

	//This block initializes the femCollision object matrix
	femColArray = new PHFemCollision**[nfems]; 
	for (int j=0; j < nfems; j++) 
		femColArray[j] = new PHFemCollision*[nfems];

	for (int i=0; i < nfems; i++)
		for (int j=0; j < nfems; j++)
			femColArray[i][j] = NULL;
}


///Clears from the memory the unused femColArray objects
///checking if both objects are still in contact with each other
void PHFemEngine::clearContacts(){
	int nfems = (int) meshes_n.size();

	//clears the femCollision object from the memory
	for(int i=0; i< nfems; i++){
		for(int j=i; j < nfems; j++) {
			if ((!contactMatrix[i][j]) && (femColArray[i][j] != NULL)) {
				delete femColArray[i][j];
				femColArray[i][j] = NULL;

				meshes_n[i]->debugPairs = NULL; //CLEANING DEBUG VARIABLES
				meshes_n[j]->debugPairs = NULL; //CLEANING DEBUG VARIABLES
			}
		}
	}

	//clears the contact array 
	for(int i=0; i< nfems; i++){
		for(int j=i; j < nfems; j++) {
			contactMatrix[i][j] = false;
			if (i != j)
				contactMatrix[j][i] = false;
		}
	}
}
void PHFemEngine::ClearContactVectors() {
	for (size_t j = 0; j < meshes_n.size(); j++)
		for (size_t k = 0; k < meshes_n.size(); k++)
			meshes_n[j]->contactVector[k] = false;
}

}
