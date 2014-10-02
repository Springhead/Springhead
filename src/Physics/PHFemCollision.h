#ifndef PHFEMCOLLISION_H
#define PHFEMCOLLISION_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

//#include <Physics\PHFemEngine.h>
#include <Physics/PHFemMeshNew.h>
#include <Physics/PHFemVibration.h>
#include <Physics/PHContactPoint.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/RingBuffer.h>
#include <iomanip> 

//using namespace Spr;
using namespace std;

//#define EDGE_EPS 1e-3  //Original Susa's threshold
#define ANGLE_THR 0.08715574274765817355806427083747  //85 degrees threshold
#define PEN_DEPTH 4e-3  // 4mm


class PHFemCollision
{
public:
	int vertexPair[2];
	double fdt;


	int nPairs;
	int master;
	int slave;
	std::vector<FemFVPair> pairs;
	Vec3d idist[2];  
	Vec3d ivel[2];  
	double k;
	double b;
	double firstDepth;

	PHFemCollision();
	void clear();

	
	void firstContactImpact(PHFemMeshNew *mesh[2], PHContactPoint *cp);  
	double calcReducedMass(double mass1, double mass2);
	double calcEffectiveYoung(double possion1, double possion2, double young1, double young2);
	
	void matchVerticesFaces(PHFemMeshNew *meshes[2], std::vector<int> vertexInside[2], PHContactPoint *contactPoint, std::vector<FemFVPair> *pairs);
	void findVertexTrianglePairs(PHFemMeshNew *meshes[2], Vec3d contactNormal,  std::vector<FemFVPair> *pairs, std::vector<int> vertexInside[2], unsigned int hitIndex, int *deapest, bool bDeform );
	void marchVertices(PHFemMeshNew *meshes[2], std::vector<int> vertexInside[2], PHContactPoint *contactPoint, double firstDepth,  double avgEdge[2]); 
	int recursiveSearch(PHFemMeshNew *mesh, Posed* pose, PHSolid *solid, int vertexIndex, PHContactPoint *contactPoint, bool normOri, double firstDepth, double *sumEdge, int *numEdge, std::vector<int> *vertexInside, bool borderVertex[] );
	
	void VibrationInterface(PHFemMeshNew *mesh[2], PHContactPoint *cp);  

	bool isNull(); 

	//DEBUG VARIABLES
	int debugCount;  //DEBUG
	ofstream myDebugFile;   //DEBUG FILE

};

#endif