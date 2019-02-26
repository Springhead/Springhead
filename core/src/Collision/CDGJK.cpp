/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*	このファイルのライセンスについての注意
	このソースは，一部 SOLID (Software Library for Interference Detection) 2.0
	http://www.win.tue.nl/~gino/solid の src/Convex.cpp を参考に書いています．
	論文とソースを公開されている Bergen さんに感謝します．

	長谷川は，このソースがSolid2.0の一部の派生物ではないと信じています．
	しかし，似ている箇所があります．もし派生物だと認定された場合，
	ライセンスがLGPLとなります．ご注意ください．
*/
#include <Foundation/UTQPTimer.h>
#include <Collision/CDBox.h>
#include <Collision/CDSphere.h>
#include <Collision/CDCapsule.h>
#include <Collision/CDRoundCone.h>
#include <Collision/CDConvexMesh.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHEngine.h>
#include <fstream>
#include <iomanip>

#include <string.h>  // strcmp


namespace Spr{
bool bGJKDebug;

void SaveMaterial(std::ostream& file, PHMaterial& m){
	file << m.e << " ";
	file << m.mu << " " << m.mu0 << std::endl;
}
void LoadMaterial(std::istream& file, PHMaterial& m){
	file >> m.e;
	file >> m.mu >> m.mu0;
}
void SaveShape(std::ostream& file, CDShape* a){
	CDConvexMesh* mesh = a->Cast();
	if (mesh){
		file << "mesh" << std::endl;
		CDConvexMeshDesc desc;
		mesh->GetDesc(&desc);
		SaveMaterial(file, desc.material);
		file << desc.vertices.size() << " ";
		for(unsigned i=0; i<desc.vertices.size(); ++i){
			file << desc.vertices[i] << std::endl;
		}
	}
	CDBox* box = a->Cast();
	if (box){
		file << "box" << std::endl;
		CDBoxDesc desc;
		box->GetDesc(&desc);
		SaveMaterial(file, desc.material);
		file << desc.boxsize << std::endl;
	}		
	CDCapsule* cap = a->Cast();
	if (cap){
		file << "cap" << std::endl;
		CDCapsuleDesc desc;
		cap->GetDesc(&desc);
		SaveMaterial(file, desc.material);
		file << desc.radius << std::endl;
		file << desc.length << std::endl;
	}		
	CDSphere* sph = a->Cast();
	if (sph){
		file << "sph" << std::endl;
		CDSphereDesc desc;
		sph->GetDesc(&desc);
		SaveMaterial(file, desc.material);
		file << desc.radius << std::endl;
	}
	CDRoundCone* rc = a->Cast();
	if(rc){
		file << "rc" << std::endl;
		CDRoundConeDesc desc;
		rc->GetDesc(&desc);
		SaveMaterial(file, desc.material);
		file << desc.radius << std::endl;
		file << desc.length << std::endl;
	}
}
CDConvex* LoadShape(std::istream& file, PHSdkIf* sdk){
	CDConvex* rv = NULL;
	char type[100];
	file >> type;
	if( strcmp(type, "mesh") == 0 ){
		CDConvexMeshDesc desc;
		LoadMaterial(file, desc.material);			
		int size;
		file >> size;
		desc.vertices.resize(size);
		for(int i=0; i<size; ++i){
			file >> desc.vertices[i];
		}
		rv = sdk->CreateShape(CDConvexMeshIf::GetIfInfoStatic(), desc)->Cast();
	}
	if( strcmp(type, "box") == 0){
		CDBoxDesc desc;
		LoadMaterial(file, desc.material);			
		file >> desc.boxsize;
		rv = sdk->CreateShape(CDBoxIf::GetIfInfoStatic(), desc)->Cast();
	}
	if( strcmp(type, "cap") == 0){
		CDCapsuleDesc desc;
		LoadMaterial(file, desc.material);			
		file >> desc.radius;
		file >> desc.length;
		rv = sdk->CreateShape(CDCapsuleIf::GetIfInfoStatic(), desc)->Cast();
	}
	if( strcmp(type, "sph") == 0){
		CDSphereDesc desc;
		LoadMaterial(file, desc.material);			
		file >> desc.radius;
		rv = sdk->CreateShape(CDSphereIf::GetIfInfoStatic(), desc)->Cast();
	}
	if( strcmp(type, "rc") == 0){
		CDRoundConeDesc desc;
		LoadMaterial(file, desc.material);
		file >> desc.radius;
		file >> desc.length;
		rv = sdk->CreateShape(CDRoundConeIf::GetIfInfoStatic(), desc)->Cast();
	}
	return rv;
}

template <class T>
void LoadP(std::istream& is, T& t) {
	unsigned char* p = (unsigned char*)&t;
	unsigned int d;
	for (int i = 0; i < sizeof(t); ++i) {
		is >> std::setbase(16) >> d;
		p[i]=d;
	}
}
template <class T>
void SaveP(std::ostream& os, const T& t) {
	const unsigned char* p = (const unsigned char*)&t;
	for (int i = 0; i < sizeof(t); ++i) {
		os << " " << std::setbase(16) << (unsigned int)p[i];
	}
	os << std::endl;
}

/**	ConfFindPointのデバッグのため、引数をファイルに保存する関数。ループを抜けないなどの場合にパラメータを保存する。
	doubleを10進に変換すると再現しないので、バイナリを１６進数で保存するように変更。
	ShapeDescは小数点のままなので、ShapeDescに10進数で桁数が多くなる数を書くと再現しない。	2018.08.16
*/
void FASTCALL ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist, const char* exName){
	std::string base = std::string("ContFindCommonPointSaveParam") + exName;
	std::ofstream file;
	for (int i = 0;; ++i) {
		std::string fname = base + std::to_string(i) + ".txt";
		std::ifstream ifs(fname);
		if (!ifs.is_open()) {
			file.open(fname);
			break;
		}
	}
	SaveShape(file, (CDConvex*)a);
	SaveShape(file, (CDConvex*)b);
	SaveP(file, a2w);
	SaveP(file, b2w);
	SaveP(file, dir);
	SaveP(file, start);
	SaveP(file, end);
	SaveP(file, normal);
	SaveP(file, pa);
	SaveP(file, pb);
	SaveP(file, dist);
}
void ContFindCommonPointCall(std::istream& file, PHSdkIf* sdk){
	bGJKDebug = true;
	const CDConvex* a;
	const CDConvex* b;
	Posed a2w, b2w;
	Vec3d dir, normal, pa, pb;
	double dist, start, end;
	a = LoadShape(file, sdk);
	b = LoadShape(file, sdk);
	LoadP(file, a2w);
	LoadP(file, b2w);
	LoadP(file, dir);
	LoadP(file, start);
	LoadP(file, end);
	LoadP(file, normal);
	LoadP(file, pa);
	LoadP(file, pb);
	LoadP(file, dist);

	Vec3f capdir = b2w.Ori() * Vec3f(0,0,1);
	DSTR << "dir of capsule = " << capdir << std::endl;
	DSTR << "center of capsule = " << b2w.Pos()  << std::endl;
	ContFindCommonPoint(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist);
	DSTR << "normal = " << normal << std::endl;
	DSTR << "pa and pb in W = "<< std::endl;
	DSTR << a2w * pa << std::endl;
	DSTR << b2w * pb << std::endl;
}
extern UTLongLong&	coltimePhase1;
extern UTLongLong&	coltimePhase2;
extern UTLongLong&	coltimePhase3;
extern int			colcounter;
extern UTQPTimer	qpTimerForCollision;

double biasParam = 0.5;

double epsilon = 1e-12; 
double threshold = 1e-12;

double epsilon2 = epsilon*epsilon;
double threshold2 = threshold*threshold;
void setGjkThreshold(double th, double e) {
	epsilon = e;
	threshold = th;
	epsilon2 = epsilon*epsilon;
	threshold2 = threshold*threshold;
}

namespace GJK{
	Vec3f p[4];			///<	Aのサポートポイント(ローカル系)
	Vec3f q[4];			///<	Bのサポートポイント(ローカル系)
	int p_id[4];		///<	Aのサポートポイントの頂点番号（球など頂点がない場合は -1）
	int q_id[4];		///<	Bのサポートポイントの頂点番号（球など頂点がない場合は -1）
	Vec3d w[4];			///<	ContFindCommonPointで使用する速度向き座標系でのサポートポイント
	Vec3d v[4];			///<	ContFindCommonPointで使用するv
	int nSupport;		///<	何点のsupportから最近傍点を計算したかを表す。(ContFindCommonPoint専用)
	Vec3d dec;			///<	内分の割合
	int ids[4];			///<	頂点ID対応表

	Vec3d p_q[4];		///<	ミンコスキー和上でのサポートポイント(ワールド系) ContでないFindXXで使用
}	//	namespace GJK
using namespace GJK;

const char vacants[] = {
	0, 1, 0, 2, 0, 1, 0, 3,
	0, 1, 0, 2, 0, 1, 0, 4,
};
inline char FindVacantId(char a, char b){
	char bits = (1<<a) | (1<<b);
	return vacants[(int)bits];
}
inline char FindVacantId(char a, char b, char c){
	char bits = (1<<a) | (1<<b) | (1<<c);
	return vacants[(int)bits];
}

inline Vec3d TriDecompose(Vec2d p1, Vec2d p2, Vec2d p3){
	double det = p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y);
	Vec3d r;
	if (abs(det) > epsilon){
		double detInv = 1/det;
		r.x = detInv * (p2.x*p3.y-p3.x*p2.y);
		r.y = detInv * (p3.x*p1.y-p1.x*p3.y);
		r.z = 1 - r.x - r.y;
	}else{
		double ip[3] = {p1*p2, p2*p3, p3*p1};
		int j=-1;
		for(int i=0; i<3; ++i){
			if (ip[i]<=0 && ip[(i+1)%3]<=0){
				if (ip[i] > ip[(i+1)%3]) j = i;
				else j = (i+1)%3;
				break;
			}
		}
		if(j < 0){
			if (ip[0] < ip[1] && ip[0] < ip[2]) j=0;
			else if (ip[1] < ip[2]) j=1;
			else j = 2;
		}
		if (j==0){
			double a = p1*p1; double b = -p1*p2;
			if (a+b == 0) a = b = 1;
			r.x = b/(a+b);
			r.y = a/(a+b);
		}else if (j==1){
			double a = p2*p2; double b = -p2*p3;
			if (a+b == 0) a = b = 1;
			r.y = b/(a+b);
			r.z = a/(a+b);
		}else{	//	j==2
			double a = p3*p3; double b = -p3*p1;
			if (a+b == 0) a = b = 1;
			r.z = b/(a+b);
			r.x = a/(a+b);
		}
	}
	return r;
}

//	以下、ContFindCommonPoint()関数専用のマクロ
#define XY()	sub_vector( PTM::TSubVectorDim<0,2>() )
#define CalcSupport(n)														\
	p_id[n] = a->Support(p[n], a2z.Ori().Conjugated() * (v[n]));			\
	q_id[n] = b->Support(q[n], b2z.Ori().Conjugated() * -(v[n]));			\
	w[n] = b2z * ((Vec3d)q[n]) - a2z * ((Vec3d)p[n]);						\
	colcounter++;


int contFindCommonPoint3DRefinementCount;

