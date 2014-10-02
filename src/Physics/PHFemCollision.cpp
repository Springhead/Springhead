#include "PHFemCollision.h"

PHFemCollision::PHFemCollision() {
	fdt = 0.001;
	nPairs = 0;
	master = 1;
	slave = 0;
	k = 0;
	b = 0;
	firstDepth = -1.0;
}

///This function cleans the class properties
void PHFemCollision::clear(){
	this->nPairs =0;
	this->master=1;
	this->slave=0;
	this->k = 0;
	this->b =0;
	this->firstDepth = -1.0;
	this->pairs.clear();
	this->ivel->clear();
	this->idist->clear();
}

///This function calulates the first impact force in the collision of 
///two FEM objects. This force is caculated using the object's impulse
void PHFemCollision::firstContactImpact (PHFemMeshNew *mesh[2], PHContactPoint *cp)  
{
	std::vector<int> vertexInside[2];
	double avgEdge[2] = {0.0f, 0.0f};
		
	Vec3d instantVel[2];
	instantVel[0] = mesh[0]->ringVel->getLastOne();
	instantVel[1] = mesh[1]->ringVel->getLastOne();

	Vec3d instantAccel[2];
	PHSolid *solid[2];
	PHFemVibration *fvib[2];
	Vec3d Fv[2];
	Vec3d Ftotal;
	Vec3d normal;
	double mass[2];

	if ((firstDepth == -1.0) || (cp->shapePair->depth > firstDepth)) { 
		firstDepth = cp->shapePair->depth; }
		
	pairs.clear();
	vertexInside[0].clear(); 
	vertexInside[1].clear();
		
	marchVertices(mesh, vertexInside, cp, firstDepth, avgEdge); 

	//DEBUG BLOCK
	DSTR << "vertexInside: " << vertexInside[0].size() << std::endl;
	DSTR << "vertexInside: " << vertexInside[1].size() << std::endl;
	DSTR << "firstDepth: " << firstDepth << std::endl;

	/*for (int i=0; i < 2 ;i++) {
		DSTR << "vertex "<< mesh[i]->femIndex << " inside :" << std::endl;
		for (int k=0; k < vertexInside[i].size(); k++) {
			DSTR << "vi " << i << " : " << vertexInside[i][k] << std::endl;
		}
	}*/
	//DEBUG BLOCK

	matchVerticesFaces(mesh, vertexInside, cp, &pairs);

	if (vertexInside[0].size() < vertexInside[1].size()  ) {
		master =0; slave = 1;
	} else {
		master =1; slave = 0; }
		
	//DEBUG BLOCK
	DSTR << "pairs :" << std::endl;
	for (int kk=0; kk<(int)pairs.size() ; kk++) {
		DSTR << pairs[kk].vertexId << "-" << pairs[kk].faceId << std::endl;
	}
	DSTR << "pairs :" << std::endl;
	//DEBUG BLOCK

	solid[0] = mesh[0]->GetPHSolid()->Cast();
	solid[1] = mesh[1]->GetPHSolid()->Cast();

	fvib[0] = mesh[0]->GetPHFemVibration()->Cast();
	fvib[1] = mesh[1]->GetPHFemVibration()->Cast();

	normal = cp->shapePair->normal;

	instantVel[0] = (instantVel[0] * normal) * normal;
	instantVel[1] = (instantVel[1] * normal) * normal;

	nPairs = (int) pairs.size();
	

	Vec3d Fp;
	double reducedMass = 0;
	double impulseRedMass = 0;
	double damperRedMass = 0;

	mass[0] = solid[0]->GetMass();
	mass[1] = solid[1]->GetMass();
	double eCoeff = (solid[0]->GetShape(0)->GetElasticity() + solid[1]->GetShape(0)->GetElasticity()) /2;
	impulseRedMass = calcReducedMass(mass[0] , mass[1] ); 
	DSTR << "reducedmass for impulse: " << impulseRedMass << std::endl;

	//This parameter may be adjusted later
	damperRedMass = impulseRedMass;
	if (nPairs > 0)
		damperRedMass = damperRedMass / nPairs;

	DSTR << "reducedmass fo damping: " << damperRedMass << std::endl;

	//eCoeff = 0.97;   //3 PERCENT ENERGY LOST DUE VIBRATION.
	
	Fp = (2 * ((1+eCoeff) * impulseRedMass * (instantVel[1]-instantVel[0]))) / (fdt);
	Ftotal = Fp * (1-eCoeff);

	//DEBUG BLOCK
	//DSTR << "solidMass 0: " << mass[0] << std::endl;
	//DSTR << "solidMass 1: " << mass[1] << std::endl;
	//DSTR << "fv0: " << Fv[0] << std::endl;
	//DSTR << "fv1: " << Fv[1] << std::endl;
	//DSTR << "FP: " << Fp << std::endl;
	//DSTR << "FP2: "  << Fp * (1-eCoeff) << std::endl;
	//DSTR << "Fresto: "  << Fp * (eCoeff) << std::endl;
	//DSTR << "eCoeff: " << eCoeff << std::endl;
	//DSTR << "reducedmass: " << reducedMass << std::endl;
	//DSTR << "iv0: " << instantVel[0] << std::endl;
	//DSTR << "iv1: " << instantVel[1] << std::endl;
	//DSTR << "resta: " << instantVel[0]-instantVel[1] << std::endl;
	//DSTR << "nPairs: " << nPairs << std::endl;
	//DSTR << "sphere instant vel: " << instantVel[1] << std::endl;
	//DSTR << "contact Depth: " << cp->shapePair->depth << std::endl;
	//DSTR << "Ftotal: " << Ftotal << std::endl;
	//DEBUG BLOCK

	//The calculated impulse force is introduced in both FEM objects
	for (int i=0; i < nPairs ;i++) {
		fvib[master]->GetInstantPosition(pairs[i].vertexId, pairs[i].initialPos[master]);
		fvib[slave]->GetPosition(pairs[i].tetraId, pairs[i].projection, pairs[i].initialPos[slave], false);
		fvib[master]->AddVertexForceW(pairs[i].vertexId, Ftotal/nPairs);
		fvib[slave]->AddForce(pairs[i].tetraId, pairs[i].projection, Ftotal/nPairs);
		DSTR << "kinetic force sphere: " << Ftotal/nPairs << std::endl;
	}

	//K AND B CALCULATIONS FOR THE NEXT STEP
	//reducedMass = calcReducedMass( fvib[0]->GetVertexMass(vertexPair[0]).norm(), fvib[1]->GetVertexMass(vertexPair[1]).norm() );
	//reducedMass = reducedMass / 100;  //MODIFIED FOR STABILITY
	double effective = calcEffectiveYoung( fvib[0]->GetPoissonsRatio(), fvib[1]->GetPoissonsRatio(), fvib[0]->GetYoungModulus(), fvib[1]->GetYoungModulus());
	
	if (master == 0) {
		k =  effective  * avgEdge[0];
	}else {
		k =  effective  * avgEdge[1];
	}
	k = k /1000.0f;   //adjustment for integration
	b = 2 * sqrt(k * damperRedMass);  //ORIGINAL
	//b = fvib[0]->GetPoissonsRatio(); //EXPERIMENT

	//double natFreq = sqrt(k/damperRedMass);   //DEBUG ORIGINAL
	//double dampingRatio = b / ( 2.0 * damperRedMass * natFreq);  //DEBUG ORIGINAL

	double natFreq = sqrt(k/damperRedMass);   //DEBUG EXPERIMENT
	double dampingRatio = b / ( 2.0 * damperRedMass * natFreq);  //DEBUG EXPERIMENT
	double dampFreq = natFreq * sqrt(1- (dampingRatio*dampingRatio));  //DEBUG EXPERIMENT

	this->debugCount = 0;
	this->ivel[0]=Vec3d(0,0,0); this->ivel[1]=Vec3d(0,0,0);
	this->idist[0]=Vec3d(0,0,0); this->idist[1]=Vec3d(0,0,0);

	//DEBUG BLOCK
	//DSTR << " depth: " << cp->shapePair->depth << std::endl;
	DSTR << "avgEdge0: " << avgEdge[0] << 
		    "avgEdge1: " << avgEdge[1] <<std::endl;
	DSTR << "effective Y: " << effective << std::endl;
	DSTR << " reducedMass: " << reducedMass << std::endl;
	DSTR << "stiff: " << k << std::endl;
	DSTR << "damp: " << b << std::endl;
	DSTR << "damping ratio: " << dampingRatio << std::endl;
	DSTR << "natural freq: " << natFreq << std::endl;
	DSTR << "damp freq: " << dampFreq << std::endl;
	//DEBUG BLOCK
}

