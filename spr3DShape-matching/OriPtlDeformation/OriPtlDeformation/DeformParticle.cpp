
#include "DeformParticle.h"
#include <stdio.h>
#include <math.h>
#include "m2Real.h"
#include <iostream>
#include"PolarDecomposition.h"

#include <GL/glut.h>
#include "SprCollision.h"
#include "Math3d\m9Matrix.h"

using namespace Spr;

void DeformParticle::gravityStep()
{
	int i=0;
	bool xChanged = false,yChanged = false,zChanged = false;
	for (i = 0; i < mNumVertices; i++) {
    	if (mFixed[i])
		{
			continue;
		}
		mVelocities[i] += params.gravity * params.timeStep;//ﾋﾙｶﾈ=ﾖﾘﾁｦ*ﾊｱｼ・
		mNewPos[i] = mPos[i] + mVelocities[i] * params.timeStep;//ﾄｿｱ・ｻﾖﾃ=ﾔｭﾎｻﾖﾃ+ﾋﾙｶﾈ*ﾊｱｼ・
		//mGoalPos[i] = mOriginalPos[i];
	}

	// boundaries
	m2Real restitution = 0.9f;//ｱﾟｽ邏ｦﾀ惞ｿ｣ｬｵｯﾇｽｵﾄｵｯﾐﾔ｣ｿ｣ｿ
	
	for (i = 0; i < mNumVertices; i++) {
    	if (mFixed[i]) continue;
		Vec3f &p = mPos[i];
		Vec3f &np = mNewPos[i];
		Vec3f &v = mVelocities[i];
		//もし境界に超えたら、npを改めて計算
		if (np.x < params.bounds.min.x || np.x > params.bounds.max.x) {
			np.x = p.x - v.x * params.timeStep * restitution;
			np.y = p.y;// - v.y * params.timeStep * (1-restitution);
			np.z = p.z;// - v.z * params.timeStep * (1-restitution);
			xChanged = true;
		}
		if (np.y < params.bounds.min.y || np.y > params.bounds.max.y) {
			np.y = p.y - v.y * params.timeStep * restitution;
			if(!xChanged)
				np.x = p.x;// - v.x * params.timeStep * (1-restitution);
			np.z = p.z;
			yChanged = true;
			//np.z = p.z;// - v.z * params.timeStep * (1-restitution);
		}
		if (np.z < params.bounds.min.z || np.z > params.bounds.max.z) {
			np.z = p.z - v.z * params.timeStep * restitution;
			if(!xChanged)
				np.x = p.x;// - v.x * params.timeStep * (1-restitution);
			if(!yChanged)
				np.y = p.y;// - v.x * params.timeStep * (1-restitution);
			zChanged = true;
		}
        params.bounds.clamp(mNewPos[i]);//ｱﾟｽ邏ｦﾀ惞ｬｱ｣ｳﾖﾔﾚｱﾟｽ釉ﾐ
		
	}
}
class DeformVertex//: public Vertex
{
public:
	//CDQHPlane<QhullVertex>* horizon;
	Vec3f pos;
	Vec3f velocity;
	///  頂点の位置
	Vec3f GetPos() const{ return pos;}
	//@}
	///	位置の設定
	void SetPos(Vec3f p){ pos = p; }
	
};

class Triangle{
public:
	int vtx[3];	//	頂点0～2
	int tri[3];	//	となりの三角形。tri[0]が頂点0と1を共有する。
	Vec3f normal;
	double area;
	bool deleted;
	Triangle():deleted(false){}
	bool HaveVtx(int v){
		return vtx[0] == v || vtx[1] == v || vtx[2] == v;
	}
	bool HaveTri(int t){
		return tri[0] == t || tri[1] == t || tri[2] == t;
	}
	int FindTri(int t){
		if (tri[0] == t) return 0;
		if (tri[1] == t) return 1;
		assert(tri[2] == t);
		return 2;
	}
};
//---------------------------------------------------------------------------
void DeformableParameters::setDefaults()
{
	
	timeStep = 0.01f;
	//gravity.z = -9.81f;
	gravity.y = -9.81f;
	//bounds = new m3Bounds(Vec3f(0,0,0),Vec3f(1,1,1));
	//bounds.min.Zero();
	//bounds.max.set(1.0f, 1.0f);// not original
	bounds.min.x = -9;
	bounds.min.y = -3;
	bounds.min.z = -9;
	bounds.max.x = 9;
	bounds.max.y = 9;
	bounds.max.z = 9;
	radius = 0.2;

	alpha = 0.10f;
	beta = 0.00f;
	gama = 1.0f;

	quadraticMatch = false;
	volumeConservation = false;

	allowFlip = false;
	//allowFlip = false;
	
	flipLoopFighter = false;
}

void DeformableParameters::setBoundId(int id)
{
	collBound.setId(id);
	
}
//---------------------------------------------------------------------------
DeformParticle::DeformParticle()
{
	reset();
}

//---------------------------------------------------------------------------
DeformParticle::~DeformParticle()
{
}


//---------------------------------------------------------------------------
void DeformParticle::reset()
{
	mNumVertices = 0;
	mOriginalPos.clear();
	mPos.clear();
	mNewPos.clear();
	mGoalPos.clear();
	mMasses.clear();
	mVelocities.clear();
    mFixed.clear();
	mCollied.clear();
	mCluNewPos.clear();
	created = false;
}

//---------------------------------------------------------------------------
void DeformParticle::initState()//ｳｼｻｯｳ｡ｾｰ
{
	for (int i = 0; i < mNumVertices; i++) {
		mPos[i] = mOriginalPos[i];
		mNewPos[i] = mOriginalPos[i];
		mGoalPos[i] = mOriginalPos[i];
		mCluNewPos[i] = mOriginalPos[i];
		mVelocities[i].Zero();
        mFixed[i] = false;
		mCollied[i] = false;
	}
	debugStepOn = false;
}