/**	並進のみの連続時間衝突判定のアルゴリズム
*/
int FASTCALL ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist){
	qpTimerForCollision.Count();

	nSupport = 0;
	Quaterniond w2z;	//	range が+Zになるような座標系
	Vec3d u = -dir;	//	u: 物体ではなく原点の速度の向きなので - がつく．
	if (u.Z() < -1+epsilon){
		w2z = Quaterniond::Rot(Rad(180), 'x');
	}else if (u.Z() < 1-epsilon){	//	TODO:直接Quaternionを求めるべき
		Matrix3d matW2z = Matrix3d::Rot(u, Vec3f(0,0,1), 'z');
		w2z.FromMatrix(matW2z);		
		w2z = w2z.Inv();
	}
	Posed a2z;
	a2z.Ori() = w2z * a2w.Ori();
	a2z.Pos() = w2z * a2w.Pos();
	Posed b2z;
	b2z.Ori() = w2z * b2w.Ori();
	b2z.Pos() = w2z * b2w.Pos();
	
	//	GJKと似た方法で，原点とCSO(Configuration Space Obstacle)の交点を求める
	//	まず、2次元で見たときに、原点が含まれるような三角形または線分を作る
	//	w0を求める
	v[0] = Vec3d(0,0,1);
	CalcSupport(0);
	if (w[0].Z() > end)
	{
		qpTimerForCollision.Accumulate(coltimePhase1);
		return -1;	//	範囲内では接触しないが，endより先で接触するかもしれない．
	}

	if (w[0].Z() < start) {	//	反対側のsupportを求めてみて，範囲外か確認
		v[3] = Vec3d(0, 0, -1);
		CalcSupport(3);
		if (w[3].Z() < start) {
			//	範囲内では接触しないが，後ろに延長すると接触するかもしれない．
			qpTimerForCollision.Accumulate(coltimePhase1);
			return -2;
		}
	}

	//	w1を求める
	v[1] = Vec3d(w[0].X(), w[0].Y(), 0);
	if (v[1].XY().square() < epsilon2){		//	w0=衝突点の場合
		normal = u.unit();
		pa = p[0]; pb = q[0];
		dist = w[0].Z();
		nSupport = 1;
		dec[0] = 1; dec[1] = 0; dec[2] = 0;
		qpTimerForCollision.Accumulate(coltimePhase1);
		if (dist > end) return -1;
		if (dist < start) return -2;
		return 1;
	}
	CalcSupport(1);
	if (w[1].XY() * v[1].XY() > 0)
	{
		qpTimerForCollision.Accumulate(coltimePhase1);
		return 0;	//	w[1]の外側にOがあるので触ってない
	}
	qpTimerForCollision.Accumulate(coltimePhase1);
	
	//	w[0]-w[1]-w[0] を三角形と考えてスタートして，oが三角形の内部に入るまで繰り返し
	ids[0] = 1;	//	新しい頂点
	ids[1] = 0;	//	もとの線分
	ids[2] = 0;	//	もとの線分
	int count = 0;
	while(1){
		count++;
		if (count > 1000) {
			DSTR << "Too many loop in 2D tri search of CCDGJK." << std::endl;
			ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "2DS");
			bGJKDebug = true;
		}
#ifdef _DEBUG
		if (bGJKDebug) {
			DSTR << "ids:";
			for (int i = 0; i < 3; ++i) {
				DSTR << ids[i];
			}
			DSTR << " w:" << std::endl;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 2; ++j) {
					DSTR << "\t" << w[i % 3][j];
				}
				DSTR << std::endl;
			}
		}
#endif
		Vec3d vNew;
		double s1, s2, s;
		s1 = w[(int)ids[0]].XY() ^ w[(int)ids[1]].XY();
		s2 = w[(int)ids[2]].XY() ^ w[(int)ids[0]].XY();
		if (s1 >= s2 && s1 > epsilon) {	//	両方から見て外にある場合、より大きい方を取る。小さい方を使うと、スレスレの場合に無限ループに陥る場合がある。epsilon = 1e-12. 2018.08.16 hase 
			s = s1;
			//	1-0の法線の向きvNewでsupport pointを探し、新しい三角形にする。
			Vec2d l = w[(int)ids[1]].XY() - w[(int)ids[0]].XY();
			assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
			double ll_inv = 1 / l.square();
			vNew.XY() = (w[(int)ids[1]].XY()*l*ll_inv) * w[(int)ids[0]].XY()
				- (w[(int)ids[0]].XY()*l*ll_inv) * w[(int)ids[1]].XY();
			vNew.Z() = 0;
			ids[2] = ids[0];
			ids[0] = FindVacantId(ids[1], ids[2]);
			//		}else if ((s = w[(int)ids[2]].XY() ^ w[(int)ids[0]].XY()) > epsilon){
		} else if (s2 > epsilon) {
			s = s2;
			//	2-0の法線の向きvでsupport pointを探し、新しい三角形にする。
			Vec2d l = w[(int)ids[2]].XY() - w[(int)ids[0]].XY();
			assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
			double ll_inv = 1/l.square();
			vNew.XY() = (w[(int)ids[2]].XY()*l*ll_inv) * w[(int)ids[0]].XY()
				   - (w[(int)ids[0]].XY()*l*ll_inv) * w[(int)ids[2]].XY();
			vNew.Z() = 0;
			ids[1] = ids[0];
			ids[0] = FindVacantId(ids[1], ids[2]);
		} else {
			//	点Oは三角形の内側にある。
			if (ids[1] == ids[2]){
				//	1と2が同じ点=最初からonlineだったため、3角形ができなかった。
				assert(ids[0] == 1);
				assert(ids[1] == 0);
				ids[2] = 2;
				w[2] = w[0];
				v[2] = v[0];
				p[2] = p[0];
				q[2] = q[0];
			}
			break;
		}
		v[ids[0]] = vNew;
#ifdef _DEBUG
		if (bGJKDebug) {
			DSTR << "v:" << vNew << std::endl;
		}
#endif
		CalcSupport(ids[0]);	//	法線の向きvNewでサポートポイントを探す
		if (w[ids[0]].XY() * v[ids[0]].XY() > epsilon){	//	0の外側にoがあるので触ってない
			qpTimerForCollision.Accumulate(coltimePhase2);
			return 0;
		}
		//	新しいsupportが1回前の線分からまったく動いていない → 点Oは外側
		double d1 = -vNew.XY() * (w[(int)ids[0]].XY()-w[(int)ids[1]].XY());
		double d2 = -vNew.XY() * (w[(int)ids[0]].XY()-w[(int)ids[2]].XY());
		if (d1 < -epsilon || d2 < -epsilon) {
			qpTimerForCollision.Accumulate(coltimePhase2);
			return 0;
		}
		if (count > 1000) return 0;
	}
	ids[3] = 3;
	//	三角形 ids[0-1-2] の中にoがある．ids[0]が最後に更新した頂点w
	//GJK部分	
	qpTimerForCollision.Accumulate(coltimePhase2);
	//	三角形を小さくしていく
	int notuse = -1;
	count = 0;
	Vec3d lastV;
	double lastZ = DBL_MAX;
	int lastVid = -1;
	while(1){
		count ++;
		if (count > 1000) {
			DSTR << "Too many loop in 3D refinement of CCDGJK." << std::endl;
			ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "3DR");
			bGJKDebug = true;
		}
		Vec3d s;		//	三角形の有向面積
		s = (w[ids[1]]-w[ids[0]]) % (w[ids[2]]-w[ids[0]]);
		if (s.Z() > epsilon || -s.Z() > epsilon){
			if (s.Z() < 0){		//	逆向きの場合、ひっくり返す
				std::swap(ids[1], ids[2]);
				s *= -1;
			}
#ifdef _DEBUG
			if (bGJKDebug) DSTR << "TRI ";
#endif
			//	三角形になる場合
			notuse = -1;
			//lastTriV = 
			v[ids[3]] = s.unit();	//	3角形の法線を使う
		}else{
#ifdef _DEBUG
			if (bGJKDebug) DSTR << "LINE";
#endif
			int id0, id1;
			if (notuse >= 0){	
				//	前回も線分だった場合。新しい点と古い線分のどちらかの頂点で新たな線分を作る。
				double ip1 = w[ids[notuse]].XY() * w[ids[(notuse+1)%3]].XY();
				double ip2 = w[ids[notuse]].XY() * w[ids[(notuse+2)%3]].XY();
				double ipN = w[ids[(notuse+1)%3]].XY() * w[ids[(notuse+2)%3]].XY();
				if (ip1 < ip2){
					id0 = notuse;
					id1 = (notuse+1)%3;
					notuse = (notuse+2)%3;
				}else{
					id0 = notuse;
					id1 = (notuse+2)%3;
					notuse = (notuse+1)%3;
				}
			}
			else{
				//	初めて線分になる場合。
				//	まず、裏側の線分（z軸向きに見て、他の1or2辺の影に隠れる線分)にdoutUseマークをつける。
				bool dontUse[3] = {false, false, false};
				for(int i=0; i<3; ++i){
					Vec3d dir = w[ids[(i+1)%3]] - w[ids[i]];
					double len = dir.norm();
					if (len < 1e-15){
						dontUse[i] = true;
					}else{
						dir /= len;
						Vec3d n = Vec3d(0,0,1) - dir.z * dir;
						if (n * (w[ids[(i+2)%3]] - w[ids[i]]) < -1e-8){
							dontUse[i] = true;
						}
					}
				}
				//	使える線分のうち、原点を含むものを探す。
				double ipMin = DBL_MAX;
				int use = -1;
				for(int i=0; i<3; ++i){
					if (dontUse[i]) continue;
					double ip = w[ids[i]].XY() * w[ids[(i+1)%3]].XY();
					if (ip < ipMin){
						ipMin = ip;
						use = i;
					}
				}
				id0 = use;
				id1 = (use+1)%3;
				notuse = (use+2)%3;
			}
			//	初めてならば、2頂点の法線の平均の線分に垂直な成分をつかう。
			Vec3d ave = v[ids[id0]] + v[ids[id1]];
			Vec3d line = (w[ids[id1]] - w[ids[id0]]);
			double len = line.norm();
			if (len == 0){
				DSTR << "id0:" << id0 << " id1:" << id1 << std::endl;
				DSTR << "ids:"; for(int i=0; i<4; ++i) DSTR << ids[i]; DSTR << std::endl;
				DSTR << "w:"; for(int i=0; i<4; ++i) DSTR << w[i]; DSTR << std::endl;
				DSTR << "v:"; for(int i=0; i<4; ++i) DSTR << v[i]; DSTR << std::endl;
#ifdef _MSC_VER
				__debugbreak();
#endif
			}else{
				line /= len;
				ave = ave - (ave * line) * line;
			}
			v[ids[3]] = ave.unit();
		}
#if 1
		if (lastVid >= 0 && (v[lastVid] - v[ids[3]]).square() < threshold) {
			//DSTR << a->GetName() << b->GetName() << " The same v" << std::endl;
			goto final;
		}
#endif
		//	新しい w w[3] を求める
		CalcSupport(ids[3]);
#ifdef _DEBUG
		if (bGJKDebug){
			DSTR << "v:" << v[ids[3]];
			for(int i=0; i<4; ++i){
				DSTR << "  w[" << (int) ids[i] << "] = " << w[ids[i]];
			}
			DSTR << std::endl;
			/*	//	ExcelでWとVを見るための出力	
			for(int j=0; j<3; ++j) DSTR << v[ids[3]][j] << "\t";
			DSTR << std::endl;
			for(int i=0; i<4; ++i){
				if (i==notuse) continue;
				for(int j=0; j<3; ++j) DSTR << w[ids[i]][j] << "\t";
				DSTR << std::endl;
			}
			DSTR << std::endl;
			*/

			DSTR << "notuse:" << notuse;
			for(int i=0; i<4; ++i) DSTR << " " << ids[i];
		}
