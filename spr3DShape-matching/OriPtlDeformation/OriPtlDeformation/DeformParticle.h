//---------------------------------------------------------------------------

#ifndef DEFORMPARTICLEH
#define DEFORMPARTICLEH
//---------------------------------------------------------------------------

#include <vector>
#include <Springhead.h>
#include "m2Real.h"
#include "m3Bounds.h"
#include "objloader.h"
#include <..\src\Graphics\GRMesh.h>

using namespace Spr;
struct BoundPoint
{
	float beP;
	int id;
	bool operator < (BoundPoint & b) { 
              return beP < b.beP;}
};
struct BoundRect
{
	public: BoundPoint bound[4];
		void setId(int id){for(int i=0;i<4;i++)
						   bound[i].id = id;}
		
};


//
//struct ColliedPair
//{
//	public: int objId,posIndex;
//			Vec3f otherObjPos;
//			float moveDis;
//			Vec3f otherObjVel;
//};


struct DeformableParameters
{
	DeformableParameters() { setDefaults(); }

	//SortBound sortBound;
	//BoundPoint xbp,ybp;
	BoundRect collBound;//ﾅｲｾﾘﾐﾎ

	float timeStep;
	Vec3f gravity;
	m3Bounds bounds;
	float radius;

	float alpha;
    float beta;
	float gama;

    bool quadraticMatch;
	bool volumeConservation;

	bool allowFlip;
	bool flipLoopFighter;

	void setDefaults();
	void setBoundId(int id);
};


//---------------------------------------------------------------------------
class DeformParticle
{
public:
    DeformParticle();
	~DeformParticle();

	void gravityStep();
	void reset();
	void addVertex(const Vec3f &pos, float mass);

	void externalForces();
	void projectPositions();
	void projectPosCluster(int* indexs, int count);
	void integrate();
	void integratewithCluster();

	void timeStep();
	void timeStep16BasedCluster(GRMesh *grMesh);
	void timeStep8BasedCluster(GRMesh *grMesh);
	void integretcluster(int beginNum,int indexes[],int &clusterNum);

	DeformableParameters params;
	

	int  getNumVertices() const { return mNumVertices; }
	/*const Vec3f & getVertexPos(int nr) { return mPos[nr]; }
	const Vec3f & getOriginalVertexPos(int nr) { return mOriginalPos[nr]; }
	const Vec3f & getGoalVertexPos(int nr) { return mGoalPos[nr]; }
	const m2Real getMass(int nr) { return mMasses[nr]; }*/
	//
	const bool getColliedP(int i) { return mCollied[i]; }
	const void setColliedP(int i) { mCollied[i] = true; }
	const void setColliedPF(int i) { mCollied[i] = false; }
	const void addColliedP() {mCollied.push_back(false);}
	const void resetColliedF() { mCollied.clear();}
	const std::vector<int> & getColliedPosIndexVInB() { return mColliedPosIndexInBound; }
	
	const void addColliedPosInB(int posIndex) { 
												for(int i=0;i<mColliedPosIndexInBound.size();i++)
												{
													if(posIndex==mColliedPosIndexInBound[i])
														return;

												}
												mColliedPosIndexInBound.push_back(posIndex);
										  }
	void deleteColliedPosInB(int posIndex);
	const void resetColliedPosInB() { mColliedPosIndexInBound.clear(); }

	//const std::vector<int> &getColliedPosIndexV() { return mColliedPosIndex; }
	int getColliedPosIndex(int index){return mColliedPosIndex[index];}
	void addColliedPosIndex(int posIndex);
	void deleteColliedPos(int posIndex);
	bool checkCollied(int posIndex){
									for(int i=0;i<mColliedPosIndex.size();i++)
										{
											if(mColliedPosIndex[i] == posIndex)
												return true;
										}
									return false;
									}
	/*bool checkCollied2(int posIndex){
									for(int i=0;i<mColliedPair.size();i++)
										{
											if(mColliedPair[i].posIndex == posIndex)
												return true;
										}
									return false;
										}*/
	void fixVertex(int nr, const Vec3f &pos);//ﾊ・羹ﾏｶｯｵﾄｵ・
	bool isFixed(int nr) { return mFixed[nr]; }
	void releaseVertex(int nr);

	void saveToFile(char *filename);
	void loadFromFile(char *filename);

	void collisionForces();
	void collisionOnlySetApart();
	void colliedMoveAlongVel();

	void findTheBound(m2Real rB);
	void setId(int i){id = i;}
	void setR(Matrix3f &R);
	//ﾅｲｶﾔ
	
	/*void addCPair(int objid,int posI,Vec3f otherobjPos,float distance,Vec3f otherobjvel){
		for(int i=0;i<mColliedPair.size();i++)
		{
			if(mColliedPair[i].posIndex == posI)
				return;
		}
		ColliedPair cp;
		cp.objId = objid;
		cp.posIndex = posI;
		cp.otherObjPos = otherobjPos;
			
		cp.moveDis = distance;
			cp.otherObjVel = otherobjvel;
			mColliedPair.push_back(cp);
	
	}*/
	/*void deleteCPair(int deleteint){mColliedPair.erase(mColliedPair.begin()+deleteint);}
	ColliedPair getCPair(){mColliedPair.pop_back();}*/
	/*int getCPsize(){return mColliedPair.size();}
	void deleteAllColliedPosIndex()
	{
		for(int i=0;i<mColliedPair.size();i++)
		{
			deleteCPair(i);
		}
	}
	Vec3f getVelocity(int index){
	return mVelocities[index];
	}*/

	//--------------------------------------------------------

	Matrix3f MatrixesMultiply3f(Matrix3f &res, Matrix3f &other);
	Vec3f MatrixMultiVec3f(Matrix3f &res, Vec3f &other);
	
	std::vector<Vec3f> mDrawVertices;
	/*std::vector<Vec3f>* getPos(){return &mPos;}
	std::vector<Vec3f>* getNewPos(){return &mNewPos;}
	std::vector<Vec3f>* getVelocities(){return &mVelocities;}*/
	std::vector<Vec3f> mPos;
	std::vector<Vec3f> mNewPos;
	std::vector<Vec3f> mCluNewPos;
	std::vector<Vec2f>  mUvs;
	std::vector<Vec3f>  mNormals;
	std::vector<bool> mFixed;
	std::vector<float> mMasses;
	std::vector<Vec3f> mOriginalPos;
	std::vector<Vec3f> mGoalPos;

	bool LoadObjToDPs(char* objFilePath){return loadOBJ(objFilePath,mDrawVertices,mPos,mUvs,mNormals);}
	bool debugStepOn;
	bool created;
	//GRMesh grMesh;


private:
	void initState();

	int mNumVertices;
	
	//std::vector<Vec3f> mPos;
	//std::vector<Vec3f> mNewPos;
	
	
	std::vector<Vec3f> mVelocities;
    
	std::vector<bool> mCollied;
	std::vector<int> mColliedPosIndexInBound;//ﾅｲｿﾐｵﾄﾅｲｵ・
	std::vector<int> mColliedPosIndex;//ｷ｢ﾉ愰ｲｵﾄﾅｲｵ・
	//std::vector<ColliedPair> mColliedPair;//ｷ｢ﾉ愰ｲｵﾄﾅｲｵ羝ﾔ
	int id;
};

#endif