//---------------------------------------------------------------------------
void DeformParticle::addVertex(const Vec3f &pos, float mass)//ｼﾓﾈ・･ｵ・
{
	mOriginalPos.push_back(pos);
	mPos.push_back(pos);
	mNewPos.push_back(pos);
	mCluNewPos.push_back(pos);
	mGoalPos.push_back(pos);
	mMasses.push_back(mass);
	mVelocities.push_back(Vec3f(0,0,0));
    mFixed.push_back(false);
	//
	mCollied.push_back(false);
	mNumVertices++;


	initState();
}

//---------------------------------------------------------------------------
//void DeformParticle::colliedMoveAlongVel()
//{
//	if(mColliedPair.size()<=0)
//		return;
//	for (int i = 0; i < mColliedPair.size(); i++) 
//	{
//		ColliedPair cp = mColliedPair[i];
//
//		Vec3f colliedP1 = mPos[cp.posIndex];
//		Vec3f colliedP2 = cp.otherObjPos;
//		Vec3f relativePosV;
//		relativePosV.x = colliedP2.x-colliedP1.x;
//		relativePosV.y = colliedP2.y-colliedP1.y;
//		if(abs(relativePosV.x)<0.000001)relativePosV.x = 0;
//		if(abs(relativePosV.y)<0.000001)relativePosV.y = 0;
//
//		Vec3f myVel = mVelocities[cp.posIndex];
//		Vec3f otherVel  = cp.otherObjVel;
//		float colliRestitution = 1.0;
//
//
//		relativePosV.normalize();
//
//		Vec3f myVelx = myVel.vectorProjection(relativePosV);//ﾋ羌ﾚﾏ犖ﾔｷｽﾏﾏｵﾄﾋﾙｶﾈﾍｶﾓｰ
//		Vec3f otherVelx = otherVel.vectorProjection(relativePosV);
//		Vec3f myVely = myVel- myVelx ;//ｷﾖｽ箋ﾄyｷｽﾏﾖﾁｿ
//
//		myVelx =((myVelx+otherVelx)*0.5-myVelx)*colliRestitution;
//
//		myVel = myVelx+myVely;//ｻｹﾔｭﾋﾙｶﾈ
//
//		mNewPos[cp.posIndex] += myVel*params.timeStep;
//		deleteCPair(i);
//	}
//}
//void DeformParticle::collisionOnlySetApart()
//{
//	if(mColliedPair.size()<=0)
//		return;
//	for (int i = 0; i < mColliedPair.size(); i++) 
//	{
//		ColliedPair cp = mColliedPair[i];
//
//		Vec3f colliedP1 = mPos[cp.posIndex];//ｱ・ﾋﾒｪｸﾄｳﾉﾔ､ｲ箏羞ﾄﾊｱｺﾄﾕ簑・
//		Vec3f colliedP2 = cp.otherObjPos;
//		Vec3f relativePosV;
//		relativePosV.x = colliedP1.x-colliedP2.x;
//		relativePosV.y = colliedP1.y-colliedP2.y;
//		cp.moveDis*=1000;
//
//		float len = relativePosV.magnitudeSquared();
//		len*=1000;
//		
//		m2Real proportion = cp.moveDis/len;
//		
//		relativePosV = relativePosV * cp.moveDis/len/2;
//
//		if(relativePosV.x>0.001f)
//			{
//				int y = 0;
//				relativePosV.x=0.001f;
//				
//				//return;
//		}
//		if(relativePosV.y>0.01f)//ｲｻﾖｪｵﾀﾎｪﾉｶｻ蘯ﾜｴｬｺﾃﾏﾇｾ狢・ﾆﾋ耨ﾐﾎﾊﾌ・
//			relativePosV.y=0.001f;
//
//		if(relativePosV.x<0.00001f)
//				relativePosV.x=0.0f;
//			
//		if(relativePosV.y<0.00001f)
//			relativePosV.y=0.0f;
//
//		float attenuationEcoff =0.9f;
//		for(int j =1;j<mNumVertices;j++)//j+1
//		{
//			if((relativePosV*attenuationEcoff).x<0.000001)
//				{
//					
//					continue;
//			}
//			int setIndex = cp.posIndex;
//			if(cp.posIndex+j>mNumVertices-1)//ﾔｽｽ逎ｬｶﾔﾐﾅﾖﾜﾎｧｵﾄｵ耡ｲﾊｩｼﾓﾁｦ｣ｬｵｫﾎ・螻ﾘﾐ・ｴﾐﾅﾋｳﾐｭｳ・
//				setIndex = cp.posIndex+j-(mNumVertices-1);
//			mNewPos[setIndex] +=relativePosV*attenuationEcoff;
//			if(cp.posIndex-j<0)//ﾔｽｽ・
//				setIndex = -(cp.posIndex-j);
//			mNewPos[setIndex] +=relativePosV*attenuationEcoff;
//			if(attenuationEcoff>0)
//				attenuationEcoff-=0.1f;
//			
//			
//
//		}
//
//		mNewPos[cp.posIndex] +=relativePosV;
//
//		deleteCPair(i);
//	}
//
//}
//void DeformParticle::collisionForces()
//{//ﾒﾅﾁﾊﾌ筌ｬｸﾃﾖｻﾒﾆｶｯﾅｲｵ罐ｬｻｹﾊﾇﾒﾆｶｯﾈｫｲｿｵ罐ｿ
//	if(mColliedPair.size()<=0)
//		return;
//	for (int i = 0; i < mColliedPair.size(); i++) 
//	{
//		
//	}
//}
void DeformParticle::externalForces()//ﾍ簔ｦ
{
	int i;

	for (i = 0; i < mNumVertices; i++) {
    	if (mFixed[i]) continue;
		mVelocities[i] += params.gravity * params.timeStep;//ﾋﾙｶﾈ=ﾖﾘﾁｦ*ﾊｱｼ・
		mNewPos[i] = mPos[i] + mVelocities[i] * params.timeStep;//ﾄｿｱ・ｻﾖﾃ=ﾔｭﾎｻﾖﾃ+ﾋﾙｶﾈ*ﾊｱｼ・
		mGoalPos[i] = mOriginalPos[i];
	}

	// boundaries
	m2Real restitution = 0.9f;//ｱﾟｽ邏ｦﾀ惞ｿ｣ｬｵｯﾇｽｵﾄｵｯﾐﾔ｣ｿ｣ｿ
	
	for (i = 0; i < mNumVertices; i++) {
    	if (mFixed[i]) continue;
		Vec3f &p = mPos[i];
		Vec3f &np = mNewPos[i];
		Vec3f &v = mVelocities[i];
		//もし境界に超えたら、npを改めて計算
		if (np.x < params.bounds.min.x || np.x > params.bounds.max.x) {
			np.x = p.x - v.x * params.timeStep * restitution;
			np.y = p.y;
		}
		if (np.y < params.bounds.min.y || np.y > params.bounds.max.y) {
			np.y = p.y - v.y * params.timeStep * restitution;
			np.x = p.x;
		}
        params.bounds.clamp(mNewPos[i]);//ｱﾟｽ邏ｦﾀ惞ｬｱ｣ｳﾖﾔﾚｱﾟｽ釉ﾐ
	}

}
//---------------------------------------------------------------------------
void DeformParticle::projectPositions()//ﾎｻﾖﾃｵﾄｴｦﾀ惞｡｣｡
{
	if (mNumVertices <= 1) return;
	int i,j,k;

	// center of mass
	Vec3f cm, originalCm;
	cm.Zero(); originalCm.Zero();
    float mass = 0.0f;

	for (i = 0; i < mNumVertices; i++) {//ｼﾆﾋ罍｣｡｣｡｣ｺﾍﾗﾜﾖﾊﾁｿ
    	m2Real m = mMasses[i];
        //if (mFixed[i]) m *= 100.0f;
		 if (mFixed[i]) m *= 1.0f;
    	mass += m;
		cm += mNewPos[i] * m;
		originalCm += mOriginalPos[i] * m;
	}

    cm /= mass;
    originalCm /= mass;

	Matrix3f Apq, Aqq;
	Vec3f p,q;
	Apq = Apq.Zero();
	Aqq = Aqq.Zero();


	for (i = 0; i < mNumVertices; i++) {
		p = mNewPos[i] - cm;
		q = mOriginalPos[i] - originalCm;
		m2Real m = mMasses[i];
		Apq.ExX() += m * p.x * q.x;
		Apq.ExY() += m * p.x * q.y;
		Apq.ExZ() += m * p.x * q.z; 
		Apq.EyX() += m * p.y * q.x;
		Apq.EyY() += m * p.y * q.y;
		Apq.EyZ() += m * p.y * q.z; 
		Apq.EzX() += m * p.z * q.x; 
		Apq.EzY() += m * p.z * q.y;
		Apq.EzZ() += m * p.z * q.z;


		Aqq.ExX() += m * q.x * q.x;
		Aqq.ExY() += m * q.x * q.y;
		Aqq.ExZ() += m * q.x * q.z;
		Aqq.EyX() += m * q.y * q.x;
		Aqq.EyY() += m * q.y * q.y;
		Aqq.EyZ() += m * q.y * q.z;
		Aqq.EzX() += m * q.z * q.x;
		Aqq.EzY() += m * q.z * q.y;
		Aqq.EzZ() += m * q.z * q.z;

		//Apq.ExX() += m * p.x * q.x;//不是这里的问题
		//Apq.ExY() += m * p.y * q.x;
		//Apq.ExZ() += m * p.z * q.x; 
		//Apq.EyX() += m * p.x * q.y;
		//Apq.EyY() += m * p.y * q.y;
		//Apq.EyZ() += m * p.z * q.y; 
		//Apq.EzX() += m * p.x * q.z; 
		//Apq.EzY() += m * p.y * q.z;
		//Apq.EzZ() += m * p.z * q.z;


	}

	if (!params.allowFlip && Apq.det() < 0.0f) {  	// prevent from flippingﾈ･ｶｶｶｯ｣ｿ   //not original
		Apq.ExX() = -Apq.ExX();						
		Apq.EyX() = -Apq.EyX();
		Apq.EzX() = -Apq.EzX();
		//Apq.ExY() = -Apq.ExY();						
		//Apq.EyY() = -Apq.EyY();
		//Apq.EzY() = -Apq.EzY();
		//Apq.ExZ() = -Apq.ExZ();						//防止循环抖动下一次不执行
		//Apq.EyZ() = -Apq.EyZ();
		//Apq.EzZ() = -Apq.EzZ();
		//std::cout<<"Apq flipped"<<std::endl;
    }

	Matrix3f R,S;
	polarDecomposition pd;
		//pd.polarDecom(R,S,Apq);  //not original
	//pd.qDUDecomposition(R,Apq); 
		pd.NewPolarDecomposition(Apq,R);
		//R = R.trans();
	//setR(R);

	m2Real A5pq[3][9];
	m9Matrix A5qq;
	m2Real q5[9];
	m2Real A5[3][9];
	Matrix3f A;
    if (!params.quadraticMatch) {	// --------- linear match
		 A = Aqq;
		A = A.inv();
		//A.multi(Apq, A);//ｼﾆﾋ譱ｪﾊｼﾄﾇｸ=Apq*Aqq  //not original
		A = MatrixesMultiply3f(Apq,A);

        if (params.volumeConservation) {//ﾌ蟾e､｣ｴ讀ｫ､ﾉ､ｦ､ｫ//ﾔﾚpage5ﾒｳｿｪﾊｼ｡｢ﾈｴﾎﾔｪｼﾆﾋ耡ｪｿｪﾈｴﾎｷｽｵﾄﾑﾓ｡｣｡｣｡｣
	        m2Real det = A.det();//ｼﾆﾋ耙ﾐﾁﾐﾊｽ｣ｬﾈｻｵﾈﾓﾚ0｣ｬｶﾔﾆ荼ﾔ1ｳﾈ｡ｾﾔﾖｵﾔﾙｿｪｷｽ｣ｬｼｴ1/ｿｪｷｽ|det|
	        if (det != 0.0f) {
            	det = 1.0f / sqrt(fabs(det));
                if (det > 3.0f) det = 3.0f;//ｽ盪錥鋗ｵﾎｪ2｣ｬﾖｮｺｫAｳﾋﾒﾔdet
		        A *= det;
            }
    	}

		Matrix3f T = R * (1.0f - params.beta) + A * params.beta;//ｼﾆﾋ羈莉ｻｾﾘﾕ・bataｵﾄﾊｹﾓﾃ/page4ﾒｳﾓﾒｵﾗ

		//Matrix3f T = R * (1.0f - params.beta) + A * params.beta;

        for (i = 0; i < mNumVertices; i++) {
            if (mFixed[i]) continue;
            q = mOriginalPos[i] - originalCm;
             //mGoalPos[i] = T.multiply(q) + cm;  //not original
			mGoalPos[i] = MatrixMultiVec3f(T,q) + cm;
			
            mNewPos[i] += (mGoalPos[i] - mNewPos[i]) * params.alpha;//ｸﾝﾄｿｱ・羮ﾆﾋ耙ﾂｵ・alphaｵﾄﾊｹﾓﾃ/page4ﾒｳﾓﾒｵﾗ
        }
    }

	
	else {	// -------------- quadratic match---------------------

        //m2Real A5pq[3][9];
        for(int i=0;i<3;i++)
			for(int j=0;j<9;j++)
				A5pq[i][j] = 0.0f;
        //m9Matrix A5qq;
        A5qq.zero();

        for (int i = 0; i < mNumVertices; i++) {
            p = mNewPos[i] - cm;
            q = mOriginalPos[i] - originalCm;
			//m2Real q5[9];
			q5[0] = q.x; q5[1] = q.y; q5[2] =q.z; q5[3] =q.x*q.x; q5[4] = q.y*q.y; q5[5] = q.z*q.z;
			q5[6] = q.x*q.y;q5[7] = q.y*q.z;q5[8] = q.z*q.x;

            m2Real m = mMasses[i];
			
			for(int jm= 0;jm<9;jm++)
			{	
				A5pq[0][jm] += m * p.x * q5[jm];
				A5pq[1][jm] += m * p.y * q5[jm];
				A5pq[2][jm] += m * p.z * q5[jm];
			}
			
			for (j = 0; j < 9; j++)
                for (k = 0; k < 9; k++)
                    A5qq(j,k) += m * q5[j]*q5[k];
        }

        A5qq.invert();

        //m2Real A5[3][9];
        for (int i = 0; i < 3; i++) {
            for (j = 0; j < 9; j++) {
                A5[i][j] = 0.0f;
                for (k = 0; k < 9; k++) {
                    A5[i][j] += A5pq[i][k] * A5qq(k,j);
                }
                A5[i][j] *= params.beta;
                if (j < 3)
                    A5[i][j] += (1.0f - params.beta) * R[i][j];//???????????j<2
            }
        }

       /* m2Real det = A5[0][0]+A5[1][1]+A5[2][2]
					+A5[0][1]+A5[1][2]+A5[2][0]
					+A5[2][0]+A5[1][0]+A5[2][1]
					-A5[2][0]-A5[1][1]-A5[0][2]
					-A5[2][1]-A5[1][2]-A5[0][0]
					-A5[2][2]-A5[1][0]-A5[0][1];*/
		/*det = A5[0][0]+A5[1][1]+A5[2][2]
					+A5[1][0]+A5[2][1]+A5[0][2]
					+A5[0][2]+A5[0][1]+A5[1][2]
					-A5[0][2]-A5[1][1]-A5[2][0]
					-A5[1][2]-A5[2][1]-A5[0][0]
					-A5[2][2]-A5[0][1]-A5[1][0];*/

		//m2Real det = A5[0][0]*A5[1][1] - A5[0][1]*A5[1][0];
   //     if (!params.allowFlip && det < 0.0f) {         		// prevent from flipping
			////det is wrong?
   //        	A5[0][1] = -A5[0][1];
   //         A5[1][1] = -A5[1][1];
			//A5[2][1] = -A5[2][1];
			//
   //     }

       /* if (params.volumeConservation) {
	        if (det != 0.0f) {
            	det = 1.0f / sqrt(fabs(det));
                if (det > 2.0f) det = 2.0f;
		        A5[0][0] *= det; A5[0][1] *= det;
                A5[1][0] *= det; A5[1][1] *= det;
            }
    	}*/


        for (int i = 0; i < mNumVertices; i++) {
            if (mFixed[i]) continue;
            q = mOriginalPos[i] - originalCm;
			
			mGoalPos[i].x = A5[0][0]*q.x + A5[0][1]*q.y + A5[0][2]*q.z + A5[0][3]*q.x*q.x + A5[0][4]*q.y*q.y + A5[0][5]*q.z*q.z +
				 A5[0][6]*q.x*q.y + A5[0][7]*q.y*q.z + A5[0][8]*q.z*q.x;
            mGoalPos[i].y = A5[1][0]*q.x + A5[1][1]*q.y + A5[1][2]*q.z + A5[1][3]*q.x*q.x + A5[1][4]*q.y*q.y + A5[1][5]*q.z*q.z +
				 A5[1][6]*q.x*q.y + A5[1][7]*q.y*q.z + A5[1][8]*q.z*q.x;
			mGoalPos[i].z = A5[2][0]*q.x + A5[2][1]*q.y + A5[2][2]*q.z + A5[2][3]*q.x*q.x + A5[2][4]*q.y*q.y + A5[2][5]*q.z*q.z +
				 A5[2][6]*q.x*q.y + A5[2][7]*q.y*q.z + A5[2][8]*q.z*q.x;
            mGoalPos[i] += cm;
            mNewPos[i] += (mGoalPos[i] - mNewPos[i]) * params.alpha;
		}
    }
}