///This function calculates the spring and damper force which is
///excited by the vibration of both objects 
void PHFemCollision::VibrationInterface(PHFemMeshNew *mesh[2], PHContactPoint *cp)  
{
	std::stringstream str;
	for(int i= 0; i < nPairs ;i++) {
		PHFemVibration * fvib[2];
		Vec3d surfaceP[2];
		Posed pose[2];
		SpatialVector totalForce = SpatialVector();
		Vec3d normal = cp->shapePair->normal;

		fvib[master] = mesh[master]->GetPHFemVibration()->Cast();
		fvib[slave] = mesh[slave]->GetPHFemVibration()->Cast();

		fvib[master]->GetInstantPosition(pairs[i].vertexId, idist[master]);
		fvib[slave]->GetPosition(pairs[i].tetraId, pairs[i].projection, idist[slave], true);
			
		fvib[master]->GetInstantVelocity(pairs[i].vertexId, ivel[master]);
		fvib[slave]->GetVelocity(pairs[i].tetraId, pairs[i].projection, ivel[slave], true);

		idist[master] = (idist[master] - pairs[i].initialPos[master]) * normal * normal;
		idist[slave] =  (idist[slave] - pairs[i].initialPos[slave]) * normal * normal;

		ivel[master] = ivel[master] * normal * normal;
		ivel[slave] = ivel[slave] * normal * normal;

		totalForce.v() = -k * (idist[slave]-idist[master]) -b * (ivel[slave]-ivel[master]); 
		
		fvib[slave]->AddForceL(pairs[i].tetraId, pairs[i].projection, totalForce.v());
		fvib[master]->AddVertexForceL(pairs[i].vertexId, -totalForce.v());

		//DEBUG BLOCK
		if (i == 0)
			CSVOUT << i << "," << totalForce.vy << std::endl;
		
		//DEBUG BLOCK
	}
}

