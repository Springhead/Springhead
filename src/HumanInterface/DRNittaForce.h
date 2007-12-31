/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DEVICE_DRNITTAFORCE_H
#define DEVICE_DRNITTAFORCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HumanInterface/HIRealDevice.h>
#include "DVForceBase.h"
#include <base/Affine.h>

namespace Spr {


class SPR_DLL DRNittaForce: public HIRealDevice{
public:
	///	仮想デバイス
	class VirtualDevice:public DVForceBase{
	protected:
		DRNittaForce* realDevice;
	public:
		VirtualDevice(DRNittaForce* r):realDevice(r){}
		virtual HIRealDeviceIf* RealDevice() { return realDevice->Cast(); }
		virtual const char* Name() const{ return realDevice->Name(); }
		virtual int GetDOF() const { return 6; }
		virtual float GetForce(int ch) const { return realDevice->GetForce(ch); }
	};

	enum{
		count1_addr			=	0xe8,
		count2_addr			=	0xe9,
		count3_addr			=	0xea,
		count4_addr			=	0xeb,
		count5_addr			=	0xec,
		count6_addr			=	0xed,
		force_units_addr	=	0xfc,
		full_fx_addr		=	0x80,
		force_addr			=	0x90,
		fx0_addr			=	0x90,
		full_fy_addr		=	0x81,
		fy0_addr			=	0x91,
		full_fz_addr		=	0x82,
		fz0_addr			=	0x92,
		full_mx_addr		=	0x83,
		mx0_addr			=	0x93,
		full_my_addr		=	0x84,
		my0_addr			=	0x94,
		full_mz_addr		=	0x85,
		mz0_addr			=	0x95,
		software_ver_no_addr=	0xf5,
		serial_no_addr		=	0xf8,
		model_no_addr		=	0xf9,
		DOF					=	6
	};
protected:
	char name[100];
	unsigned int baseAdr;
	unsigned int boardNumber;
	unsigned short software_ver_no;
	unsigned short serial_no;
	unsigned short model_no;
	unsigned short full_range[DOF];
	unsigned short units;
	short force[DOF];

public:
	///
	DRNittaForce(int boardNum);
	///
	virtual ~DRNittaForce();
	///	初期化
	virtual bool Init();
	///	登録
	virtual void Register(HIVirtualDevicePool& vpool);
	///	名前
	virtual const char* Name() const { return name; }
	///	ボード上のタイマーの読み出し
	unsigned short GetCounter2K();
	///	ボード上のタイマーの読み出し
	unsigned short GetCounter8K();
	///	タイマーなどのリセット
	void Reset();
	///	力の読み出し
	float GetForce(int ch){
		if (ch > DOF-1) ch = DOF-1;
		if (ch < 0) ch = 0;
		ReadForce(ch);
		return Force(ch);
	}
protected:
	///	データをニュートンに変換
	float DRNittaForce::Force(int num){
		return (float)force[num]/(float)16384*(float)full_range[num];
	}
	///	力の取得
	void ReadForce(int ch);
	///	レジスタの読み出し
	void ReadReg(unsigned short a, unsigned short* d);
	void WriteReg(unsigned short a, unsigned short d);
};

}	//	namespace Spr

#endif