void DeformParticle::projectPosCluster(int indexs[16], int count)//ﾎｻﾖﾃｵﾄｴｦﾀ惞｡｣｡
{
	if (count == 3) return;
	if (count <= 1) return;
	int i,j,k;

	// center of mass
	Vec3f cm, originalCm;
	cm.Zero(); originalCm.Zero();
    float mass = 0.0f;

	for (i = 0; i < count; i++) {//ｼﾆﾋ罍｣｡｣｡｣ｺﾍﾗﾜﾖﾊﾁｿ
    	m2Real m = mMasses[indexs[i]];
        if (mFixed[indexs[i]]) m = 10.0f;
    	mass += m;
		cm += mNewPos[indexs[i]] * m;
		originalCm += mOriginalPos[indexs[i]] * m;
	}

    cm /= mass;
    originalCm /= mass;

	Matrix3f Apq, Aqq;
	Vec3f p,q;
	Apq.Zero();
	Aqq.Zero();

	//true zero
		Apq.xx = 0;
		Apq.yy = 0;
		Apq.zz = 0;
		Aqq.xx = 0;
		Aqq.yy = 0;
		Aqq.zz = 0;

	for (i = 0; i < count; i++) {
		p = mNewPos[indexs[i]] - cm;
		q = mOriginalPos[indexs[i]] - originalCm;
		m2Real m = mMasses[indexs[i]];
		Apq.ExX() += m * p.x * q.x;
		Apq.ExY() += m * p.x * q.y;
		Apq.ExZ() += m * p.x * q.z; 
		Apq.EyX() += m * p.y * q.x;
		Apq.EyY() += m * p.y * q.y;
		Apq.EyZ() += m * p.y * q.z; 
		Apq.EzX() += m * p.z * q.x; 
		Apq.EzY() += m * p.z * q.y;
		Apq.EzZ() += m * p.z * q.z;


		Aqq.ExX() += m * q.x * q.x;
		Aqq.ExY() += m * q.x * q.y;
		Aqq.ExZ() += m * q.x * q.z;
		Aqq.EyX() += m * q.y * q.x;
		Aqq.EyY() += m * q.y * q.y;
		Aqq.EyZ() += m * q.y * q.z;
		Aqq.EzX() += m * q.z * q.x;
		Aqq.EzY() += m * q.z * q.y;
		Aqq.EzZ() += m * q.z * q.z;

		//Apq.ExX() += m * p.x * q.x;//不是这里的问题
		//Apq.ExY() += m * p.y * q.x;
		//Apq.ExZ() += m * p.z * q.x; 
		//Apq.EyX() += m * p.x * q.y;
		//Apq.EyY() += m * p.y * q.y;
		//Apq.EyZ() += m * p.z * q.y; 
		//Apq.EzX() += m * p.x * q.z; 
		//Apq.EzY() += m * p.y * q.z;
		//Apq.EzZ() += m * p.z * q.z;


	}

	if (!params.allowFlip && Apq.det() < 0.0f) {  	// prevent from flippingﾈ･ｶｶｶｯ｣ｿ   //not original
		Apq.ExX() = -Apq.ExX();						
		Apq.EyX() = -Apq.EyX();
		Apq.EzX() = -Apq.EzX();
		//Apq.ExY() = -Apq.ExY();						
		//Apq.EyY() = -Apq.EyY();
		//Apq.EzY() = -Apq.EzY();
		//Apq.ExZ() = -Apq.ExZ();						//防止循环抖动下一次不执行
		//Apq.EyZ() = -Apq.EyZ();
		//Apq.EzZ() = -Apq.EzZ();
		//std::cout<<"Apq flipped"<<std::endl;
    }

	Matrix3f R,S;
	polarDecomposition pd;
		//pd.polarDecom(R,S,Apq);  //not original
	//pd.qDUDecomposition(R,Apq); 
		pd.NewPolarDecomposition(Apq,R);
		//R = R.trans();
	//setR(R);

	m2Real A5pq[3][9];
	m9Matrix A5qq;
	m2Real q5[9];
	m2Real A5[3][9];
	Matrix3f A;
    if (!params.quadraticMatch) {	// --------- linear match
		 A = Aqq;
		A = A.inv();
		//A.multi(Apq, A);//ｼﾆﾋ譱ｪﾊｼﾄﾇｸ=Apq*Aqq  //not original
		A = MatrixesMultiply3f(Apq,A);
		//beta错了可能是乘法反了
        if (params.volumeConservation) {//ﾌ蟾e､｣ｴ讀ｫ､ﾉ､ｦ､ｫ//ﾔﾚpage5ﾒｳｿｪﾊｼ｡｢ﾈｴﾎﾔｪｼﾆﾋ耡ｪｿｪﾈｴﾎｷｽｵﾄﾑﾓ｡｣｡｣｡｣
	        m2Real det = A.det();//ｼﾆﾋ耙ﾐﾁﾐﾊｽ｣ｬﾈｻｵﾈﾓﾚ0｣ｬｶﾔﾆ荼ﾔ1ｳﾈ｡ｾﾔﾖｵﾔﾙｿｪｷｽ｣ｬｼｴ1/ｿｪｷｽ|det|
	        if (det != 0.0f) {
            	det = 1.0f / sqrt(fabs(det));
                if (det > 3.0f) det = 3.0f;//ｽ盪錥鋗ｵﾎｪ2｣ｬﾖｮｺｫAｳﾋﾒﾔdet
		        A *= det;
            }
    	}
		
		Matrix3f T = R * (1.0f - params.beta) + A * params.beta;//ｼﾆﾋ羈莉ｻｾﾘﾕ・bataｵﾄﾊｹﾓﾃ/page4ﾒｳﾓﾒｵﾗ

        for (i = 0; i < count; i++) {
            if (mFixed[indexs[i]]) continue;
            q = mOriginalPos[indexs[i]] - originalCm;
             //mGoalPos[i] = T.multiply(q) + cm;  //not original
			mGoalPos[indexs[i]] = MatrixMultiVec3f(T,q) + cm;
			
            //mCluNewPos[indexs[i]] += (mGoalPos[indexs[i]] - mNewPos[indexs[i]]) * params.alpha;//ｸﾝﾄｿｱ・羮ﾆﾋ耙ﾂｵ・alphaｵﾄﾊｹﾓﾃ/page4ﾒｳﾓﾒｵﾗ
			mNewPos[indexs[i]] += (mGoalPos[indexs[i]] - mNewPos[indexs[i]]) * params.alpha;//ｸﾝﾄｿｱ・羮ﾆﾋ耙ﾂｵ・alphaｵﾄﾊｹﾓﾃ/page4ﾒｳﾓﾒｵﾗ
        }
    }

	
	else {	// -------------- quadratic match---------------------

        //m2Real A5pq[3][9];
        for(int i=0;i<3;i++)
			for(int j=0;j<9;j++)
				A5pq[i][j] = 0.0f;
        //m9Matrix A5qq;
        A5qq.zero();
		

        for (int i = 0; i < count; i++) {
            p = mNewPos[indexs[i]] - cm;
            q = mOriginalPos[indexs[i]] - originalCm;
			//m2Real q5[9];
			q5[0] = q.x; q5[1] = q.y; q5[2] =q.z; q5[3] =q.x*q.x; q5[4] = q.y*q.y; q5[5] = q.z*q.z;
			q5[6] = q.x*q.y;q5[7] = q.y*q.z;q5[8] = q.z*q.x;

            m2Real m = mMasses[indexs[i]];
			
			for(int jm= 0;jm<9;jm++)
			{	
				A5pq[0][jm] += m * p.x * q5[jm];
				A5pq[1][jm] += m * p.y * q5[jm];
				A5pq[2][jm] += m * p.z * q5[jm];
			}
			
			for (j = 0; j < 9; j++)
                for (k = 0; k < 9; k++)
                    A5qq(j,k) += m * q5[j]*q5[k];
        }

        A5qq.invert();

        //m2Real A5[3][9];
        for (int i = 0; i < 3; i++) {
            for (j = 0; j < 9; j++) {
                A5[i][j] = 0.0f;
                for (k = 0; k < 9; k++) {
                    A5[i][j] += A5pq[i][k] * A5qq(k,j);
                }
                A5[i][j] *= params.beta;
                if (j < 3)
                    A5[i][j] += (1.0f - params.beta) * R[i][j];//???????????j<2
            }
        }

#if 0
        m2Real det = A5[0][0]+A5[1][1]+A5[2][2]
					+A5[0][1]+A5[1][2]+A5[2][0]
					+A5[2][0]+A5[1][0]+A5[2][1]
					-A5[2][0]-A5[1][1]-A5[0][2]
					-A5[2][1]-A5[1][2]-A5[0][0]
					-A5[2][2]-A5[1][0]-A5[0][1];
		/*m2Real det = A5[0][0]+A5[1][1]+A5[2][2]
					+A5[1][0]+A5[2][1]+A5[0][2]
					+A5[0][2]+A5[0][1]+A5[1][2]
					-A5[0][2]-A5[1][1]-A5[2][0]
					-A5[1][2]-A5[2][1]-A5[0][0]
					-A5[2][2]-A5[0][1]-A5[1][0];*/

		//m2Real det = A5[0][0]*A5[1][1] - A5[0][1]*A5[1][0];
        if (!params.allowFlip && det < 0.0f) {         		// prevent from flipping
			//det is wrong?
           	A5[0][1] = -A5[0][1];
            A5[1][1] = -A5[1][1];
			A5[2][1] = -A5[2][1];
			std::cout<<"A5 Fliped"<<std::endl;
			
        }
#endif
       /* if (params.volumeConservation) {
	        if (det != 0.0f) {
            	det = 1.0f / sqrt(fabs(det));
                if (det > 2.0f) det = 2.0f;
		        A5[0][0] *= det; A5[0][1] *= det;
                A5[1][0] *= det; A5[1][1] *= det;
            }
    	}*/


        for (int i = 0; i < count; i++) {
            if (mFixed[indexs[i]]) continue;
            q = mOriginalPos[indexs[i]] - originalCm;
			
			mGoalPos[indexs[i]].x = A5[0][0]*q.x + A5[0][1]*q.y + A5[0][2]*q.z + A5[0][3]*q.x*q.x + A5[0][4]*q.y*q.y + A5[0][5]*q.z*q.z +
				 A5[0][6]*q.x*q.y + A5[0][7]*q.y*q.z + A5[0][8]*q.z*q.x;
            mGoalPos[indexs[i]].y = A5[1][0]*q.x + A5[1][1]*q.y + A5[1][2]*q.z + A5[1][3]*q.x*q.x + A5[1][4]*q.y*q.y + A5[1][5]*q.z*q.z +
				 A5[1][6]*q.x*q.y + A5[1][7]*q.y*q.z + A5[1][8]*q.z*q.x;
			mGoalPos[indexs[i]].z = A5[2][0]*q.x + A5[2][1]*q.y + A5[2][2]*q.z + A5[2][3]*q.x*q.x + A5[2][4]*q.y*q.y + A5[2][5]*q.z*q.z +
				 A5[2][6]*q.x*q.y + A5[2][7]*q.y*q.z + A5[2][8]*q.z*q.x;
            mGoalPos[indexs[i]] += cm;
            //mCluNewPos[indexs[i]] += (mGoalPos[indexs[i]] - mNewPos[indexs[i]]) * params.alpha;
			mNewPos[indexs[i]] += (mGoalPos[indexs[i]] - mNewPos[indexs[i]]) * params.alpha;
		}
    }
}
void DeformParticle::setR(Matrix3f &R)
{
	R.xx = 0;R.xy = 0;R.xz = 1;
	R.yx = 0;R.yy = 1;R.yz = 0;
	R.zx = 1;R.zy = 0;R.zz = 0;
}