//This function calculates the reduced mass of two masses
double PHFemCollision::calcReducedMass(double mass1, double mass2)
{	if (mass1 + mass2 == 0) { return 0; }
	return ((mass1 * mass2)/(mass1 + mass2)); 
}

//Used to determine the connection stiffness
double PHFemCollision::calcEffectiveYoung(double possion1, double possion2, double young1, double young2)
{
	double a = 0;
	double b = 0;

	a  = (1 - (possion1 * possion1)) / (young1);  
	b  = (1 - (possion2 * possion2)) / (young2);  

	if (a + b == 0)
		return 0;

	return 1.0 / (a + b);
}


/// This function checks point by point with are the best matches 
/// between vertex and face, for the FEM objects involved in the collision.
void PHFemCollision::findVertexTrianglePairs(PHFemMeshNew *meshes[2], Vec3d contactNormal,  std::vector<FemFVPair> *pairs, std::vector<int> vertexInside[2], unsigned int hitIndex, int *deapest, bool bDeform ) {

	PHFemMeshNew *mesh0, *mesh1;
	std::vector<int> *vertexInside0;
	std::vector<int> *vertexInside1;
	bool *visitedArray;
	int nv [2];

	//Depeding on wich object has lees vertices inside the collision area
	if (hitIndex == 0) {
		mesh0 = meshes[0];
		mesh1 = meshes[1];
		vertexInside0 = &vertexInside[0];
		vertexInside1 = &vertexInside[1];
	} else { 
		mesh0 = meshes[1];
		mesh1 = meshes[0];
		vertexInside0 = &vertexInside[1];
		vertexInside1 = &vertexInside[0]; }

	
	Posed pose0 = mesh0->GetPHSolid()->GetPose();
	Posed pose1 = mesh1->GetPHSolid()->GetPose();
	PHFemVibration *fvib = mesh0->GetPHFemVibration()->Cast();
	nv[0] = (int) vertexInside0->size();
	nv[1] = (int) vertexInside1->size();
	Vec3d fp[3];
	visitedArray = new bool[mesh1->vertices.size()];
	memset(visitedArray, false, sizeof(bool) * mesh1->vertices.size());
	double dtmp = 0;
	double maxDist = DBL_MIN;

	//For each vertex inside the collision area
	for (int i=0; i < nv[0] ;i++) {
		for (int j=0; j< nv[1] ;j++) {

			int insideIndex = vertexInside0->at(i);
			int outsideIndex = vertexInside1->at(j);

			//If the vertex has been already connected with a face
			if (visitedArray[outsideIndex])  {continue;}   

			if (pairs->size() == 49) {
				DSTR << "error" << std::endl;
			}

			//distance check (for faster perfomance)
			if ( ((pose0 * mesh0->vertices[insideIndex].pos) - (pose1 * mesh1->vertices[outsideIndex].pos)).norm()  > mesh0->vertices[insideIndex].centerDist )
			{   continue; }

			maxDist = DBL_MIN;
			int nf = (int) mesh0->vertices[insideIndex].faceIDs.size();
			std::vector <int> facesIds = mesh0->vertices[insideIndex].faceIDs;
			for (int f=0; f < nf ;f++) {

				//surface face check
				if (facesIds[f] >= (int) mesh0->nSurfaceFace) {continue;}

				//normal check
				if (!((pose0 * mesh0->faces[facesIds[f]].normal).unit() * contactNormal > 0.899)) { continue; }

				fp->clear();
				for(int k = 0; k < 3; k++){
					if(bDeform)		fp[k] = pose0 * mesh0->vertices[mesh0->faces[facesIds[f]].vertexIDs[k]].pos;
					else			fp[k] = pose0 * mesh0->vertices[mesh0->faces[facesIds[f]].vertexIDs[k]].initialPos;
				}

				Vec3d point = pose1 * mesh1->vertices[outsideIndex].pos;

				//Tests if the projected point (point) is inside the face (fp)
				if (fvib->TestPointTriangle(fp, point) ) {
					double dist;
					Vec3d surfacePoint;

					//Projects (point) into the other mesh face (fp)
					fvib->FindClosestPointOnMesh(point, fp, surfacePoint, dist, bDeform);

					dtmp = (point-surfacePoint).norm();

					//Discards the point if it exceeds the depth threshold
					if ( dtmp > PEN_DEPTH)  {continue;}

					//Checks wich is the deapest point in contact
					if (maxDist < (surfacePoint - point).norm() ) {
						maxDist = (surfacePoint - point).norm();
						*deapest = (int) pairs->size();
					}

					//Finally the selected points are saved
					FemFVPair p;
					p.faceId = facesIds[f];
					p.vertexId = outsideIndex;
					p.femIndex = hitIndex;
					p.debugVertex = point;   //DEBUG
					p.debugMaster = mesh1->femIndex;  //DEBUG
					p.debugSlave = mesh0->femIndex;   //DEBUG
					p.projection = surfacePoint;
					p.dist = dist;
					p.tetraId = mesh0->faces[facesIds[f]].tetraId;
					pairs->push_back(p);
					visitedArray[outsideIndex] = true;
					break;
				}
			}
		}
	}
}


