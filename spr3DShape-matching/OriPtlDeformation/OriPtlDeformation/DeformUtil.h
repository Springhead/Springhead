//---------------------------------------------------------------------------

#ifndef DEFORMUTILH
#define DEFORMUTILH
//---------------------------------------------------------------------------

#include <vector>
#include <Springhead.h>
#include "m2Real.h"
#include "m3Bounds.h"
#include "DeformParticle.h"
#include <../src/Graphics/GRMesh.h>

class DeformUtil
{
public :
	bool isLinkedToDp;
	DeformUtil();
	void InitialDU();

	void BuildParticles(GRMesh &meshObj, DeformParticle &dp, int strategy);

	void DeleteParticles();

private :
			GRMesh &meshObj;
			DeformParticle &mDp;
			void collisionInitial();
			
			int stra_CONCENTRATED;
			int stra_FRIABLE;
			float pRadius;
			std::vector<int> parIndex;
			inline int findNeighborPs(Vec3f objP, int curindex)
			{

				if(isLinkedToDp)
				{
					//float distance = mDp.mPos[curindex].x
				}
				return 0;
			}
};


#endif