//---------------------------------------------------------------------------
void DeformParticle::integrate()
{
	m2Real dt1 = 1.0f / params.timeStep;
	for (int i = 0; i < mNumVertices; i++) {
		mVelocities[i] = (mNewPos[i] - mPos[i]) * dt1;
		mPos[i] = mNewPos[i];
	}

}

void DeformParticle::integratewithCluster()
{
	m2Real dt1 = 1.0f / params.timeStep;
	for (int i = 0; i < mNumVertices; i++) {
		mVelocities[i] = ((1 - params.gama) * mNewPos[i] - mPos[i] + (mCluNewPos[i] * params.gama)) * dt1;

		mNewPos[i] = mCluNewPos[i];

		mPos[i] = mNewPos[i];
	}

}

//---------------------------------------------------------------------------
void DeformParticle::timeStep()//ﾃｿﾒｻｴﾎﾊｱｼ茣ｽｴｦﾀ昉ﾄﾊﾂﾇ・
{
	
	//1collisionOnlySetApart();
	
	//externalForces();
	//colliedMoveAlongVel();//5
	//collisionOnlySetApart();//4
	gravityStep();
	projectPositions();
	
	//collisionOnlySetApart();//3

	integrate();

	//2collisionOnlySetApart();

}
void DeformParticle::timeStep16BasedCluster(GRMesh *grMesh)//ﾃｿﾒｻｴﾎﾊｱｼ茣ｽｴｦﾀ昉ﾄﾊﾂﾇ・
{
	int count = 16;
	int indexes[16];

	gravityStep();
	
	int beginNum = 14;
	//Group 1 in bottom
	int i = 0;
		for(int j=0;j<8;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 15;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 11;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		projectPosCluster(indexes, count);

	//Group 2 overlapping with Group 1
	beginNum = 10;
	i = 0;
		for(int j=0;j<8;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 17;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 11;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		projectPosCluster(indexes, count);

	beginNum = 6;
	i = 0;
		for(int j=0;j<8;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 19;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 11;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		projectPosCluster(indexes, count);

		beginNum = 2;
	i = 0;
		for(int j=0;j<8;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 21;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 11;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		projectPosCluster(indexes, count);

		beginNum = 0;
		i = 0;
		for(int j=0;j<6;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 22;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		beginNum += 11;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		projectPosCluster(indexes, count);
	//projectPositions();

	integrate();
	//integratewithCluster();

}

void DeformParticle::timeStep8BasedCluster(GRMesh *grMesh)//ﾃｿﾒｻｴﾎﾊｱｼ茣ｽｴｦﾀ昉ﾄﾊﾂﾇ・
{
	int count = 8;
	int indexes[8];

	gravityStep();
	
	int beginNum = 0;
	
	
	int i = 0;
	int clusterNum = 0;
	for(int im =0;im<10;im++)
	{
		
		integretcluster(beginNum,indexes,clusterNum);
		projectPosCluster(indexes, count);
		beginNum +=2;
	}	
	//projectPositions();

	integrate();
	//integratewithCluster();

}

void DeformParticle::integretcluster(int beginNum,int indexes[],int &clusterNum)
{
	int subbeginNum1 = 0; 
	int subbeginNum2 = 0;
	int i = 0;
		for(int j=0;j<4;j++)
		{
			indexes[i] = beginNum + j;
			i++;
		}
		//subbeginNum1 = beginNum + 22;
		subbeginNum1 = clusterNum + 22;
		for(int j=0;j<2;j++)
		{
			indexes[i] = subbeginNum1 + j;
			i++;
		}
		subbeginNum2 = subbeginNum1 + 11;
		for(int j=0;j<2;j++)
		{
			indexes[i] = subbeginNum2 + j;
			i++;
		}
		clusterNum++;
}
//void DeformParticle::timeStepwithCluster(GRMesh *grMesh)//ﾃｿﾒｻｴﾎﾊｱｼ茣ｽｴｦﾀ昉ﾄﾊﾂﾇ・
//{
//	int count = grMesh->faces[0].nVertices;
//	int* indexes = new int[count];
//
//	gravityStep();
//	
//	
//	for(int i=0;i< grMesh->faces.size();i++)
//	{
//		for(int j=0;j<count;j++)
//		{
//			indexes[j] = grMesh->faces[i].indices[j];
//		}
//		//projectPosCluster(indexes, count);
//	}
//	projectPositions();
//
//	integrate();
//	//integratewithCluster();
//
//}

//---------------------------------------------------------------------------
void DeformParticle::fixVertex(int nr, const Vec3f &pos)//!!?ﾐﾞﾕｹｵﾄｵ羮ﾆﾋ・ｹﾌｶｨｵ罐ｿ//ﾊ・羹ﾏｶｯｵﾄｵ・
{
    mNewPos[nr] = pos;
    mFixed[nr] = true;
}


//---------------------------------------------------------------------------
void DeformParticle::releaseVertex(int nr)//ﾊﾍｷﾅﾐﾞﾕｵ・ｹﾌｶｨｵ罐ｿ
{
	mFixed[nr] = false;
}

//---------------------------------------------------------------------------


void DeformParticle::findTheBound(m2Real rB)
{
	float minX = 0.0,maxX = 0.0;
	float minY = 0.0,maxY = 0.0;
	int axis = 0;
	if(mNumVertices>0)
	{
		minX = mPos[0].x;
		maxX = minX;
		minY = mPos[0].y;
		maxY = maxY;
	}

	for(int i=0;i<mNumVertices;i++)
	{
		if(minX>mPos[i].x)
			minX = mPos[i].x;
		if(maxX<mPos[i].x)
			maxX = mPos[i].x;

		if(minY>mPos[i].y)
			minY = mPos[i].y;
		if(maxY<mPos[i].y)
			maxY = mPos[i].y;

		
	}
	/*if(minX<0) minX = 0.0;
	if(maxX<0) maxX = 0.0;
	if(minY<0) minY = 0.0;
	if(maxY<0) maxY = 0.0;*/

	params.collBound.bound[0].beP = minX-rB;
	params.collBound.bound[1].beP = minY-rB;
	params.collBound.bound[2].beP = maxX+rB;
	params.collBound.bound[3].beP = maxY+rB;
	
	//return BR;

	//ｲ衒・ﾅﾐ・
	//std::sort(mPosXVector.begin(),mPosXVector.end());
	//std::sort(mPosYVector.begin(),mPosYVector.end());

	//char s[20];
	//for(i=0;i<mPosXVector.size();i++)
	//{
		//std::cout<<mPosXVector.pop_back()<<std::endl;
		
			//sprintf(s, "sortedNum ", mPosXVector[i]);
	//}
}
void DeformParticle::deleteColliedPos(int posIndex)
{
	if(mColliedPosIndex.size()<=0)
		return;
	for(int i=0;i<mColliedPosIndex.size();i++)
	{
		if(posIndex==mColliedPosIndex[i])
			mColliedPosIndex.erase(mColliedPosIndex.begin()+i);
													
	}
	
}

void DeformParticle::addColliedPosIndex(int posIndex)
{
		
	for(int i=0;i<mColliedPosIndex.size();i++)
	{
		if(posIndex==mColliedPosIndex[i])
			return;

	}
mColliedPosIndex.push_back(posIndex);
}

void DeformParticle::deleteColliedPosInB(int posIndex)
{
	if(mColliedPosIndexInBound.size()<=0)
		return;
	for(int i=0;i<mColliedPosIndexInBound.size();i++)
	{
		if(posIndex==mColliedPosIndexInBound[i])
			mColliedPosIndexInBound.erase(mColliedPosIndexInBound.begin()+i);
													
	}
	
}

Matrix3f DeformParticle::MatrixesMultiply3f(Matrix3f &res, Matrix3f &other)
{
	Matrix3f result;
	result.xx = res.xx * other.xx + res.xy * other.yx + res.xz * other.zx;
	result.xy = res.xx * other.xy + res.xy * other.yy + res.xz * other.zy;
	result.xz = res.xx * other.xz + res.xy * other.yz + res.xz * other.zz;

	result.yx = res.yx * other.xx + res.yy * other.yx + res.yz * other.zx;
	result.yy = res.yx * other.xy + res.yy * other.yy + res.yz * other.zy;
	result.yz = res.yx * other.xz + res.yy * other.yz + res.yz * other.zz;

	result.zx = res.zx * other.xx + res.zy * other.yx + res.zz * other.zx;
	result.zy = res.zx * other.xy + res.zy * other.yy + res.zz * other.zy;
	result.zz = res.zx * other.xz + res.zy * other.yz + res.zz * other.zz;
	
	return result;
}
Vec3f DeformParticle::MatrixMultiVec3f(Matrix3f &res, Vec3f &other)
{
	Vec3f result;
	result.x = res.xx * other.x + res.xy * other.y + res.xz * other.z;
	result.y = res.yx * other.x + res.yy * other.y + res.yz * other.z;
	result.z = res.zx * other.x + res.zy * other.y + res.zz * other.z;
	
	
	/*result.x = res.xx * other.x + res.yx * other.y + res.zx * other.z;
	result.y = res.xy * other.x + res.yy * other.y + res.zy * other.z;
	result.z = res.xz * other.x + res.yz * other.y + res.zz * other.z;
	*/
	//res = result;
	return result;
}