#endif
		lastVid = ids[3];
		if (notuse>=0){	//	線分の場合、使った2点と新しい点で三角形を作る
			int nid[3];
			nid[0] = ids[(notuse+1)%3];
			nid[1] = ids[(notuse+2)%3];
			nid[2] = ids[3];
			Vec3d dec = TriDecompose(w[nid[0]].XY(), w[nid[1]].XY(), w[nid[2]].XY());
			double newZ = w[nid[0]].z * dec[0] + w[nid[1]].z * dec[1] + w[nid[2]].z * dec[2];
#ifdef _DEBUG
			if (bGJKDebug){
				DSTR << " newZ:" << newZ << "  dec:"<< dec << std::endl;
			}
#endif
#if 1
			//			if (newZ > lastZ + epsilon) {
			if (newZ >= lastZ) {
			//DSTR << "Stop: newZ:" << newZ << " > lastZ:" << lastZ << std::endl;
				goto final;
			}
#endif
			lastZ = newZ;
			std::swap(ids[notuse], ids[3]);
		}else{
			//	どの2点とw[3]で三角形を作れるか確認する
			Vec3d decs[3];
			double minDec[3];
			int i;
			for(i=0; i<3; ++i){
				int nid0 = ids[i];
				int nid1 = ids[(i+1)%3];
				int nid2 = ids[3];
				decs[i] = TriDecompose(w[nid0].XY(), w[nid1].XY(), w[nid2].XY());
				minDec[i] = std::min(decs[i][0], std::min(decs[i][1], decs[i][2]));
			}
			if (minDec[0] > minDec[1]){
				if (minDec[0] > minDec[2]) i = 0;
				else i=2;
			}else{
				if (minDec[1] > minDec[2]) i = 1;
				else i=2;
			}
			//	ここで改善したかチェックする
			int nid0 = ids[i];
			int nid1 = ids[(i+1)%3];
			int nid2 = ids[3];
			Vec3d dec = decs[i];
			double newZ = w[nid0].z * dec[0] + w[nid1].z * dec[1] + w[nid2].z * dec[2];
#ifdef _DEBUG
			if (bGJKDebug){
				DSTR << " newZ:" << newZ << std::endl;
			}
#endif
			if (newZ >= lastZ - epsilon) {
				//goto final;	//	Zだけでは打ち切らない
				double progress = -DBL_MAX;
				for (int i = 0; i < 3; ++i) {
					if (i == notuse) continue;
					double d = (w[ids[3]] - w[ids[i]]) * v[ids[3]];
					if (progress < d) progress = d;
				}
				if (progress > -epsilon) {
					//DSTR << a->GetName() << b->GetName() << " W on the same plane." << std::endl;
					goto final;
				}
			}
			lastZ = newZ;
			std::swap(ids[(i+2)%3], ids[3]);
		}
	}
	//	無事停止
final:
	if (notuse >=0){
		int id0 = ids[(notuse+1)%3];	int id1 = ids[(notuse+2)%3];
		double a = w[id0].norm();		double b = w[id1].norm();
		if (a+b > 1e-10){ dec[0] = b/(a+b); dec[1] = a/(a+b); }
		else {dec[0] =0.5; dec[1]=0.5;}
		dec[2]=0;
		pa = dec[0]*(Vec3d)p[id0] + dec[1]*(Vec3d)p[id1];
		pb = dec[0]*(Vec3d)q[id0] + dec[1]*(Vec3d)q[id1];
		dist = dec[0]*w[id0].z + dec[1]*w[id1].z;
		ids[0] = id0; ids[1] = id1;
		nSupport = 2;
	}else{
		dec = TriDecompose(w[ids[0]].XY(), w[ids[1]].XY(), w[ids[2]].XY());
		pa = dec[0]*(Vec3d)p[ids[0]] + dec[1]*(Vec3d)p[ids[1]] + dec[2]*(Vec3d)p[ids[2]];
		pb = dec[0]*(Vec3d)q[ids[0]] + dec[1]*(Vec3d)q[ids[1]] + dec[2]*(Vec3d)q[ids[2]];
		dist = dec[0]*w[ids[0]].z + dec[1]*w[ids[1]].z + dec[2]*w[ids[2]].z;
		nSupport = 3;
	}

	normal = w2z.Conjugated() * v[ids[3]];

	normal.unitize();
	//	HASE_REPORT
	qpTimerForCollision.Accumulate(coltimePhase3);
	static bool bSave = false;
	if (bSave){
		ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "bSave");
	}
	contFindCommonPoint3DRefinementCount = count;
	if (dist > end) return -1;
	if (dist < start) return -2;
	return 1;
}


float FASTCALL RaycastOreOre(Vec3d o, Vec3d ray, Vec3d tri1, Vec3d tri2, Vec3d tri3)//ベクトルと三角形の衝突判定　距離を返す　当たらなければ-1 
{
	Vec3d edge1 = tri2 - tri1;
	Vec3d edge2 = tri3 - tri1;
	Vec3d P = ray%edge2;
	float det = P*edge1;

	if (det > epsilon)
	{
		Vec3d T = o - tri1;
		float u = P*T;
		if ((u >= 0) && (u <= 1 * det))
		{
			Vec3d Q = T%edge1;
			float v = Q*ray;
			if ((v >= 0) && (u + v <= 1 * det))
			{
				//ここまでくれば当たる
				//tは距離
				float t = Q*edge2 / det;
				return t;
				
			}
		}
	}
	return -1;
}


