/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Collision/CDShape/main.cpp

【概要】
 連続接触判定のテスト
  
【終了基準】
  ・プログラムが正常終了したら0を返す。
      
 */
#include <iostream>
#include <Springhead.h>		//	Springheadのインタフェース
#include <Collision/CDDetectorImp.h>
#include <math.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
namespace Spr {
	void setGjkThreshold(double th, double e);
	extern int contFindCommonPoint3DRefinementCount;
}
using namespace Spr;

std::string texText(double d) {
	int e = log10(d) - 1;
	double f = round( d / pow(10, e) *10 );
	char buf[256];
	snprintf(buf, sizeof(buf), "$ %1.1f \\times 10^{%d} $", f/10, e);
	return std::string(buf);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return	0 (正常終了)
 */
int SPR_CDECL main(int argc, char* argv[]) {
	UTRef<PHSdkIf> sdk = PHSdkIf::CreateSdk();				//	SDKの作成
	PHSceneIf* scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	PHSolidIf* solid = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	const bool SHOWEACH = true;

	//	形状の作成
	CDSphereDesc sda, sdb;
	CDBoxDesc bda;
	sda.radius = 0.1;
	bda.boxsize = Vec3d(1, 1, 1) * sda.radius * 2;
	sdb.radius = 0.001;

#define BOX


#ifdef BOX
	CDConvexIf* sa = sdk->CreateShape(bda)->Cast();
#else
	CDConvexIf* sa = sdk->CreateShape(sda)->Cast();
#endif
	CDConvexIf* sb = sdk->CreateShape(sdb)->Cast();
#ifdef BOX
	Posed poseA(Vec3d(-sda.radius, sda.radius, 0)), poseB(Vec3d(sdb.radius, 0, 0));
#else
	Posed poseA(Vec3d(-sda.radius, 0, 0)), poseB(Vec3d(sdb.radius, 0, 0));
#endif
	std::ostringstream ostrPR;
	std::ostringstream ostrGJK;
	for (double thres = 4; thres <= 15; thres+= 1) {
		setGjkThreshold(pow(10, -thres), pow(10, -12));
		double distESq1 = 0, normalESq1 = 0;
		double distESq2 = 0, normalESq2 = 0;
		double distEMax1 = 0, distEMax2 = 0;
		double normalEMax1 = 0, normalEMax2 = 0;
		double nIter = 0;
		int count = 0;
		Posed poseA2 = poseA;
		poseA2.Pos() += Vec3d(-0.01, 0, 0);
		for (double theta = M_PI*(-89.0 / 180.0); theta < M_PI*(+89.5 / 180.0); theta += M_PI / 180) {
			//if (abs(theta - M_PI*(-82.0 / 180.0)) > 0.001) continue;
			Vec3d dir = Vec3d(cos(theta), sin(theta), 0);	//	Aが動く向き
			//poseA.Pos() -= dir;
			Vec3d normal1, normal2, posA, posB;
			double dist1, dist2;
			double dist = 0;
#if 1
			//	Aに対するBの速度なので、-dir
			ContFindCommonPoint(sa->Cast(), sb->Cast(), poseA, poseB, -dir, -10, 1, normal1, posA, posB, dist1);
			nIter += contFindCommonPoint3DRefinementCount;
			double distE1 = dist1 - dist;
			double normalE1 = acos(normal1*Vec3d(1, 0, 0));
			if (SHOWEACH) DSTR << "Theta = " << theta * 180 / M_PI << std::endl;
			if (SHOWEACH) DSTR << "PR: dist:" << dist1 << " E:" << distE1 << " normal:" << normal1 << "E:" << normalE1 << std::endl;
			distESq1 += abs(distE1);
			normalESq1 += abs(normalE1);
			if (abs(distE1) > distEMax1) distEMax1 = abs(distE1);
			if (abs(normalE1) > normalEMax1) normalEMax1 = abs(normalE1);
#endif
#if 1
			ContFindCommonPointGinoPrec(sa->Cast(), sb->Cast(), poseA, poseB, -dir, -10, 1, normal2, posA, posB, dist2);
#else
			FindClosestPoints(sa->Cast(), sb->Cast(), poseA2, poseB, normal2, posA, posB);
			dist2 = (posA - posB).norm();
			normal2 = (posA - posB).unit();
#endif
			double distE2 = dist2 - dist;
			double normalE2 = acos(normal2*Vec3d(1, 0, 0));
			if (SHOWEACH) DSTR << "GR: dist:" << dist2 << " E:" << distE2 << " normal:" << normal2 << "E:" << normalE2 << (normalE2 > 0.5? "BIG ERROR" : "") << std::endl;
			distESq2 += abs(distE2);
			normalESq2 += abs(normalE2);
			if (abs(distE2) > distEMax2) distEMax2 = abs(distE2);
			if (abs(normalE2) > normalEMax2) normalEMax2 = abs(normalE2);
			count++;
		}
		distESq1 /= count;
		distESq2 /= count;
		normalESq1 /= count;
		normalESq2 /= count;
		nIter /= count;
		DSTR << std::endl;
#ifdef BOX
		DSTR << "box " << sdb.radius << std::endl;
#else
		DSTR << "sphere " << sdb.radius << std::endl;
#endif
		DSTR << "PR 10^{-" << thres << "}  " << distESq1 << "(" << distEMax1 << ")  "
			<< texText(normalESq1) << "(" << normalEMax1 << ")" << "n=" << nIter << std::endl;
		DSTR << "GJK10^{-" << thres << "}  " << distESq2 << "(" << distEMax2 << ")  "
			<< texText(normalESq2) << "(" << normalEMax2 << ")" << std::endl;


		ostrPR << "PR  & $10^{-" << thres << "}$ & " << texText(distESq1) << " & " << texText(distEMax1) << " & " 
			<< texText(normalESq1) << " & " << texText(normalEMax1) << /*" n=" << nIter << */ " \\\\"  << std::endl;
		ostrGJK << "GJK & $10^{-" << thres << "}$ & " << texText(distESq2) << " & " << texText(distEMax2) << " & "
			<< texText(normalESq2) << " & " << texText(normalEMax2) << " \\\\" << std::endl;
	}
	DSTR << "---------------------------" << std::endl;
	std::ofstream ofs("out.txt");
	ofs << ostrPR.str();
	ofs << ostrGJK.str();
	return 0;
}
