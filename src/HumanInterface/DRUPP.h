/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UPP_H
#define UPP_H
#include <Base/BaseDebug.h>
#include <vector>

namespace Spr {

/**	UPP(Universal Pulse Processor)のドライバー.
	CRatorUppCard, CPCCardDriverが使う	*/
class SPR_DLL DRUpp{
public:
	class SPR_DLL UppFunc
	{
	public:
		///	UPPの命令語
		enum {	FRS = 0x00, INS = 0x10, UDS = 0x20, GTS = 0x30,
			FRC = 0x40, INC = 0x50, PWC = 0x60, OSC = 0x70,
			FFC = 0x80, TPC = 0x98, GTC = 0xA0, CTO = 0xB0,
			SIT = 0xC0, SOT = 0xD0, SPO = 0xE0, NOP = 0xF0};
		///	UPPの命令語のオプション
		enum {};
		///	端子のエッジ検出
		enum {};
		enum {IDLEN = 20};
		///	命令を識別するためのID、特定の命令を命令表から削除するために用いる
		char id[IDLEN+1];
			
		///	UPP命令
		int cmd, ar, br, ioa, iob, ioc, iod;
	
		/**	コンストラクタ.
		@param aCmd		命令
		@param anAr		Aレジスタ
		@param anBr		Bレジスタ
		@param anIoa	端子A
		@param anIob	端子B
		@param anIoc	端子C
		@param anIod	端子D
		@param anId		識別文字列(省略可)	*/
		UppFunc(int aCmd = NOP, int anAr = 0, int anBr = 0,
			int anIoa = 0, int anIob = 0, int anIoc = 0, int anIod = 0, char* anId = "");
		bool operator ==(const UppFunc& f) const;
		bool operator <(const UppFunc& f) const;
		bool operator >(const UppFunc& f) const;
		bool operator !=(const UppFunc& f) const;
	};
protected:
	///	UPP内部レジスタのアドレス
	enum {PORT1 = 0x3, PORT2 = 0x2, UDR_BASE=0x40};
	///	初期化済みなら true.
	bool bInitalized;
	///	FNRの表

	static int FNR[17];
	///	UPPの命令表をUPPに設定する
	void SetFunc();
	///	UPPの命令表

	std::vector<UppFunc> func;
public:
	/// 
	DRUpp();
	/// 
	virtual bool Init();
	///	UPPのレジスタのアドレス a に値 v を書きこむ
	virtual void Out(unsigned a, unsigned v)=0;
	///	UPPのレジスタのアドレス a の値を読み出す
	virtual int In(unsigned a)=0;
	///	UPPの実行状態の取得
	bool	Run();
	/**	UPPの状態を設定する.
	@param b 設定したい状態. true:実行状態 false:停止状態	*/
	void	Run(bool b);
	///	命令表に命令を追加する
	void	Add(const UppFunc& f);
	///	命令表から命令を削除する
	void	Del(char* id);
	///	命令表から命令を削除する
	void	Del(int id);
	
	/**	UPPの端子を使用するユニットの設定.
	1bit目がポート0に、16bit目がポート0に対応する。
    @verbatim
	UPPの端子とビットの割り当て
	0xABCD
	dir  MSB 0x   A    B    C    D LSB
	port       FEDC BA98 7654 3210
	@endverbatim
	0: PIOが使用	1: UPPが使用	*/
	void	PortOwner(unsigned owner = 0xFFFF);
	/**	UPPの端子の入出力の設定.
	1bit目がポート0に、16bit目がポート0に対応する。
    @verbatim
	UPPの端子とビットの割り当て
	0xABCD
	dir  MSB 0x   A    B    C    D LSB
	port       FEDC BA98 7654 3210
	@endverbatim
	0: 入力	1: 出力	*/
	void	PortDir(unsigned dir = 0x0000);
	///	PIOの値を入力(PortOwnerがPIOになっている端子のみ使用できる)
	unsigned short PioRead();
	///	PIOへ値を出力(PortOwnerがPIOになっている端子のみ使用できる)
	void	PioWrite(unsigned short d);
	/**	UPP Data Register の値を設定.
	@param	no	UDRの番号(0-23)
	@param	v	値	*/
	void	UdrWrite(unsigned no,unsigned short v);
	/**	UPP Data Register の値の読み出し.
	@param	no	UDRの番号(0-23)
	@return		値	*/
	unsigned short	UdrRead(unsigned no);
};
}	//	namespace Spr
#endif