//加速適用版
int FASTCALL ContFindCommonPointAccel(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist) {
nSupport = 0;
//	range が+Zになるような座標系を求める．
Quaterniond w2z;
Vec3d u = -dir;	//	u: 物体ではなく原点の速度の向きなので - がつく．
if (u.Z() < -1 + epsilon) {
	w2z = Quaterniond::Rot(Rad(180), 'x');
}
else if (u.Z() < 1 - epsilon) {
	Matrix3d matW2z = Matrix3d::Rot(u, Vec3f(0, 0, 1), 'z');
	w2z.FromMatrix(matW2z);
	w2z = w2z.Inv();
}
Posed a2z;
a2z.Ori() = w2z * a2w.Ori();
a2z.Pos() = w2z * a2w.Pos();
Posed b2z;
b2z.Ori() = w2z * b2w.Ori();
b2z.Pos() = w2z * b2w.Pos();
	qpTimerForCollision.Count();
//	GJKと似た方法で，交点を求める
//	まず、2次元で見たときに、原点が含まれるような三角形または線分を作る
//	w0を求める
 v[0] = Vec3d(0, 0, 1);
 CalcSupport(0);
 if (w[0].Z() > end)
 {
	 qpTimerForCollision.Accumulate(coltimePhase1);
	 return -1;	//	範囲内では接触しないが，endより先で接触するかもしれない．
 }

 if (w[0].Z() < start) {	//	反対側のsupportを求めてみて，範囲外か確認
	 v[3] = Vec3d(0, 0, -1);
	 CalcSupport(3);
	 if (w[3].Z() < start) {
		 //	範囲内では接触しないが，後ろに延長すると接触するかもしれない．
		 qpTimerForCollision.Accumulate(coltimePhase1);
		 return -2;
	 }
 }

 //	w1を求める 
 v[1] = Vec3d(w[0].X(), w[0].Y(), 0);
 if (v[1].XY().square() < epsilon2) {		//	w0=衝突点の場合
	 normal = u.unit();
	 pa = p[0]; pb = q[0];
	 dist = w[0].Z();
	 nSupport = 1;
	 dec[0] = 1; dec[1] = 0; dec[2] = 0;
	 qpTimerForCollision.Accumulate(coltimePhase1);
	 if (dist > end) return -1;
	 if (dist < start) return -2;
	 return 1;
 }
 CalcSupport(1);
 if (w[1].XY() * v[1].XY() > 0) 
 {
	 qpTimerForCollision.Accumulate(coltimePhase1);
	 return 0;	//	w[1]の外側にOがあるので触ってない
 }
 qpTimerForCollision.Accumulate(coltimePhase1);

//	w[0]-w[1]-w[0] を三角形と考えてスタートして，oが三角形の内部に入るまで繰り返し
ids[0] = 1;	//	新しい頂点
ids[1] = 0;	//	もとの線分
ids[2] = 0;	//	もとの線分
while (1) 
{
	double s;
	Vec3d vNew;
	if ((s = w[(int)ids[0]].XY() ^ w[(int)ids[1]].XY()) > epsilon) {
		//	点Oが、線分1-0から、三角形の外にはみ出している場合  
		//		... epsilon=1e-8だと無限ループ，1e-7でも稀に無限ループ
		//	1-0の法線の向きvNewでsupport pointを探し、新しい三角形にする。
		Vec2d l = w[(int)ids[1]].XY() - w[(int)ids[0]].XY();
		assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
		double ll_inv = 1 / l.square();
		vNew.XY() = (w[(int)ids[1]].XY()*l*ll_inv) * w[(int)ids[0]].XY()
			- (w[(int)ids[0]].XY()*l*ll_inv) * w[(int)ids[1]].XY();
		vNew.Z() = 0;
		ids[2] = ids[0];
		ids[0] = FindVacantId(ids[1], ids[2]);
	}
	else if ((s = w[(int)ids[2]].XY() ^ w[(int)ids[0]].XY()) > epsilon)
	{
		//	点Oが、線分2-0から、三角形の外にはみ出している場合
		//	2-0の法線の向きvでsupport pointを探し、新しい三角形にする。
		Vec2d l = w[(int)ids[2]].XY() - w[(int)ids[0]].XY();
		assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
		double ll_inv = 1 / l.square();
		vNew.XY() = (w[(int)ids[2]].XY()*l*ll_inv) * w[(int)ids[0]].XY()
			- (w[(int)ids[0]].XY()*l*ll_inv) * w[(int)ids[2]].XY();
		vNew.Z() = 0;
		ids[1] = ids[0];
		ids[0] = FindVacantId(ids[1], ids[2]);
		}
	else {
		//	点Oは三角形の内側にある。
		if (ids[1] == ids[2]) {
			//	1と2が同じ点=最初からonlineだったため、3角形ができなかった。
			assert(ids[0] == 1);
			assert(ids[1] == 0);
			ids[2] = 2;
			w[2] = w[0];
			v[2] = v[0];
			p[2] = p[0];
			q[2] = q[0];
		}
		break;
	}
	v[ids[0]] = vNew;
	CalcSupport(ids[0]);	//	法線の向きvNewでサポートポイントを探す
	if (w[ids[0]].XY() * v[ids[0]].XY() > -epsilon2) {	//	0の外側にoがあるので触ってない
		qpTimerForCollision.Accumulate(coltimePhase2);
		return 0;
		
	}
	//	新しいsupportが1回前の線分からまったく動いていない → 点Oは外側
	double d1 = -vNew.XY() * (w[(int)ids[0]].XY() - w[(int)ids[1]].XY());
	double d2 = -vNew.XY() * (w[(int)ids[0]].XY() - w[(int)ids[2]].XY());
	if (d1 < epsilon2 || d2 < epsilon2) 
	{
		qpTimerForCollision.Accumulate(coltimePhase2);
		return 0;
	}
		
}
ids[3] = 3;
qpTimerForCollision.Accumulate(coltimePhase2);

//	三角形 ids[0-1-2] の中にoがある．ids[0]が最後に更新した頂点w

	//GJK部分
#if 0	//	長谷川専用デバッグコード．三角形が原点を含むことを確認
	int sign[3];
	double d[3];
	for (int i = 0; i<3; ++i) {
		Vec2d edge = w[ids[(i + 1) % 3]].XY() - w[ids[i]].XY();
		Vec2d n = Vec2d(-edge.Y(), edge.X());
		d[i] = n * w[ids[i]].XY();
		double epsilon = 1e-5;
		sign[i] = d[i] > epsilon ? 1 : d[i] < -epsilon ? -1 : 0;
	}
	if (sign[0] * sign[1] < 0 || sign[1] * sign[2] < 0) {
		DSTR << "tri: 0-2:" << std::endl;
		for (int i = 0; i<3; ++i) {
			DSTR << w[ids[i]].X() << "\t" << w[ids[i]].Y() << std::endl;
		}
		DSTR << "dist: " << std::endl;
		for (int i = 0; i<3; ++i) {
			DSTR << d[i] << std::endl;
		}
		DSTR << "Error could not find a traiangle including origin." << std::endl;
	}
#endif
	for (int i = 0; i < 3; i++)
	{
		v[i].unitize();
	}
	ids[3] = 3;
	//	三角形を小さくしていく
	int notuse = -1;
	int count = 0;
	Vec3d lastTriV;
	double lastZ = DBL_MAX;
	double cbiasParam = 0; //現在のバイアス値
	while (1) {
		count++;
		if (count > 1000) {
#if 1
			DSTR << "Too many loop in CCDGJK." << std::endl;
			ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "3DRA");
			//DebugBreak();
			bGJKDebug = true;
#endif
		}
		Vec3d s;		//	三角形の有向面積
		s = (w[ids[1]] - w[ids[0]]) % (w[ids[2]] - w[ids[0]]);
		if (s.Z() > epsilon || -s.Z() > epsilon) {
			if (s.Z() < 0) {		//	逆向きの場合、ひっくり返す
				std::swap(ids[1], ids[2]);
				s *= -1;
			}
#ifdef _DEBUG
			if (bGJKDebug) DSTR << "TRI ";
#endif
			//	三角形になる場合
			notuse = -1;
			lastTriV = s.unit();
			
			if (cbiasParam >= 0) {
				cbiasParam = lastTriV.z * 0.9; //三角面の法線と相対ベクトル（-z)との内積を加速バイアス値にする
				//相対ベクトルと三角面が平行に近い場合加速したサポートベクトルが既存サポートベクトルに近づきすぎるため
			}
			
			if (cbiasParam > 0) {
#define NORM_BIAS 1 //加速flag 0にすると加速無効化
#if NORM_BIAS == 1
				
				//二次元上での原点と三角形頂点の距離でバイアスを掛ける 3点のサポートベクトル比版
				Vec3d tridec = TriDecompose(w[ids[0]].XY(), w[ids[1]].XY(), w[ids[2]].XY());
				Vec3d newSup = v[ids[0]] * tridec[0] + v[ids[1]] * tridec[1] + v[ids[2]] * tridec[2];
				newSup = newSup*cbiasParam + lastTriV;
				v[ids[3]] = newSup;
#else
				lastTriV = v[ids[3]] = s.unit();
#endif
			}
			else {
				//バイアス値が0以下なら法線で探す
				v[ids[3]] = lastTriV;
			}
			//	新しい w w[3] を求める
			CalcSupport(ids[3]);
		}
		else {
#ifdef _DEBUG
			if (bGJKDebug) DSTR << "LINE";
#endif
			int id0, id1;
			if (notuse >= 0) {
				//	前回も線分だった場合。新しい点と古い線分のどちらかの頂点で新たな線分を作る。
				double ip1 = w[ids[notuse]].XY() * w[ids[(notuse + 1) % 3]].XY();
				double ip2 = w[ids[notuse]].XY() * w[ids[(notuse + 2) % 3]].XY();
				double ipN = w[ids[(notuse + 1) % 3]].XY() * w[ids[(notuse + 2) % 3]].XY();
				if (ip1 < ip2) {
					id0 = notuse;
					id1 = (notuse + 1) % 3;
					notuse = (notuse + 2) % 3;
				}
				else {
					id0 = notuse;
					id1 = (notuse + 2) % 3;
					notuse = (notuse + 1) % 3;
				}
			}
			else {
				//	初めて線分になる場合。

#if NORM_BIAS >= 10 //バイアスかかっていたら戻す（球に対応できないので無効化中）
				if (lastTriV.square() > 0) {
					v[ids[3]] = lastTriV;
					CalcSupport(ids[3]);
				}
				notuse = ids[3];
#else
				//	まず、裏側の線分（z軸向きに見て、他の1or2辺の影に隠れる線分)にdontUseマークをつける。
				bool dontUse[3] = { false, false, false };
				for (int i = 0; i < 3; ++i) {
					Vec3d dir = w[ids[(i + 1) % 3]] - w[ids[i]];
					double len = dir.norm();
					if (len < 1e-15) {
						dontUse[i] = true;
					}
					else {
						dir /= len;
						Vec3d n = Vec3d(0, 0, 1) - dir.z * dir;
							if (n * (w[ids[(i + 2) % 3]] - w[ids[i]]) < -1e-8) {
								dontUse[i] = true;
							}
						}
					}
				//	使える線分のうち、原点を含むものを探す。
				double ipMin = DBL_MAX;
				int use = -1;
				for (int i = 0; i < 3; ++i) {
					if (dontUse[i]) continue;
					double ip = w[ids[i]].XY() * w[ids[(i + 1) % 3]].XY();
					if (ip < ipMin) {
						ipMin = ip;
						use = i;
						}
					}
				id0 = use;
				id1 = (use + 1) % 3;
				notuse = (use + 2) % 3;
			}
			//	support vector用法線には、前回の(最後に作った三角形の)法線を使う。
			if (lastTriV.square() != 0) {
				v[ids[3]] = lastTriV;
			}
			else {
				//	初めてならば、2頂点の法線の平均の線分に垂直な成分をつかう。
				Vec3d ave = (v[ids[id0]] + v[ids[id1]])/2.0;
				Vec3d line = (w[ids[id1]] - w[ids[id0]]);
				double len = line.XY().norm();
				if (len == 0) {
					DSTR << "id0:" << id0 << " id1:" << id1 << std::endl;
					DSTR << "ids:"; for (int i = 0; i < 4; ++i) DSTR << ids[i]; DSTR << std::endl;
					DSTR << "w:"; for (int i = 0; i < 4; ++i) DSTR << w[i]; DSTR << std::endl;
					DSTR << "v:"; for (int i = 0; i < 4; ++i) DSTR << v[i]; DSTR << std::endl;
					__debugbreak();
				}
				else {
#if NORM_BIAS >= 1
					double p = ave.z;
					double w0len = w[ids[id0]].XY().norm();
					//double w1len = w[ids[id1]].XY().norm();
					double rate = 1-(w0len / len);
					Vec3d acc = (v[ids[id0]] * rate + v[ids[id1]] * (1 - rate));
					//line /= len;
					//ave = ave - (ave * line) * line;
					ave = ave * (1 - p) + acc * p;
#else
					line /= len;
					ave = ave - (ave * line) * line;
#endif
				}
				v[ids[3]] = ave.unit();
#endif
			}
			CalcSupport(ids[3]);
		}
#ifdef _DEBUG
		if (bGJKDebug) {
			DSTR << "v:" << v[ids[3]];
			for (int i = 0; i < 4; ++i) {
				DSTR << "  w[" << (int)ids[i] << "] = " << w[ids[i]];
			}
			DSTR << std::endl;
			/*	//	ExcelでWとVを見るための出力
			for(int j=0; j<3; ++j) DSTR << v[ids[3]][j] << "\t";
			DSTR << std::endl;
			for(int i=0; i<4; ++i){
			if (i==notuse) continue;
			for(int j=0; j<3; ++j) DSTR << w[ids[i]][j] << "\t";
			DSTR << std::endl;
			}
			DSTR << std::endl;
			*/

			DSTR << "notuse:" << notuse;
			for (int i = 0; i < 4; ++i) DSTR << " " << ids[i];
		}
#endif
		if (notuse >= 0) {	//	線分の場合、使った2点と新しい点で三角形を作る
			int nid[3];
			nid[0] = ids[(notuse + 1) % 3];
			nid[1] = ids[(notuse + 2) % 3];
			nid[2] = ids[3];
			Vec3d dec = TriDecompose(w[nid[0]].XY(), w[nid[1]].XY(), w[nid[2]].XY());
			double newZ = w[nid[0]].z * dec[0] + w[nid[1]].z * dec[1] + w[nid[2]].z * dec[2];
#ifdef _DEBUG
			if (bGJKDebug) {
				DSTR << " newZ:" << newZ << "  dec:" << dec << std::endl;
			}
#endif
			if (newZ + epsilon >= lastZ) {
				notuse = -1;
				goto final2;
			}
			lastZ = newZ;
			std::swap(ids[notuse], ids[3]);
		}
		else {
			//	どの2点とw[3]で三角形を作れるか確認する
			Vec3d decs[3];
			double minDec[3];
			int i;
			for (i = 0; i < 3; ++i) {
				int nid0 = ids[i];
				int nid1 = ids[(i + 1) % 3];
				int nid2 = ids[3];
				decs[i] = TriDecompose(w[nid0].XY(), w[nid1].XY(), w[nid2].XY());
				minDec[i] = std::min(decs[i][0], std::min(decs[i][1], decs[i][2]));
			}
			if (minDec[0] > minDec[1]) {
				if (minDec[0] > minDec[2]) i = 0;
				else i = 2;
			}
			else {
				if (minDec[1] > minDec[2]) i = 1;
				else i = 2;
			}
			//	ここで改善したかチェックする
			int nid0 = ids[i];
			int nid1 = ids[(i + 1) % 3];
			int nid2 = ids[3];
			int amariID = ids[(i + 2) % 3];
			Vec3d dec = decs[i];
			double newZ = w[nid0].z * dec[0] + w[nid1].z * dec[1] + w[nid2].z * dec[2];
#ifdef _DEBUG
			if (bGJKDebug) {
				DSTR << " newZ:" << newZ << std::endl;
			}
#endif

			if (newZ + epsilon >= lastZ) {
#if NORM_BIAS >= 1		
				std::swap(ids[(i + 2) % 3], ids[3]);
				//前回と同じサポートポイントだった場合ループ抜ける　これがないと無限ループするときがあったが消せる気がする
				if ((lastTriV.z - v[ids[3]].z) < epsilon) {
					goto final2;
				}
				//加速が適用されない場合最終処理へ
				if (cbiasParam <= 0) {

					goto final2;
				}
				//加速適用（cbiasParam > 0）はバイアス値をマイナスにして加速を切ってまたループ
				cbiasParam = -1;
				continue;
#else			
				goto final2;
#endif
			}
			lastZ = newZ;
			std::swap(ids[(i + 2) % 3], ids[3]);		
			
		}
	}
	//	無事停止
	final2:
	if (notuse >= 0) {
		int id0 = ids[(notuse + 1) % 3];	
		int id1 = ids[(notuse + 2) % 3];
		double a = w[id0].XY().norm();		
		double b = w[id1].XY().norm();
		Vec3d dec;
		if (a + b > 1e-10) {
			dec[0] = b / (a + b); dec[1] = a / (a + b);
			}
		else {
			dec[0] = 0.5;
			dec[1] = 0.5;
			}
		dec[2] = 0;
		pa = dec[0] * (Vec3d)p[id0] + dec[1] * (Vec3d)p[id1];
		pb = dec[0] * (Vec3d)q[id0] + dec[1] * (Vec3d)q[id1];
		dist = dec[0] * w[id0].z + dec[1] * w[id1].z;
		ids[0] = id0; ids[1] = id1;
		nSupport = 2;
		}
	else {
		dec = TriDecompose(w[ids[0]].XY(), w[ids[1]].XY(), w[ids[2]].XY());
		pa = dec[0] * (Vec3d)p[ids[0]] + dec[1] * (Vec3d)p[ids[1]] + dec[2] * (Vec3d)p[ids[2]];
		pb = dec[0] * (Vec3d)q[ids[0]] + dec[1] * (Vec3d)q[ids[1]] + dec[2] * (Vec3d)q[ids[2]];
		dist = dec[0] * w[ids[0]].z + dec[1] * w[ids[1]].z + dec[2] * w[ids[2]].z;
		nSupport = 3;
	}
