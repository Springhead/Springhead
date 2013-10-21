#include <Springhead.h>
#include <..\src\Graphics\GRMesh.h>
#include "DeformParticle.h"
#include "OPApp.h"
#include "Math3d\m9Matrix.h"
#include"PolarDecomposition.h"


#define USE_DISPLAY = false;
#define tempPosDebug = false;
#define ARROW_KEY
#define BAR_CTRL
//#define DEFORM_TIME ;

using namespace Spr;
using namespace std;

void OPApp::CalGRObjNormal(GRMesh& objMesh)
{
	Vec3f* srcPos = objMesh.GetVertices();
	Vec3f tempPos = Vec3f(0,0,0);
	for(int i = 0;i < objMesh.NVertex();i++)
	{
		tempPos += srcPos[i];
	}
	//ostream ost = ost new ostream();
	//tempPos.print();
}
void  OPApp::Keyboard(int key, int x, int y)
		{
			bool tmpb = GetSdk()->GetDebugMode();
			Vec3f* tempPos = grMeshBox->GetVertices();
			Vec3f tmp1 = tempPos[0];
				Vec3f tmp2 = tempPos[1];
				int size =tempPos->size();
			float alpha = -1.0,beta = -1.0,gama = -1.0;

			Vec3f originalCm,cm;
			Matrix3f Apq, Aqq;
			Matrix3f R,S;
			polarDecomposition pd;
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

			m2Real m;

			//Vec3d pos = boxPhIf->GetCenterPosition();
			cout<<key<<endl;
			switch (key)
			{
			case ESC:
			case 'q':
				dP.params.quadraticMatch = !dP.params.quadraticMatch;
					dP1.params.quadraticMatch = !dP1.params.quadraticMatch;
				break;
#ifdef ARROW_KEY
			case 357://上
				tempPos[3].x += 0;
				tempPos[3].y += 1;
				tempPos[3].z += 0;
				break;
			case 359://下
				
				tempPos[2].x += 0;
				tempPos[2].y += -1;
				tempPos[2].z += 0;
				break;
			case 356://左
				
				tempPos[3].x += -1;
				tempPos[3].y += 0;
				tempPos[3].z += 0;
				break;
			case 358://右
				
				tempPos[4].x += 1;
				tempPos[4].y += 0;
				tempPos[4].z += 0;
				break;
#endif
			case 'i':
				tempPos = grMeshBar->GetVertices();
				tempPos[0].x = 0;
				tempPos[0].y = 2;
				tempPos[0].z = 0;
				break;
			case 'k':
				tempPos = grMeshBar->GetVertices();
				tempPos[2].x = 0;
				tempPos[2].y = -2;
				tempPos[2].z = 0;
				break;
			case 'j':
				tempPos = grMeshBar->GetVertices();
				tempPos[3].x = -2;
				tempPos[3].y = 0;
				tempPos[3].z = 0;
				break;
			case 'l':
				tempPos = grMeshBar->GetVertices();
				tempPos[4].x = 2;
				tempPos[4].y = 0;
				tempPos[4].z = 0;
				break;
			case '8':
				dP.params.allowFlip = !dP.params.allowFlip;
				dP1.params.allowFlip = !dP1.params.allowFlip;
				break;
			case 'd':
				tmpb = !tmpb;
				GetSdk()->SetDebugMode(tmpb);
				break;
			case 'z':
				dP.debugStepOn = !dP.debugStepOn;
				dP1.debugStepOn = !dP1.debugStepOn;
				break;
			case 'a':
				
				tempPos[0].x = 0;
				tempPos[0].y = 3;
				tempPos[0].z = 0;
				if(!dP.mFixed[0])
				{
					dP.fixVertex(0,tempPos[0]);
					std::cout<<"Vertexfixed!"<<std::endl;
				}
				else dP.releaseVertex(0);
				break;
			case 'x':
				tempPos = grMeshBar->GetVertices();
				tempPos[25].x = 0;
				tempPos[25].y = 3;
				tempPos[25].z = 0;
				if(!dP1.mFixed[25])
				{
					dP1.fixVertex(25,tempPos[25]);
					std::cout<<"Vertexfixed!"<<std::endl;
				}
				else dP1.releaseVertex(25);
				break;
			case 'r':
				reset = true;
				//Clear();
				////GetSdk()->Clear();
				//loadMyScene();
				Init();
				dP.reset();
				break;
			case 's':
				if(!dP.debugStepOn)
					return;
				inputPosition(grMeshBox,dP);
				dP.timeStep();
				//dP.timeStepwithCluster(grMeshBox);
			
				UpdatePosition(grMeshBox,dP);//stepで結果を計算と更新
				inputPosition(grMeshBar,dP1);
				if(cluster8Base)
				dP1.timeStep8BasedCluster(grMeshBar);
				else dP1.timeStep16BasedCluster(grMeshBar);
				UpdatePosition(grMeshBar,dP1);
				break;
			case '1':
				dP.params.alpha = 0.02;
				break;
			case '2':
				dP.params.alpha = 0.05;
				break;
				case '3':
				dP.params.beta = 0.001;
				break;
				case '4':
				dP.params.beta = 0.002;
				break;
				case '5':
				dP.params.beta = 0.003;
				break;
				
				
				case '0':
					
					break;
			case 360:
				gama = dP1.params.gama;
				if(gama >= 1)break;
				if(gama == 0.0) gama = 0.001;
				gama*=2;
				cout<<"bar gama = "<<gama<<endl;
				dP1.params.gama = gama;
				break;
			case 361:
					gama = dP1.params.gama;
				if(gama <= 0.0001)break;
				gama/=1.5;
				cout<<"bar gama = "<<gama<<endl;
				dP1.params.gama = gama;
					break;
			case 'e':
				cluster8Base = !cluster8Base;
				break ;
#ifdef BAR_CTRL
			case '+':
				/*gama = dP.params.gama;
				if(gama >= 1)break;
				gama*=2;
				cout<<"box gama = "<<gama<<endl;
				dP.params.gama = gama;*/
				/*alpha = dP.params.alpha;
				if(alpha >= 1)break;
				alpha*=2;
				cout<<"box alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;*/
				beta = dP1.params.beta;
				if(beta== 0 )beta = 0.01;
				
				beta*=2;
				if(beta >= 1)beta = 1;
				cout<<"bar beta = "<<beta<<endl;
				dP1.params.beta = beta;
				break;
			case '-':
				/*gama = dP.params.gama;
				if(gama <= 0.0001)break;
				gama/=1.5;
				cout<<"box gama = "<<gama<<endl;
				dP.params.gama = gama;*/
				/*alpha = dP.params.alpha;
				if(alpha <= 0.0001)break;
				alpha/=1.5;
				cout<<"box alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;*/
				beta = dP1.params.beta;
				if(beta <= 0.0001)break;
				beta/=1.5;
				cout<<"bar beta = "<<beta<<endl;
				dP1.params.beta = beta;
				break;
			case '*':
				alpha = dP1.params.alpha;
				if(alpha<=0)alpha=0.01;
				
				alpha*=2;
				if(alpha >= 1)alpha = 1;
				cout<<"bar alpha = "<<alpha<<endl;
				dP1.params.alpha = alpha;
				break;
			case '/':
				alpha = dP1.params.alpha;
				if(alpha <= 0.0001)alpha =0;
				alpha/=2;
				cout<<"bar alpha = "<<alpha<<endl;
				dP1.params.alpha = alpha;
				break;
#else
				case '+':
				/*gama = dP.params.gama;
				if(gama >= 1)break;
				gama*=2;
				cout<<"box gama = "<<gama<<endl;
				dP.params.gama = gama;*/
				/*alpha = dP.params.alpha;
				if(alpha >= 1)break;
				alpha*=2;
				cout<<"box alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;*/
				beta = dP.params.beta;
				if(beta== 0 )beta = 0.01;
				
				beta*=2;
				if(beta >= 1)beta = 1;
				cout<<"bar beta = "<<beta<<endl;
				dP.params.beta = beta;
				break;
			case '-':
				/*gama = dP.params.gama;
				if(gama <= 0.0001)break;
				gama/=1.5;
				cout<<"box gama = "<<gama<<endl;
				dP.params.gama = gama;*/
				/*alpha = dP.params.alpha;
				if(alpha <= 0.0001)break;
				alpha/=1.5;
				cout<<"box alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;*/
				beta = dP.params.beta;
				if(beta <= 0.0001)break;
				beta/=1.5;
				cout<<"bar beta = "<<beta<<endl;
				dP.params.beta = beta;
				break;
			case '*':
				alpha = dP.params.alpha;
				if(alpha<=0)alpha=0.01;
				
				alpha*=2;
				if(alpha >= 1)alpha = 1;
				cout<<"bar alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;
				break;
			case '/':
				alpha = dP.params.alpha;
				if(alpha <= 0.0001)alpha =0;
				alpha/=2;
				cout<<"bar alpha = "<<alpha<<endl;
				dP.params.alpha = alpha;
				break;
#endif
			case 'v':
				dP1.params.volumeConservation = !dP1.params.volumeConservation;
				break;
			//case '7'://翻转实验
			//	
			//	while(1)
			//	{
			//		tempPos[1] = tempPos[2];
			//		tempPos[0] = tempPos[7];
			//		tempPos[7] = tempPos[4];
			//		tempPos[2] = tempPos[5];
			//		tempPos[4] = tempPos[3];
			//		tempPos[5] = tempPos[6];
			//		tempPos[3] = tmp1;
			//		tempPos[6] = tmp2;
			//	break;
			//	}
			//	break;
			case '9'://扭曲实验
				for(int i=3;i<7;i++)
				{
					dP.mFixed[i] = true;
					tempPos[6-i] = tempPos[5-i];
				}
				dP.mFixed[7] = true;
				//tempPos[6-i] = tempPos[5-i];
				//tempPos[7] = tempPos[6];

				//tempPos[0].x = 0;
				//tempPos[0].y = 3;
				//tempPos[0].z = 0;
				break;
			case 'f'://固定把转实验
				tempPos = grMeshBar->GetVertices();
				//size =grMeshBar->NVertex();
				if(dP1.mFixed[topindexoftheBar[0]] == false)
				{
					for(int i=0;i<4;i++)
					{
						/*tempPos[topindexoftheBar[i]].x = 0.0;
						tempPos[topindexoftheBar[i]].y = -3.0;
						tempPos[topindexoftheBar[i]].z = 0.0;*/

						//dP1.mMasses[topindexoftheBar[i]] = 10;
						dP1.fixVertex(topindexoftheBar[i],tempPos[topindexoftheBar[i]]);
						std::cout<<"Vertexfixed!"<<std::endl;
					}
				}
				else
				{
					for(int i=0;i<4;i++)
					{
						
						dP1.mMasses[topindexoftheBar[i]] = 0.1;
						dP1.releaseVertex(topindexoftheBar[i]);
						std::cout<<"Released!"<<std::endl;
					}
				}
				break;
			case '6':
				
				break;
			case 'p'://打印q
				
				originalCm.Zero();
				for(int i=0;i<dP.mPos.size();i++)
				{
					originalCm += dP.mOriginalPos[i] ;
				}
				
				for(int i=0;i<dP.mPos.size();i++)
				{
					q = dP.mOriginalPos[i] - originalCm;
					cout<<"q = "<<q.X()<<" "<<q.Y()<<" "<<q.Z()<<endl;
				}
				
				break;
			case 'o'://打印Apq
				originalCm.Zero();
				for(int i=0;i<dP.mPos.size();i++)
				{
					m = dP.mMasses[i];
					cm += dP.mNewPos[i] * m;
					originalCm += dP.mOriginalPos[i] ;
				}

				for (int i = 0; i < dP.mPos.size(); i++) {
					m = dP.mMasses[i];
					p = dP.mNewPos[i] - cm;
					q = dP.mOriginalPos[i] - originalCm;
					
					Apq.ExX() += m * p.x * q.x;
					Apq.ExY() += m * p.x * q.y;
					Apq.ExZ() += m * p.x * q.z; 
					Apq.EyX() += m * p.y * q.x;
					Apq.EyY() += m * p.y * q.y;
					Apq.EyZ() += m * p.y * q.z; 
					Apq.EzX() += m * p.z * q.x; 
					Apq.EzY() += m * p.z * q.y;
					Apq.EzZ() += m * p.z * q.z;
				}
				cout.setf(ios::fixed);
				cout.precision(3);
				
				//pd.NewPolarDecomposition(Apq,R);
				
				/*	cout<<"box Apq "<<endl;
					cout<<Apq.ExX()<<" "<<ceil(Apq.ExY())<<" "<<ceil(Apq.ExZ())<<endl;
					cout<<Apq.EyX()<<" "<<ceil(Apq.EyY())<<" "<<ceil(Apq.EyZ())<<endl;
					cout<<Apq.EzX()<<" "<<ceil(Apq.EzY())<<" "<<ceil(Apq.EzZ())<<endl;
					cout<<"R "<<endl;
					cout<<R.ExX()<<" "<<R.ExY())<<" "<<R.ExZ())<<endl;
					cout<<R.EyX()<<" "<<R.EyY())<<" "<<R.EyZ())<<endl;
					cout<<R.EzX()<<" "<<R.EzY())<<" "<<R.EzZ())<<endl;
					cout<<endl;*/
				R.Zero();
				R.xx = 0;
				R.yy = 0;
				R.zz = 0;
				Apq.Zero();
				Apq.xx = 0;
				Apq.yy = 0;
				Apq.zz = 0;
				for (int i = 0; i < dP1.mPos.size(); i++) {
					m = dP1.mMasses[i];
					p = dP1.mNewPos[i] - cm;
					q = dP1.mOriginalPos[i] - originalCm;
					
					Apq.ExX() += m * p.x * q.x;
					Apq.ExY() += m * p.x * q.y;
					Apq.ExZ() += m * p.x * q.z; 
					Apq.EyX() += m * p.y * q.x;
					Apq.EyY() += m * p.y * q.y;
					Apq.EyZ() += m * p.y * q.z; 
					Apq.EzX() += m * p.z * q.x; 
					Apq.EzY() += m * p.z * q.y;
					Apq.EzZ() += m * p.z * q.z;
				}
				
				pd.NewPolarDecomposition(Apq,R);
				cout<<"bar Apq "<<endl;
				cout<<Apq.ExX()<<" "<<ceil(Apq.ExY())<<" "<<ceil(Apq.ExZ())<<endl;
				cout<<Apq.EyX()<<" "<<ceil(Apq.EyY())<<" "<<ceil(Apq.EyZ())<<endl;
				cout<<Apq.EzX()<<" "<<ceil(Apq.EzY())<<" "<<ceil(Apq.EzZ())<<endl;
				cout<<"R "<<endl;
				cout<<R.ExX()<<" "<<R.ExY()<<" "<<R.ExZ()<<endl;
				cout<<R.EyX()<<" "<<R.EyY()<<" "<<R.EyZ()<<endl;
				cout<<R.EzX()<<" "<<R.EzY()<<" "<<R.EzZ()<<endl;
					
				break;
			case'@':

				break;
			default:

				break;
			}

			
		}


int OPApp::ceilOrFloor(float f)
	{
		/*if(f>0.5)
		{
			return ceil(f);
		}
		else {
			return floor(f);
		}*/
		return 0;
	}