///This function matches the vertices of one FEM object with the faces of the other.
///(always considering the FEM object with the least vertices inside the collision area )
void PHFemCollision::matchVerticesFaces(PHFemMeshNew *meshes[2], std::vector<int> vertexInside[2], PHContactPoint *contactPoint, std::vector<FemFVPair> *pairs) {

	//int vin[2] = {};
	int deapest = -1;
	unsigned int hitObject = 0;
	bool spherical = false;
	Vec3d contactNormal;


	//checks if one of the objects is spheric (from SPR file)
	if (meshes[0]->spheric || meshes[1]->spheric)
		spherical = true;

	//checks wich object has less vertex inside the collision
	if (vertexInside[0].size() < vertexInside[1].size() )  
		hitObject = 1;
	else 
		hitObject = 0;

	//cheks the normal orientation for that specific collision
	contactNormal = contactPoint->shapePair->normal;
	if ((hitObject == 1) && (meshes[0]->femIndex < meshes[1]->femIndex) ) {
		contactNormal = -contactNormal;
	} else if ((hitObject == 0) && (meshes[1]->femIndex < meshes[0]->femIndex)  ){
		contactNormal = -contactNormal; } 

	//In case both objects have the same number of vertices inside //RESCUE FIX
	if (vertexInside[0].size() == vertexInside[1].size()) {
		contactNormal = contactPoint->shapePair->normal;
		hitObject = 0;
	}
	
	findVertexTrianglePairs(meshes, contactNormal, pairs, vertexInside, hitObject, &deapest, true);
		
	//ADD CODE HERE TO GET THE SPHERE FLAG FROM THE SPR FILE 
	if ((spherical) && (deapest != -1)) {
		FemFVPair tmp_pair;
		tmp_pair = pairs->at(deapest);
		pairs->clear();
		pairs->push_back(tmp_pair); 
	}

	meshes[0]->debugPairs = pairs;  //DEBUG
	meshes[1]->debugPairs = pairs;  //DEBUG
}