#if NORM_BIAS >= 1
	if (lastTriV.square() != 0) {
		//lasttriVは三角面の法線が確実に入るので
		normal = w2z.Conjugated() * lastTriV;
	}
	else {
		normal = w2z.Conjugated() * v[ids[3]];
	}
#else
	normal = w2z.Conjugated() * v[ids[3]];
#endif
	normal.unitize();
	qpTimerForCollision.Accumulate(coltimePhase3);
	static bool bSave = false;
	if (bSave) {
		ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "bSaveA");
	}
	if (dist > end) return -1;
	if (dist < start) return -2;
	return 1;
}

#define USE_NON_CONTINUOUS_DETECTORS
#ifdef USE_NON_CONTINUOUS_DETECTORS
/*	GJKアルゴリズム(FindCommonPoint(), FindClosestPoint())の実装．
	SOLIDのソースを参考にして書いた部分です．
	FindClosestPointは使っていません．
	
	FindCommonPointは，
		CDShapePair::Detect()
		CDContactAnalysis::CalcNormal(CDShapePair* cp)
	で使っています．ペナルティ法を使うと呼び出されます．	*/

//	過去のSupportPointが張る形状(点・線分・3角形，4面体)．最大4点まで．
static int usedBits;		//	下位4ビットが，どの4点で現在の形状が構成されるかを表す．
static int lastId;			//	最後に見つけたSupportPointをしまった場所(0～3)
static int lastBit;			//	lastBit = 1<<lastId (ビットで表したもの）
static int allUsedBits;		//	usedBitsにlastBitを加えたもの
static double det[16][4];	//	係数
//	det[3][0] * p[0] + det[3][1]*p[1] + det[3][2]*p[2] / sum で最近傍点が求まる
//	p_qベクトルたちが張る形状の体積(or長さor面積)．4点から作れる16通りについて

/*
      a   |
     /    | v = (1-k) a + k b
    /     | va = (1-k)aa + kba = vb = (1-k)ab + kbb
   /v     | -kaa + kba + kab - kbb = -aa + ab
  b       | k = a^2-ab / (a^2+b^2-2ab), 1-k = b^2-ab / (a^2+b^2-2ab)
----------+ なので， k = a^2-ab を と (k-1) = b^2-abを記録しておく	*/

static Vec3d lastNormal;
static double lastDist;

///	det(長さ・面積・体積)の計算．垂線の足が頂点を何:何に内分するかも計算
inline void CalcDet() {
	static double dotp[4][4];	//	p_q[i] * p_q[j] 

	//	新しく増えた点(lastId)について，内積を計算
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<=1){
		if (usedBits & curPoint) 
			dotp[i][lastId] = dotp[lastId][i] = p_q[i] * p_q[lastId];
	}
	dotp[lastId][lastId] = p_q[lastId] * p_q[lastId];

	det[lastBit][lastId] = 1;


	for (int j = 0, jBit = 1; j < 4; ++j, jBit <<= 1) {
		if (usedBits & jBit) {		
			int s2 = jBit|lastBit;	//	新しく増えた点について係数の計算
			det[s2][j] = dotp[lastId][lastId] - dotp[lastId][j];	//	a^2-ab
			det[s2][lastId] = dotp[j][j] - dotp[j][lastId];			//	b^2-ab
			for (int k = 0, kBit = 1; k < j; ++k, kBit <<= 1) {	//	3点の場合
				if (usedBits & kBit) {
					int s3 = kBit|s2;
					det[s3][k] = det[s2][j] * (dotp[j][j] - dotp[j][k]) + 
						det[s2][lastId] * (dotp[lastId][j] - dotp[lastId][k]);
					det[s3][j] = det[kBit|lastBit][k] * (dotp[k][k] - dotp[k][j]) + 
						det[kBit|lastBit][lastId] * (dotp[lastId][k] - dotp[lastId][j]);
					det[s3][lastId] = det[kBit|jBit][k] * (dotp[k][k] - dotp[k][lastId]) + 
						det[kBit|jBit][j] * (dotp[j][k] - dotp[j][lastId]);
				}
			}
		}
	}
	if (allUsedBits == 15) {	//	4点の場合
		det[15][0] =	det[14][1] * (dotp[1][1] - dotp[1][0]) + 
						det[14][2] * (dotp[2][1] - dotp[2][0]) + 
						det[14][3] * (dotp[3][1] - dotp[3][0]);
		det[15][1] =	det[13][0] * (dotp[0][0] - dotp[0][1]) + 
						det[13][2] * (dotp[2][0] - dotp[2][1]) + 
						det[13][3] * (dotp[3][0] - dotp[3][1]);
		det[15][2] =	det[11][0] * (dotp[0][0] - dotp[0][2]) + 
						det[11][1] * (dotp[1][0] - dotp[1][2]) +	
						det[11][3] * (dotp[3][0] - dotp[3][2]);
		det[15][3] =	det[ 7][0] * (dotp[0][0] - dotp[0][3]) + 
						det[ 7][1] * (dotp[1][0] - dotp[1][3]) + 
						det[ 7][2] * (dotp[2][0] - dotp[2][3]);
	}
}

///	detの計算＝垂線の足が頂点を何:何に内分するかを計算
inline void CalcDetAll() {
	static double dotp[4][4];	//	p_q[i] * p_q[j] 
	//	全部の点について，内積を計算
	for (int i = 0, iBit = 1; i < 4; ++i, iBit <<= 1) {
		for (int j = i + 1, jBit = iBit << 1; j < 4; ++j, jBit <<= 1) {
			if (allUsedBits & iBit && allUsedBits && jBit) {
				dotp[i][j] = dotp[j][i] = p_q[i] * p_q[j];
			}
		}
		dotp[i][i] = p_q[i] * p_q[i];
	}
	
	//	detの計算
	det[lastBit][lastId] = 1;	//	これはいつも変わらないので、新しい点ができたときに計算

	//	こっちは変わる
	for (int i = 0, iBit = 1; i < 4; ++i, iBit <<= 1) {
		for (int j = i+1, jBit = iBit << 1; j < 4; ++j, jBit <<= 1) {
			if (allUsedBits & iBit && allUsedBits & jBit) {
				int s2 = iBit | jBit;		//	異なる２頂点から辺s2を作る
				det[s2][i] = dotp[j][j] - dotp[j][i];	//	b^2-ab
				det[s2][j] = dotp[i][i] - dotp[i][j];	//	a^2-ab
				for (int k = j+1, kBit = jBit << 1; k < 4; ++k, kBit <<= 1) {	//	3点の場合
					if (allUsedBits & kBit) {
						int s3 = kBit | s2;
						det[s3][k] = det[s2][j] * (dotp[j][j] - dotp[j][k]) +
							det[s2][i] * (dotp[i][j] - dotp[i][k]);
						det[s3][j] = det[kBit | iBit][k] * (dotp[k][k] - dotp[k][j]) +
							det[kBit | iBit][i] * (dotp[i][k] - dotp[i][j]);
						det[s3][i] = det[kBit | jBit][k] * (dotp[k][k] - dotp[k][i]) +
							det[kBit | jBit][j] * (dotp[j][k] - dotp[j][i]);
					}
				}
			}
		}
	}
	if (allUsedBits == 15) {	//	4点の場合
		det[15][0] = det[14][1] * (dotp[1][1] - dotp[1][0]) +
			det[14][2] * (dotp[2][1] - dotp[2][0]) +
			det[14][3] * (dotp[3][1] - dotp[3][0]);
		det[15][1] = det[13][0] * (dotp[0][0] - dotp[0][1]) +
			det[13][2] * (dotp[2][0] - dotp[2][1]) +
			det[13][3] * (dotp[3][0] - dotp[3][1]);
		det[15][2] = det[11][0] * (dotp[0][0] - dotp[0][2]) +
			det[11][1] * (dotp[1][0] - dotp[1][2]) +
			det[11][3] * (dotp[3][0] - dotp[3][2]);
		det[15][3] = det[7][0] * (dotp[0][0] - dotp[0][3]) +
			det[7][1] * (dotp[1][0] - dotp[1][3]) +
			det[7][2] * (dotp[2][0] - dotp[2][3]);
	}
}

//	係数から，最近傍点 v を計算
inline void CalcVector(int usedBits, Vec3d& v) {
	double sum = 0;
	v.clear();
	for (int i = 0; i < 4; ++i) {
		if (usedBits & (1<<i)) {
			sum += det[usedBits][i];
			v += p_q[i] * det[usedBits][i];
		}
	}
	v *= 1 / sum;
}

//	係数から，最近傍点と，元の2つの図形上での，その点の位置を計算
inline void CalcPoints(int usedBits, Vec3d& p1, Vec3d& p2) {
	double sum = 0;
	p1.clear();
	p2.clear();
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedBits & curPoint) {
			sum += det[usedBits][i];
			p1 += (Vec3d)p[i] * det[usedBits][i];
			p2 += (Vec3d)q[i] * det[usedBits][i];
		}
	}
	if (sum){
		double s = 1 / sum;
		p1 *= s;
		p2 *= s;
	}else{
		p1 = p[0];
		p2 = q[0];
	}
}

//	最近傍点を返す．CalcDet()を呼んでおく必要あり
inline bool CalcClosest(Vec3d& v) {
	if (!usedBits){
		usedBits = lastBit;
		v = p_q[lastId];
		return true;
	}
	int simplex[5][4];
	int nSimplex[5] = {};
	const char numVertices[] = {
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};
	int nVtx = numVertices[allUsedBits];
	simplex[nVtx][0] = allUsedBits;
	nSimplex[nVtx] = 1;
	
	for(; nVtx>1; --nVtx){
		for(int sid=0; sid<nSimplex[nVtx]; ++sid){
			int s = simplex[nVtx][sid];
			nSimplex[nVtx-1]=0;
			for(int i = 0; i!=4; ++i){
				int bit = 1<<i;
				if ((s&bit) && det[s][i] < -1e-5){	//	ここが0でよいか謎
					simplex[nVtx-1][nSimplex[nVtx-1]] = s & ~bit;
					++ nSimplex[nVtx-1];
				}
			}
			if (nSimplex[nVtx-1] == 0){
				usedBits = s;
				CalcVector(usedBits, v);	//	最近傍点を計算して返す．			
				return true;
			}
		}
	}
	return false;
}
//	新しい点wが，今までの点と等しい場合
inline bool HasSame(const Vec3d& w) {
	for (int i = 0; i < 4; ++i){
		if ((allUsedBits & (1<<i)) && (p_q[i]-w).square() < epsilon2) return true;
	}
	return false;
}
inline char VacantIdFromBits(char bits){
	return vacants[(int)bits];
}

