#ifndef CDSPATIALHASHTABLE_H
#define CDSPATIALHASHTABLE_H

#include <Foundation/Object.h>
#include <Physics\PHOpObj.h>

namespace Spr{;

//class Bounds
//		{
//		public:
//			m3Bounds(){}
//			inline m3Bounds(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }
//
//			inline void set(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }
//
//			void clamp(Vec3f &pos){//境界を超えた点をそのまま境界のどころ設定する
//				if (isEmpty()) return;
//				//pos.maximum(min);
//				maximum(pos, min);
//				minimum(pos, max);
//			};
//			inline bool isEmpty() const {
//				if (min.x > max.x&&min.y > max.y&&min.z > max.z) return true;
//				return false;
//			}
//			inline void minimum(Vec3f &self, Vec3f &other) {
//				if (other.x < self.x) self.x = other.x;
//				if (other.y < self.y) self.y = other.y;
//				if (other.z < self.z) self.z = other.z;
//
//			}
//			inline void maximum(Vec3f &self, Vec3f &other) {
//				if (other.x > self.x) self.x = other.x;
//				if (other.y > self.y) self.y = other.y;
//				if (other.z > self.z) self.z = other.z;
//			}
//			Vec3f min, max;
//		};



struct PtclInfoNode{
			int ptclIndex;
			int objIndex;
			int timeStamp;
			PtclInfoNode()
			{
				ptclIndex	=	-1;
				objIndex	=	-1;
				timeStamp	=	-1;
			}
			PtclInfoNode(int &pInd,	int &objInd,	int &timestamp)
			{
				ptclIndex	=	pInd;
				objIndex	=	objInd;
				timeStamp	=	timestamp;
			}
	};

class PtclInfoList{
public:


	std::vector<PtclInfoNode> pcInfolist;
		std::vector<PtclInfoNode> ::iterator itr;
		//std::vector<int> includedObjInd;
		int objNum;
		int* includedObjInd; 
		int IndexOfincludedObjInd;
		int collilength;

		int reserveListSize;

		
	PtclInfoList(){
		reserveListSize = 100;
		pcInfolist.reserve(reserveListSize);
		collilength = 0;
		IndexOfincludedObjInd = 0;
		objNum = 10;//be carefull the 10 here means U can only support 10 objs collided in the same time
		includedObjInd = new int[objNum]();
	}
	
		
		

		//pointをHashCellにaddしてみます、他のobjのpointも同じhashにいるなら、コリジョン発生する。
		bool addNewP(int pIndex,int oIndex,int timestamp,int hashIndex,int axis)
		{
			int hashId = hashIndex;
			int count = 0;
			IndexOfincludedObjInd= (-1) ;//
			if(pcInfolist.size()>0)
			{
				int testNum = (int) pcInfolist.size();
				std::vector<PtclInfoNode> ::iterator crackendtest = pcInfolist.end();
				itr = pcInfolist.begin();

				//Check points that already added 
				//for(itr= pcInfolist.begin(); itr != crackendtest; itr++) 
				for(int j =0;j<testNum;j++)
				{
					//PtclInfoNode &piNode = *itr;
					PtclInfoNode &piNode = *itr;
					itr++;
					
					//timestamp is old, than just add new one inside
					if(piNode.timeStamp!=timestamp)
					{//case of [past][*] or [*][past]
						if(itr==pcInfolist.begin())
						{//case of [past][add]
							//if(pcInfolist.size()!=1)
								swapList();
							PtclInfoNode newNode(pIndex,oIndex,timestamp);

							pcInfolist.push_back(newNode);
							//list<PtclInfoNode> ::iterator crackendtest2 = pcInfolist.end();
							collilength = (int) pcInfolist.size();
							return false;		//exit 1
						}
						else
						{//case of [*][past]
							piNode.timeStamp = timestamp;
							piNode.ptclIndex = pIndex;
							piNode.objIndex = oIndex;
							collilength = count+1;
							
							return true;	//exit 2
						}
						
						
					}
					else if((piNode.ptclIndex==pIndex)&&(piNode.objIndex==oIndex))
							return false;//why it happen. case of [self][]	//exit 3
					else 
						{//case of [..][other][..]
							
							count++;
							continue;
					}
					
				}
			

				//case of [..][other][add]
				PtclInfoNode newNode(pIndex,oIndex,timestamp);
				pcInfolist.push_back(newNode);
				
				collilength = (int) pcInfolist.size();
				return true;
			}
			else 
			{//case of [add]
				PtclInfoNode newNode(pIndex,oIndex,timestamp);
				pcInfolist.push_back(newNode);

				collilength = 0;
				
				return false;
			}
			
		}
		void swapList()
		{
			
			pcInfolist.clear();
			std::vector<PtclInfoNode> s;
			s.swap(pcInfolist);

		
		}
		
	};


class CDSpatialHashTable: public Object
{
	SPR_OBJECTDEF(CDSpatialHashTable);


	public:
	
		std::vector<PtclInfoList> ptclColliList;
		

	CDSpatialHashTable(){
		cellsize = -1;
		reserveSize = 100;
			}

	
	float cellsize;
	
	float distX;
	float distY;
	float distZ;
	int hashIndex;//hashInx,hashIny,hashInz;
	std::vector<int> ColliedHashIndexList;
	//speedup,hash numbers for one particle. prevent redundant check
	std::vector<int> currHashs;
	
	
	//params
	int reserveSize ;
	int hashtableSize;
	float posCoefft; //
	float cellCoefft;