///This function calls the recursive function to select the vertices with the 
///best collision orientation. 
void PHFemCollision::marchVertices(PHFemMeshNew *meshes[2], std::vector<int> vertexInside[2], PHContactPoint *contactPoint, double firstDepth,  double avgEdge[2]) {

	Posed pose[2];
	PHSolid* solid[2];
	
	pose[0] = meshes[0]->GetPHSolid()->GetPose();
	pose[1] = meshes[1]->GetPHSolid()->GetPose();

	solid[0] = meshes[0]->GetPHSolid()->Cast();
	solid[1] = meshes[1]->GetPHSolid()->Cast();  

	bool *borderVertex[2];
	borderVertex[0] = new bool[meshes[0]->vertices.size()];
	borderVertex[1] = new bool[meshes[1]->vertices.size()];
	memset(borderVertex[0], false, sizeof(bool) * meshes[0]->vertices.size());
	memset(borderVertex[1], false, sizeof(bool) * meshes[1]->vertices.size());

	double sumEdge[2] = {0.0, 0.0}; 
	int numEdge[2] = {0.0, 0.0};

	int r = -1;
	vertexInside[0].clear();
	vertexInside[1].clear();
	if (meshes[0]->femIndex < meshes[1]->femIndex)  {
		r = recursiveSearch(meshes[0], &pose[0], solid[1], vertexPair[0], contactPoint, true, firstDepth, &sumEdge[0], &numEdge[0], &vertexInside[0], borderVertex[0]); 
		r = recursiveSearch(meshes[1], &pose[1], solid[0], vertexPair[1], contactPoint, false, firstDepth, &sumEdge[1], &numEdge[1], &vertexInside[1], borderVertex[1]); 
	}
	else {
		r = recursiveSearch(meshes[0], &pose[0], solid[1], vertexPair[0], contactPoint, false, firstDepth, &sumEdge[0], &numEdge[0], &vertexInside[0], borderVertex[0]); 
		r = recursiveSearch(meshes[1], &pose[1], solid[0], vertexPair[1], contactPoint, true, firstDepth, &sumEdge[1], &numEdge[1], &vertexInside[1], borderVertex[1]);
	}
	if (meshes[0]->femIndex == 0) {
	meshes[0]->debugVertexInside = &vertexInside[0];  //DEBUG 
	}
	if (meshes[1]->femIndex == 0) {
	meshes[1]->debugVertexInside = &vertexInside[1];  //DEBUG 
	}

	//DEBUG
	//DSTR << "DEBUG vertexinside 0 " << std::endl;
	//for (int i=0; i < vertexInside[0].size() ;i++) {
	//	DSTR << vertexInside[0].at(i) << "," ;
	//}
	//DSTR << std::endl << "DEBUG vertexinside 1 " << std::endl;
	//for (int i=0; i<vertexInside[1].size() ;i++) {
	//	DSTR << vertexInside[1].at(i) << "," ;
	//}
	//DSTR << std::endl;
	//DEBUG

	if (numEdge[0] == 0) { avgEdge[0]=0; }
	else {avgEdge[0] = sumEdge[0]/numEdge[0];}
	if (numEdge[1] == 0) { avgEdge[1]=0; }
	else {avgEdge[1] = sumEdge[1]/numEdge[1];}

	return;
}