bool FASTCALL FindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& v, Vec3d& pa, Vec3d& pb) {
	Vec3d w;
	usedBits = 0;
	allUsedBits = 0;
	int count = 0;
	do {
		lastId = VacantIdFromBits(usedBits);
		lastBit = 1 << lastId;

		p_id[lastId] = a->Support(p[lastId], a2w.Ori().Conjugated() * (-v));
		q_id[lastId] = b->Support(q[lastId], b2w.Ori().Conjugated() * v);
		colcounter++;
		lastNormal = v;
		w = a2w * p[lastId]  -  b2w * q[lastId];
		if (v*w > 0) return false;			//	原点がsupport面の外側
		if (HasSame(w)) return false;		//	supportが1点に集中＝原点は外にある．
		p_q[lastId] = w;					//	新しい点を代入
		allUsedBits = usedBits|lastBit;//	使用中頂点リストに追加
		CalcDet();
		if (!CalcClosest(v)) return false;

		count ++;
		if (count > 100){
			Vec3d vtxs[3];
			Vec3d notUsed[3];
			int nUsed=0, nNotUsed=0;
			for(int i=0; i<4; ++i){
				if (usedBits & (1<<i)){
					vtxs[nUsed] = p_q[i];
					nUsed++;
				}else{
					notUsed[nNotUsed] = p_q[i];
					nNotUsed++;
				}
			}
			if (nUsed == 3 && allUsedBits == 15){
				Vec3d n = (vtxs[2]-vtxs[0]) ^ (vtxs[1]-vtxs[0]);
				n.unitize();
				if (n * (notUsed[0] - vtxs[0]) > 0){
					n *= -1;
				}
				double dist = n * (-vtxs[0]);
/*				DSTR << "n:\t" << nUsed << "Dist:\t" << dist << std::endl;
				DSTR << "v:\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl; 
				DSTR << "Normal:\t" << n.x << "\t" << n.y << "\t" << n.z << std::endl;
				for(int i=0; i<nUsed; ++i){
					DSTR << "Use:\t" << vtxs[i].x << "\t" << vtxs[i].y << "\t" << vtxs[i].z << std::endl;
				}
				for(int i=0; i<nNotUsed; ++i){
					DSTR << "Not:\t" << notUsed[i].x << "\t" << notUsed[i].y << "\t" << notUsed[i].z << std::endl;
				}
				*/
			}else{
				/*	DSTR << "n:\t" << nUsed << std::endl;
				DSTR << "v:\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl;
				for(int i=0; i<nUsed; ++i){
					DSTR << "Use:\t" << vtxs[i].x << "\t" << vtxs[i].y << "\t" << vtxs[i].z << std::endl;
				}
				for(int i=0; i<nNotUsed; ++i){
					DSTR << "Not:\t" << notUsed[i].x << "\t" << notUsed[i].y << "\t" << notUsed[i].z << std::endl;
				}
				*/
			}
		}
	} while ( usedBits < 15 && !(v.square() < epsilon2) ) ;
	CalcPoints(usedBits, pa, pb);
	return true;
}


#define USE_FIND_CLOSEST_POINT
#ifdef USE_FIND_CLOSEST_POINT

inline bool IsDegenerate(const Vec3d& w) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1){
		if ((allUsedBits & curPoint) && (p_q[i]-w).square() < threshold2){
			return true;
		}
	}
	return false;
}

double FASTCALL FindClosestPoints(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w,
	Vec3d& v, Vec3d& pa, Vec3d& pb) {
	Vec3f p_0, q_0;
	a->Support(p_0, Vec3d());
	b->Support(q_0, Vec3d());
	v = a2w * p_0 - b2w * q_0;
	double dist = v.norm();
	Vec3d w;
	double maxSupportDist = 0.0f;

	usedBits = 0;
	allUsedBits = 0;

	int count = 0;
	while (usedBits < 15 && dist > threshold) {
		lastId = VacantIdFromBits(usedBits);
		lastBit = 1 << lastId;
		p_id[lastId] = a->Support(p[lastId], a2w.Ori().Conjugated() * (-v));
		q_id[lastId] = b->Support(q[lastId], b2w.Ori().Conjugated() * v);
		colcounter++;
		lastNormal = v;
		w = a2w * p[lastId] - b2w * q[lastId];
		double supportDist = w*v / dist;
		if (maxSupportDist < supportDist) maxSupportDist = supportDist;
		//if (dist - maxSupportDist <= dist * threshold) break;
		if (dist - maxSupportDist <= threshold) break;
		if (IsDegenerate(w)) break;
		p_q[lastId] = w;
		allUsedBits = usedBits | lastBit;
		CalcDet();
		if (!CalcClosest(v)) break;
		dist = v.norm();

		count++;
		if (count == 100) {
			//			DSTR << "Too many loop in FindClosestPoints!!" << std::endl;		
			break;
		}
	}
	CalcPoints(usedBits, pa, pb);
	return dist;
}

#endif
#endif

inline double CalcMenseki(Vec3d p1, Vec3d p2, Vec3d p3) {
	Vec3d v1 = (p2 - p1);
	Vec3d v2 = (p3 - p1);
	double menseki = (v1%v2).norm()*0.5;
	return abs(menseki);
}
inline double CalcMensekiSq(Vec3d p1, Vec3d p2, Vec3d p3) {
	Vec3d v1 = (p2 - p1);
	Vec3d v2 = (p3 - p1);
	double menseki = (v1%v2).square()*0.25f;
	return abs(menseki);
}
inline double CalcTaiseki(Vec3d p1, Vec3d p2, Vec3d p3, Vec3d p4) {
	Vec3d v1 = (p2 - p1);
	Vec3d v2 = (p3 - p1);
	Vec3d v3 = (p4 - p1);
	double vp = (v1%v2)*v3;
	return abs(vp/6.0);
}

double CalcNormal(Vec3d& n,Vec3d origin)
{
	n.clear();
	int counter = 0;
	EPATri tris[4];
	int useLength = 1;
	int bitLength = 0;

	if (usedBits >= 15) {
		origin = (p_q[0] + p_q[1] + p_q[2] + p_q[3]) / 4;
		tris[0] = EPATri(p_q[0], p_q[1], p_q[2], origin);
		tris[0].SetIdx(p[0], p[1], p[2], q[0], q[1], q[2]);
		tris[1] = EPATri(p_q[0], p_q[2], p_q[3], origin);
		tris[1].SetIdx(p[0], p[2], p[3], q[0], q[2], q[3]);
		tris[2] = EPATri(p_q[0], p_q[1], p_q[3], origin);
		tris[2].SetIdx(p[0], p[1], p[3], q[0], q[1], q[3]);
		tris[3] = EPATri(p_q[1], p_q[2], p_q[3], origin);
		tris[3].SetIdx(p[1], p[2], p[3], q[1], q[2], q[3]);
		useLength = 4;
	}
	else {
		tris[0] = EPATri(p_q[0], p_q[1], p_q[2], origin);
		tris[0].SetIdx(p[0], p[1], p[2], q[0], q[1], q[2]);
	}
	std::sort(tris, tris + useLength, [](const EPATri &left, const EPATri &right) {
		return left.distance < right.distance;
	});
	//n= tris[0].normal;
	n = -lastNormal.unit();
	return tris[0].distance;
}

Vec3d CalcContactPoint(Vec3d &pa, Vec3d &pb)
{
	Vec3d n = (p_q[1] - p_q[0]) ^ (p_q[2] - p_q[1]);
	Quaterniond n2z;
	if (n.Z() < -1 + epsilon) {
		n2z = Quaterniond::Rot(Rad(180), 'x');
	}
	else if (n.Z() < 1 - epsilon) {
		Matrix3d matW2z = Matrix3d::Rot(n, Vec3f(0, 0, 1), 'z');
		n2z.FromMatrix(matW2z);
		n2z = n2z.Inv();
	}


	Vec3d dec = TriDecompose((n2z*p_q[0]).XY(), (n2z*p_q[1]).XY(), (n2z*p_q[2]).XY());
	pa = p[0] * dec[0] + p[1] * dec[1] + p[2] * dec[2];
	pb = q[0] * dec[0] + q[1] * dec[1] + q[2] * dec[2];
	return dec;
}

inline Vec3d TriNaibun(Vec3d p1, Vec3d p2, Vec3d p3,Vec3d sep) {
	
	double m1 = CalcMenseki(sep,p1, p2);
	double m2 = CalcMenseki(sep,p1, p3);
	double m3 = CalcMenseki(sep,p2, p3);

	double alllen = m1 + m2 + m3;
	if (alllen < epsilon2) return Vec3d(0.333, 0.333, 0.333);
	Vec3d retV;
	retV.x = m3 / alllen;
	retV.y = m2 / alllen;
	retV.z = m1 / alllen;
	
	return retV;

}

//EPAで接触面をもとめる
//	const int EPAsize = 10; //EPAの三角面プール数	10だと不安定
//	const int EPAsize = 100; //EPAの三角面プール数	100なら十分
//const int EPAsize = 20;	//OK
const int EPAsize = 20;