	void init(float cellSize,Bounds  bounds)
	{//hash表、空間の構築
		currHashs.reserve(reserveSize);
		//params
		hashtableSize = 809;
		//posCoefft = 2.0;
		//cellCoefft = 1.0;
		posCoefft = 1.0;
		cellCoefft = 1.0;//0.3;

		cellsize = cellSize;
		distX = fabs(bounds.min.x - bounds.max.x);
		distY = fabs(bounds.min.y - bounds.max.y);
		distZ = fabs(bounds.min.z - bounds.max.z);
	
		for(int i =0;i<hashtableSize;i++)
		{
			PtclInfoList pNode;
			ptclColliList.push_back(pNode);
		}
		

		/*propX = fabs(cellsize/distX);
		propY = fabs(cellsize/distY);
		propZ = fabs(cellsize/distZ);*/
	}

	int  hash(Vec3i pos,int hashsize)
	{//hash関数
		//float p1= 73856093,p2 = 19349663,p3 = 83492791;
		int p1= 73856099,p2 = 1934966,p3 = 89279;
		int xpp1= pos.x *p1;
		int ypp2= pos.y *p2;
		int zpp3= pos.z *p3;
		xpp1 = xpp1^ypp2;
		xpp1 = xpp1^zpp3;
		return abs((xpp1)%hashsize);
	}
	bool ReducedSelectPointsToAdd(Vec3f &pCtr, float radius, int objInd, int pInd, int timestamp)
	{//1 選んだ点をHashTableに積み込む

		

		//float hashIndex = hash(pCtr,1000);
		std::vector<int> tmphashlist;
		if (ptclColliList.size() == 0)
		{
			std::cout << "please initalize sphashtable" << std::endl;
			return false;

		}
		//if(spTableX.hashtable.size()==0)
		//	return false;
		//Vec3f tmp = pCtr;//,front,top,left;
		//Vec3f ftl,ftr,btl,btr,fbl,fbr,bbl,bbr;
		bool collied = false;

		float tx = pCtr.x;// + distX/2;
		float ty = pCtr.y;// + distY/2;
		float tz = pCtr.z;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, -1))
		{
			//addColliPairs(hashIndex);
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}

		tx = pCtr.x + radius;// + distX/2;
		ty = pCtr.y + radius;// + distY/2;
		tz = pCtr.z + radius;// + distZ/2;
		//if(addTothreeCoord(tx,ty,tz,objInd,pInd,timestamp));
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 0))
		{
			//addColliPairs(hashIndex);
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x - radius;// + distX/2;
		ty = pCtr.y - radius;// + distY/2;
		tz = pCtr.z - radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 1))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x + radius;// + distX/2;
		ty = pCtr.y - radius;// + distY/2;
		tz = pCtr.z - radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 2))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x - radius;// + distX/2;
		ty = pCtr.y + radius;// + distY/2;
		tz = pCtr.z - radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 3))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x - radius;// + distX/2;
		ty = pCtr.y - radius;// + distY/2;
		tz = pCtr.z + radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 4))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x - radius;// + distX/2;
		ty = pCtr.y + radius;// + distY/2;
		tz = pCtr.z + radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 5))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x + radius;// + distX/2;
		ty = pCtr.y - radius;// + distY/2;
		tz = pCtr.z + radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 6))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}
		tx = pCtr.x + radius;// + distX/2;
		ty = pCtr.y + radius;// + distY/2;
		tz = pCtr.z - radius;// + distZ/2;
		if (addTohashtable(tx, ty, tz, objInd, pInd, timestamp, 7))
		{
			ColliedHashIndexList.push_back(hashIndex);
			collied = true;
		}

		//std::vector<int> sw;
		//sw.swap(currHashs);
		currHashs.clear();

		return collied;
	}
	
private: 	bool addTohashtable(float tx,float ty,float tz,int objInd,int pInd,int timestamp,int boxId)
	{//hashIndex is calculated here and already updated to be used in addColliPairs
		//Hashtableに積み込む

		/*int txx = tx/cellsize;
		int tyy = ty/cellsize;
		int tzz = tz/cellsize;*/

		//float is too approximated here txx is a cut to classify nearby pos
		//is it work? If use float then many will be hashed to same cell
		int txx = (int)(tx * posCoefft)/(cellsize * cellCoefft);
		int tyy = (int)(ty * posCoefft)/(cellsize * cellCoefft);
		int tzz = (int)(tz * posCoefft)/(cellsize * cellCoefft);

		Vec3i &tmp = Vec3i(txx,tyy,tzz);
		hashIndex = hash(tmp,hashtableSize);

		//redundant check
		for (int ri = 0; ri <(int) currHashs.size(); ri++)
		{
			if (currHashs[ri] == hashIndex)
			{
				return false;
			}
		}
		currHashs.push_back(hashIndex);

		//if(hashIndex==1648)mygod400hash.push_back(tmp);//result both 13,11,12 and 11,11,10 and...is hashed to 400

		//このpointをaddしてみます、他のobjのpointも同じhashにいるなら、コリジョン発生する。
		return ptclColliList[hashIndex].addNewP(pInd,objInd,timestamp,hashIndex,0);
	}

	int fourThrowFiveHold(float numb)
	{
		if(numb>0.0)
		return int(numb + 0.5);
		else if(numb<0.0)
		{
			return int(-(numb - 0.5));
		}
		else return 0;
	}
	
	
};

}//	namespace Spr

#endif