///This is a recursive function to march the vertices and preselect 
///the ones best oriented to the collision area
int PHFemCollision::recursiveSearch(PHFemMeshNew *mesh, Posed* pose, PHSolid *solid, int vertexIndex, PHContactPoint *contactPoint, bool normOri, double firstDepth, double *sumEdge, int *numEdge, std::vector<int> *vertexInside, bool borderVertex[] ) {

	if (std::find(vertexInside->begin(), vertexInside->end(), vertexIndex) != vertexInside->end() )  //this can be optimized
		return 0;

	Vec3d vertexPos = *pose * mesh->vertices[vertexIndex].pos;

	//This old code considered the bbox and the contactAxis
	//if (!insideBBox(solid, vertexPos, contactAxis))
	//	return 1;

	//Removed from the code to include edge-face, vertex-face, vertex-vertex collisions
	//if (!distanceCheck(vertexPos, contactPoint, firstDepth)) {return 1;}

	int edgesSize = (int) mesh->vertices[vertexIndex].edgeIDs.size();
	vertexInside->push_back(vertexIndex);
	
	Vec3d vecNormal;
	if (normOri)
		vecNormal = *pose * (contactPoint->shapePair->normal);
	else
		vecNormal = *pose * (-contactPoint->shapePair->normal) ;

	for (int i=0; i< edgesSize ;i++) {
		
		int point;
		int edgeIndx = mesh->vertices[vertexIndex].edgeIDs[i];

		if (mesh->edges[edgeIndx].vertexIDs[0] == vertexIndex) {
			point = mesh->edges[edgeIndx].vertexIDs[1]; 
		} if (mesh->edges[edgeIndx].vertexIDs[1] == vertexIndex) {
			point = mesh->edges[edgeIndx].vertexIDs[0];  }

		if (edgeIndx > (int) mesh->nSurfaceEdge) { continue; }

		//first check the normal orientation
		//NOTE maybe with this check is enough, the distance calculation could be redundant (distanceCheck)
		//DSTR << vertexIndex << ":" << point << " : " << ((vertexPos - *pose * mesh->vertices[point].pos) * vecNormal) << std::endl;  //DEBUG
		Vec3d edgeVector = (vertexPos - *pose * mesh->vertices[point].pos);
		double orientCheck = edgeVector * vecNormal;
		if (abs(orientCheck) > ANGLE_THR ) {
			if (orientCheck > 0.1) { continue; }  //checks if the orientation is perpendicular
			borderVertex[vertexIndex] = true;
			continue; 
		}
		
		//NOTE: validate the sumEdge method return data
		*sumEdge = *sumEdge + edgeVector.norm();
		*numEdge = *numEdge + 1;
		int r = recursiveSearch(mesh, pose, solid, point, contactPoint, normOri, firstDepth, sumEdge, numEdge, vertexInside, borderVertex);
		if (r == 1) { borderVertex[vertexIndex] = true; *sumEdge = *sumEdge - edgeVector.norm(); *numEdge = *numEdge -1; }
		else if (r == 0) { *sumEdge = *sumEdge - edgeVector.norm(); *numEdge = *numEdge -1;}
	}
	return -1;
}