void FASTCALL CalcEPA(Vec3d &v,const CDConvex* a,const CDConvex* b, const Posed &a2w, const Posed &b2w, Vec3d& pa, Vec3d& pb) {
	v.clear();
	int counter = 0;
	Vec3d origin;
	EPATri tris[EPAsize];
	std::vector<EPAEdge> edges;
	int useLength = 1;
	lastBit = 1;
	int useSlot[4];
	int posLength = 0;
	for (int i = 0; i < 4; i++) {
		if (usedBits & lastBit) {
			useSlot[posLength] = i;
			posLength++;
		}
		lastBit <<= 1;
	}
	
	bool keeploop = true;
	if (posLength == 4) {
		origin = (p_q[0] + p_q[1] + p_q[2] + p_q[3]) / 4.0;
		tris[0] = EPATri(p_q[0], p_q[1], p_q[2], origin);
		tris[0].SetIdx(p[0], p[1], p[2], q[0], q[1], q[2]);
		tris[1] = EPATri(p_q[0], p_q[2], p_q[3], origin);
		tris[1].SetIdx(p[0], p[2], p[3], q[0], q[2], q[3]);
		tris[2] = EPATri(p_q[0], p_q[1], p_q[3], origin);
		tris[2].SetIdx(p[0], p[1], p[3], q[0], q[1], q[3]);
		tris[3] = EPATri(p_q[1], p_q[2], p_q[3], origin);
		tris[3].SetIdx(p[1], p[2], p[3], q[1], q[2], q[3]);
		useLength = 4;
	}
	else if (posLength == 3) {
		//origin = a2w.Pos() - b2w.Pos();
		origin.clear();
		tris[0] = EPATri(p_q[useSlot[0]], p_q[useSlot[1]], p_q[useSlot[2]], origin);
		tris[0].SetIdx(p[useSlot[0]], p[useSlot[1]], p[useSlot[2]], q[useSlot[0]], q[useSlot[1]], q[useSlot[2]]);
		
		Vec3f newp, newq;
		p_id[lastId] = a->Support(newp, a2w.Ori().Conjugated() * (-tris[0].normal));
		q_id[lastId] = b->Support(newq, b2w.Ori().Conjugated() * tris[0].normal);
		Vec3d w = (a2w * (Vec3d)newp) - (b2w * (Vec3d)newq);
		origin = (w + tris[0].vert[0] + tris[0].vert[1] + tris[0].vert[2]) / 4.0;
		tris[1] = EPATri(w, tris[0].vert[0], tris[0].vert[1], origin);
		tris[1].SetIdx(newp, tris[0].aidx[0], tris[0].aidx[1], newq, tris[0].bidx[0], tris[0].bidx[1]);
		tris[2] = EPATri(w, tris[0].vert[0], tris[0].vert[2], origin);
		tris[2].SetIdx(newp, tris[0].aidx[0], tris[0].aidx[2], newq, tris[0].bidx[0], tris[0].bidx[2]);
		tris[3] = EPATri(w, tris[0].vert[1], tris[0].vert[2], origin);
		tris[3].SetIdx(newp, tris[0].aidx[1], tris[0].aidx[2], newq, tris[0].bidx[1], tris[0].bidx[2]);
		tris[0] = EPATri();
		useLength = 4;
		
		std::sort(tris, tris + useLength, [](const EPATri &left, const EPATri &right) {
			return left.distance < right.distance;
		});
		keeploop = false;
		//return;
	}
	else {
		origin = a2w.Pos() - b2w.Pos();
		origin.unitize();
		Vec3f newp, newq;
		p_id[lastId] = a->Support(newp, a2w.Ori().Conjugated() * (-origin));
		q_id[lastId] = b->Support(newq, b2w.Ori().Conjugated() * origin);
		v = (a2w * (Vec3d)newp) - (b2w * (Vec3d)newq);
		pa = newp;
		pb = newq;
		return;
	}
	Vec3d beforeNorm = tris[0].normal;
	int deleteLength = 0;
	while (keeploop)
	{
		counter++;
		//距離でソートして最短距離面を出す
		std::sort(tris, tris + useLength+deleteLength, [](const EPATri &left, const EPATri &right) {
			return left.distance < right.distance;
		});
		//規定回数以上なら脱出
		if (counter > EPAsize - 2) break;
		Vec3f newp, newq;
		p_id[lastId] = a->Support(newp, a2w.Ori().Conjugated() * (tris[0].normal));
		q_id[lastId] = b->Support(newq, b2w.Ori().Conjugated() * -tris[0].normal);
		colcounter++;
		Vec3d w = (a2w * (Vec3d)newp) - (b2w * (Vec3d)newq);
		//同じサポートが出たら抜ける →少三角形の面積で判断
		if (CalcMensekiSq(w, tris[0].vert[0], tris[0].vert[1]) < epsilon) break;
		else if (CalcMensekiSq(w, tris[0].vert[0], tris[0].vert[2]) < epsilon) break;
		else if (CalcMensekiSq(w, tris[0].vert[1], tris[0].vert[2]) < epsilon) break;
		beforeNorm = tris[0].normal;
		//wが見える面を消さないといけない
		deleteLength = 0;
		edges.clear();
		for (int i = 0; i < useLength; i++) {
			Vec3d w2plane = (w - tris[i].vert[0]);
			if (tris[i].normal*w2plane > 0) {//見える面判定
				EPAEdge addEdges[3] = { EPAEdge(tris[i].vert[0], tris[i].vert[1],tris[i].aidx[0],tris[i].aidx[1],tris[i].bidx[0],tris[i].bidx[1]) ,
					EPAEdge(tris[i].vert[0], tris[i].vert[2],tris[i].aidx[0],tris[i].aidx[2],tris[i].bidx[0],tris[i].bidx[2]) ,
					EPAEdge(tris[i].vert[1], tris[i].vert[2],tris[i].aidx[1],tris[i].aidx[2],tris[i].bidx[1],tris[i].bidx[2]) };
				for (int x = 0; x < 3; x++) {//エッジが重複するか
					bool noAdd = false;
					for (std::vector<EPAEdge>::iterator it = edges.begin(); it != edges.end();) {
						if (*it == addEdges[x]) {
							it = edges.erase(it);
							noAdd = true;
							break;
						}
						else {
							it++;
						}
					}
					if(!noAdd)edges.push_back(addEdges[x]);
				}
				tris[i] = EPATri();
				deleteLength++;
			}
		}
		if (EPAsize < useLength + edges.size()) {
			std::sort(tris, tris + EPAsize, [](const EPATri &left, const EPATri &right) {
				return left.distance < right.distance;
			});
			break;
		}
		int addCount = 0;
		for (auto edge : edges) {
			tris[useLength + addCount] = EPATri(w, edge.vert[0], edge.vert[1], origin);
			tris[useLength + addCount].SetIdx(newp, edge.aidx[0], edge.aidx[1], newq, edge.bidx[0], edge.bidx[1]);
			if ((tris[useLength + addCount].normal - beforeNorm).square() < epsilon) keeploop = false;
			addCount++;
		}
		useLength += -deleteLength+addCount;
		if (!keeploop) {
			std::sort(tris, tris + EPAsize, [](const EPATri &left, const EPATri &right) {
				return left.distance < right.distance;
			});
		}
	}
	v = tris[0].normal*tris[0].distance;
	Vec3d dec = TriNaibun(tris[0].vert[0], tris[0].vert[1], tris[0].vert[2], v);
	pa = dec[0] * (Vec3d)tris[0].aidx[0] + dec[1] * (Vec3d)tris[0].aidx[1] + dec[2] * (Vec3d)tris[0].aidx[2];
 	pb = dec[0] * (Vec3d)tris[0].bidx[0] + dec[1] * (Vec3d)tris[0].bidx[1] + dec[2] * (Vec3d)tris[0].bidx[2];
}

//Ginoの手法（GJKRaycast）

int FASTCALL ContFindCommonPointGino(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist)
{
	qpTimerForCollision.Count();
	Posed a2l = a2w;
	Posed b2l = b2w;
	dist = 0;
	if (start < -10e+3) start = -10e+3;
	double cdist = end - start;
	Vec3d r = dir*(end-start);
	a2l.Pos() += r;
//	Vec3d r = dir*start;
//	a2l.Pos() -= r;
	Vec3d tmpV;
	//近づくまで繰り返し
	int count = 0;
	int ret = 1;
	while (1) {
		count++;
		if (count > 100) break;
		//GJKの計算
		
		tmpV.clear();
		cdist = FindClosestPoints(a, b, a2l, b2l, tmpV, pa, pb);
		if (cdist < threshold) {
				if (start < 0) {
					dist += -tmpV.norm();
				}
				else {
					dist += tmpV.norm();
				}
			ret = 1;
			break;
		}

		CalcNormal(normal,a2l.Pos()-b2l.Pos());
		Vec3d w = tmpV;//wは最近点の位置
  		double vw = (normal * w);		
		double vr = (normal * r);
		if (vr > epsilon && vw > epsilon) {	ret = -2; break;}
		if (vr <= -epsilon && vw > epsilon) { ret = -1; break; }
		if (vr == 0 ) { ret = 0; break; }
		double lambda = vw/vr;
		cdist = (r*lambda).norm();
		r -= dir*cdist;	
		dist += cdist;
		if (dist > end + (end - start) + epsilon) { ret = -1; break; }
		if (dist < start) { ret = -2; break; }

		//原点をすすめる
		a2l.Pos() +=   -dir * cdist*0.5f;
		b2l.Pos() +=   dir * cdist*0.5f;
	}
	dist -= (end - start);
	qpTimerForCollision.Accumulate(coltimePhase1);
	if (normal.square() < epsilon2) return 0;
	static bool bSave = false;
	if (bSave) {
		ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, start, end, normal, pa, pb, dist, "bSaveA");
	}
		return ret;
}

//Ginoの手法2（GJKRaycast改良版＝非繰り返し版）
#if 0
int FASTCALL ContFindCommonPointGinoNew(const CDConvex* a, const CDConvex* b,
	const Posed& a2wOrg, const Posed& b2wOrg, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist)
{
	qpTimerForCollision.Count();
	double shift = 0;
	Posed a2l = a2wOrg;
	Posed b2l = b2wOrg;
	if (start < -10e+3) start = -10e+3;
	b2l.Pos() += dir*start;

	usedBits = 0;
	allUsedBits = 0;

	//近づくまで繰り返し
	int count = 0;
	int ret = 1;
	Vec3d v = dir;
	dist = v.norm();
	Vec3d w;
	int stopCount = 0;
	while (usedBits < 15) {
		lastId = VacantIdFromBits(usedBits);
		lastBit = 1 << lastId;
		p_id[lastId] = a->Support(p[lastId], a2l.Ori().Conjugated() * (-v));
		q_id[lastId] = b->Support(q[lastId], b2l.Ori().Conjugated() * v);
		colcounter++;
		Vec3d pw = a2l * p[lastId];
		Vec3d qw = b2l * q[lastId];
		w = pw - qw;
		p_q[lastId] = w;
		allUsedBits = usedBits | lastBit;
		normal = v / dist;
		double supportDist = w * normal;
		if (supportDist > 0) {	// 原点を進める
			double dirNormal = dir*normal;
			double dirDist = supportDist / dirNormal - threshold / 2;
			//DSTR << "sd:" << supportDist << " dd:" << dirDist << " dir*v:" << dir * v << " dist:" << dist << std::endl;
			shift += dirDist;
			if (shift > (end - start)) {
				return -1;
			}
			b2l.Pos() += dirDist * dir;
			for (int i = 0, iBit = 1; i < 4; ++i, iBit <<= 1) {
				if (iBit & allUsedBits) {
					p_q[i] -= dirDist * dir;
				}
			}
			stopCount = 0;
		}
		else {
			stopCount++;
			if (stopCount > 10) {
				ret = 0;
				break;
			}
		}

		CalcDetAll();
		if (!CalcClosest(v)) {
			CalcClosest(v);
			break;
		}
		dist = v.norm();
		if (dist < threshold) break;
		count++;
		if (count == 100) {
			DSTR << "Too many loop in FindClosestPoints!!" << std::endl;		
			break;
		}
	}
	if (dist > 1e-20) {
		normal = v/dist;
	}
	CalcPoints(usedBits, pa, pb);
	qpTimerForCollision.Accumulate(coltimePhase1);
	double d = dist;
	dist = start + shift;
	normal *= -1;
//	DSTR << "GRDist:" << dist << " start:"<<start << " shift:"<< shift << " d:" << d << 
//		" pd:" << (b2l.Pos()-b2wOrg.Pos()).norm() << std::endl;
	return ret;
}
#endif

const int vertexIds1[16] = {
	-1, 0, 1, -1,  2, -1, -1, -1,   3, -1, -1, -1,  -1, -1, -1, -1
};
const int vertexIds2[16][2] = {
	{ -1,-1 },{ -1,-1 },{ -1,-1 },{ 0,1 },{ -1,-1 },{ 0,2 },{ 1,2 },{ -1,-1 },
	{ -1,-1 },{ 0,3 },{ 1,3 },{ -1,-1 },{ 2,3 },{ -1,-1 },{ -1,-1 },{ -1,-1 }
};
const int vertexIds3[16][3] = {
	{ -1,-1,-1 },{ -1,-1,-1 },{ -1,-1,-1 },{ -1,-1 - 1 },{ -1,-1,-1 },{ -1,-1,-1 },{ -1,-1,-1 },{ 0,1,2 },
	{ -1,-1,-1 },{ -1,-1,-1 },{ -1,-1,-1 },{ 0,1,3 },{ -1,-1,-1 },{ 0,2,3 },{ 1,2,3 },{ -1,-1,-1 }
};
const char numVertices[16] = {
	0, 1, 1, 2, 1, 2, 2, 3,
	1, 2, 2, 3, 2, 3, 3, 4
};

//	3点がつくる三角形と原点と2点が作る三角形の面積の比から、内分比を求める。
//	内分比が正の頂点の数を返す。
int CalcTriAreas(int bits, double* areas) {
	assert(numVertices[bits] == 3);
	int unusedBit = ~bits & 0xF;
	int unusedId = -1;
	if (unusedBit & allUsedBits) {
		unusedId = vertexIds1[unusedBit];
	}
	const int* vids = vertexIds3[bits];
	Vec3d n = (p_q[vids[1]] - p_q[vids[0]]) ^ (p_q[vids[2]] - p_q[vids[0]]);
	double sign = 1;
	if (unusedId >= 0) {
		double dp = n * (p_q[unusedId] - p_q[vids[0]]);
		if (dp > 0) {
			n = -n;
			sign = -1;
		}
		double forthDist = n*p_q[vids[0]];
		if (forthDist > -1e-8) {	//	内部
			return 0;
		}
	}
	areas[3] = n.norm();
	int nPlus;
	if (areas[3] > 0) {
		n /= areas[3];
		areas[0] = (p_q[vids[1]] ^ p_q[vids[2]]) * n * sign / areas[3];
		areas[1] = (p_q[vids[2]] ^ p_q[vids[0]]) * n * sign / areas[3];
		areas[2] = (p_q[vids[0]] ^ p_q[vids[1]]) * n * sign / areas[3];
		nPlus = (areas[0] >= 0 ? 1 : 0) + (areas[1] >= 0 ? 1 : 0) + (areas[2] >= 0 ? 1 : 0);
	}
	else {
		areas[0] = areas[1] = areas[2] = 1.0 / 3;
		nPlus = 3;
	}
	return nPlus;
}



//Ginoの手法2（GJKRaycast改良版＝非繰り返し版）
int FASTCALL ContFindCommonPointGinoPrec(const CDConvex* a, const CDConvex* b,
	const Posed& a2wOrg, const Posed& b2wOrg, const Vec3d& dir, double start, double end,
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist)
{
	qpTimerForCollision.Count();
	double shift = 0;
	Posed a2l = a2wOrg;
	Posed b2l = b2wOrg;
	if (start < -10e+3) start = -10e+3;
	b2l.Pos() += dir*start;

	usedBits = 0;
	allUsedBits = 0;

	//近づくまで繰り返し
	int count = 0;
	int ret = 1;
	Vec3d v = dir;
	dist = v.norm();
	int stopCount = 0;
	int weightBit;
	double lineWeight[2];
	double* triArea;
	double areas[4][4];
	triArea = areas[0];
	while (1) {
		usedBits = allUsedBits;
		lastId = VacantIdFromBits(usedBits);
		lastBit = 1 << lastId;
		p_id[lastId] = a->Support(p[lastId], a2l.Ori().Conjugated() * (-v));
		q_id[lastId] = b->Support(q[lastId], b2l.Ori().Conjugated() * v);
		colcounter++;
		Vec3d pw = a2l * p[lastId];
		Vec3d qw = b2l * q[lastId];
		p_q[lastId] = pw - qw;
		allUsedBits = usedBits | lastBit;
		normal = v / dist;
		double supportDist = p_q[lastId] * normal;
		if (supportDist > 0) {	// 原点を進める
			double dirNormal = dir*normal;
			if (dirNormal < 0) {
				return 0;
			}
			double dirDist = supportDist / dirNormal * (1.0 - 1e-4);
			shift += dirDist;
			if (shift > (end - start)) {
				return -1;
			}
			b2l.Pos() += dirDist * dir;
			for (int i = 0, iBit = 1; i < 4; ++i, iBit <<= 1) {
				if (iBit & allUsedBits) {
					p_q[i] -= dirDist * dir;
				}
			}
			stopCount = 0;
		}
		else {
			stopCount++;
			if (stopCount > 10) {
				ret = 0;
				break;
			}
		}
		int nVtx = numVertices[allUsedBits];
		int theTriangle;	//	原点が上にのる三角形
		int theLine;	//	原点が上にのる辺
		int thePoint;	//	原点が上にのる頂点
		int triNPlus;
		if (nVtx == 4) {
			//	４点の場合、最後の１点と他のどの２点で作った三角形が一番近くなるか調べる。
			//	外向きの法線があれば、
			int nPlus[4];
			int nPlusMax=0;
			const int linesFromTri[16][3] = {
				{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},  {0,0,0}, {0,0,0}, {0,0,0}, {3,5,6},
				{0,0,0}, {0,0,0}, {0,0,0}, {3,9,10}, {0,0,0}, {5,9,12},{6,10,12}, {0,0,0},
			};
			const int* lines = linesFromTri[usedBits];
			int tris[3] = { lines[0] | lastBit, lines[1] | lastBit, lines[2] | lastBit };
			nPlus[0] = CalcTriAreas(tris[0], areas[0]);
			if (nPlus[0] > nPlusMax) { nPlusMax = nPlus[0]; }
			nPlus[1] = CalcTriAreas(tris[1], areas[1]);
			if (nPlus[1] > nPlusMax) { nPlusMax = nPlus[1]; }
			nPlus[2] = CalcTriAreas(tris[2], areas[2]);
			if (nPlus[2] > nPlusMax) { nPlusMax = nPlus[2]; }
			nPlus[3] = CalcTriAreas(usedBits, areas[3]);
			if (nPlus[3] > nPlusMax) { nPlusMax = nPlus[3]; }
			if (nPlus[0] == 0 || nPlus[1] == 0 || nPlus[2] == 0 || nPlus[3] == 0) {
				//	すでに原点がSimplexの内側→近づきすぎ→すぐ抜ける
				goto HitExit;
			}
			if (nPlusMax == 3) {	//	三角形の上に原点がある
				int i;
				for (i = 0; i < 4; ++i) {
					if (nPlus[i] == 3) {
						if (i < 3) {
							theTriangle = tris[i];
							triArea = areas[i];
							triNPlus = nPlus[i];
						} else {
							theTriangle = usedBits;
							triArea = areas[3];
							triNPlus = nPlus[3];
						}
						allUsedBits = theTriangle;
						goto TriCase;
					}
				}
				assert(0);
			}else if (nPlusMax == 2){		//	辺の上に原点
				theLine = 0xF;
				int triCount = 0;
				for (int i = 0; i < 3 && triCount < 2; ++i) {
					if (nPlus[i] == 2) {
						theLine &= tris[i];
						triCount ++;
					}
				}
				if (nPlus[3] == 2 && triCount < 2) {
					theLine &= usedBits;
				}
				if (numVertices[theLine] != 2){
					assert(numVertices[theLine] == 2);
				}
				allUsedBits = theLine;
				goto LineCase;
			}
			else if (nPlusMax == 1) {
				thePoint = 0xF;
				for (int i = 0; i < 3; ++i) {
					if (nPlus[i] == 1) {
						thePoint &= tris[i];
					}
				}
				if (nPlus[3] == 1) {
					thePoint &= usedBits;
				}
				assert(numVertices[thePoint] == 1);
				allUsedBits = thePoint;
				goto PointCase;
			}else {	//	四面体の中に原点がある
				assert(0);	//	近づけすぎ。あってはならない。
			}
		}else if (nVtx == 3){	//	三角形
			theTriangle = allUsedBits;
			triArea = areas[0];
			triNPlus = CalcTriAreas(theTriangle, triArea);
			if (triNPlus == 3) {	//三角形上
			TriCase:;
				const int* vids = vertexIds3[theTriangle];
				v = triArea[0] * p_q[vids[0]] + triArea[1] * p_q[vids[1]] + triArea[2] * p_q[vids[2]];
				weightBit = theTriangle;
			}else if (triNPlus == 2){	//辺上
				const int* vids = vertexIds3[theTriangle];
				int ids[2];
				int j = 0;
				for (int i = 0; i < 3; ++i) {
					if (triArea[i] >= 0) {
						ids[j] = vids[i];
						j++;
					}
				}
				theLine = (1 << ids[0]) | (1 << ids[1]);
				allUsedBits = theLine;
				if (j!=2){		
					assert(j == 2);
				}
				goto LineCase;
			}else if (triNPlus == 1){
				const int* vids = vertexIds3[theTriangle];
				for (int i = 0; i < 3; ++i) {
					if (triArea[i] >= 0) {
						thePoint = 1 << vids[i];
						goto PointCase;
					}
				}
				assert(0);
			}else {
				triNPlus = CalcTriAreas(theTriangle, triArea);
				assert(0);	//	inside -> 近すぎ
			}
		}
		else if (nVtx == 2) {
			theLine = allUsedBits;
			LineCase:
			const int* ids = vertexIds2[theLine];
			Vec3d line = p_q[ids[1]] - p_q[ids[0]];
			double len = line.norm();
			line /= len;
			double h0 = p_q[ids[0]] * line;
			double h1 = p_q[ids[1]] * line;
			if (h0 > 0) {
				thePoint = 1 << ids[0];
				goto PointCase;
			}
			else if (h1 > 0) {
				lineWeight[0] = h1 / (h1 - h0);
				lineWeight[1] = -h0 / (h1 - h0);
				v = p_q[ids[0]] * lineWeight[0] + p_q[ids[1]] * lineWeight[1];
				//v = (p_q[ids[0]] * h1 + p_q[ids[1]] * (-h0)) / (h1 - h0);
				weightBit = theLine;
			}
			else {
				thePoint = 1 << ids[1];
				goto PointCase;
			}
		}
		else if (nVtx == 1) {
			thePoint = allUsedBits;
			PointCase:
			int id = vertexIds1[thePoint];
			v = p_q[id];
			weightBit = thePoint;
		}
		dist = v.norm();
		if (dist < threshold) break;
		count++;
		if (count == 100) {
			DSTR << "Too many loop in FindClosestPoints!!" << std::endl;
			break;
		}
	}
	if (dist > 1e-8) {
		normal = v / dist;
	}
	HitExit:
	CalcPoints(usedBits, pa, pb);
	
	double d = dist;
	dist = start + shift;
	normal = -normal;

	Vec3d pl, ql;
	int nVtx = numVertices[weightBit];
	if (nVtx == 3) {
		const int* ids = vertexIds3[weightBit];
		pl = p[ids[0]] * triArea[0] + p[ids[1]] * triArea[1] + p[ids[2]] * triArea[2];
		ql = q[ids[0]] * triArea[0] + q[ids[1]] * triArea[1] + q[ids[2]] * triArea[2];
	}
	else if (nVtx == 2) {
		const int* ids = vertexIds2[weightBit];
		pl = p[ids[0]] * lineWeight[0] + p[ids[1]] * lineWeight[1];
		ql = q[ids[0]] * lineWeight[0] + q[ids[1]] * lineWeight[1];
	}
	else {
		int id = vertexIds1[weightBit];
		pl = p[id];
		ql = q[id];
	}
	pa = a2l * pl;
	pb = b2l * ql;
	qpTimerForCollision.Accumulate(coltimePhase1);
	//	DSTR << "GRDist:" << dist << " start:"<<start << " shift:"<< shift << " d:" << d << 
	//		" pd:" << (b2l.Pos()-b2wOrg.Pos()).norm() << std::endl;
	return ret;
}

}