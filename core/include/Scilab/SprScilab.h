#ifndef SCILAB_H
#define SCILAB_H

//	ScilabCall For Springhead2

#include <Base/TMatrix.h>
#include <complex>

namespace Scilab{

#define __INTERNAL_API_SCILAB__
//#define API_SCILAB_EXPORTS
#include "Scilab/api_scilab/includes/api_common.h"
// api_common.h中の#defineが他と干渉するのでundefする
#undef Rhs
#undef Lhs
//#undef API_SCILAB_EXPORTS
#include "Scilab/core/includes/doublecomplex.h"
#include "Scilab/core/includes/BOOL.h"
#undef __INTERNAL_API_SCILAB__
bool StartScilab(char* SCIpath, char *ScilabStartup,int *Stacksize);
#include "ScilabStub.hpp"
extern StrCtx** ppvApiCtx;
}
namespace Spr{
	///	SciLabの初期化。使用前に一度だけ呼ぶ。
	/// 例外処理をいれると一度止まる。
	inline int ScilabStart(char* SCIpath=NULL, char *ScilabStartup=NULL,int *Stacksize=NULL){
		return (int)Scilab::StartScilab(SCIpath, ScilabStartup, Stacksize);
	}
	///	SciLabの終了。
	inline bool ScilabEnd(char *ScilabQuit=NULL){ return Scilab::TerminateScilab(ScilabQuit) != 0; }
	///	SciLabに仕事を送る。 jobにはSciLabの文を文字列で渡す。
	inline int ScilabJob(const char* job){ return Scilab::SendScilabJob((char*)job); }
	///	SciLab行列を参照するための型
	typedef PTM::EMatrixCol<double> SCMatrix;
	///	SciLab行列への参照の取得。 name にScilabの文での変数名を渡す。 
	inline SCMatrix ScilabMatrix(const char* name){
		// 行列数の取得
		int n, m;
		Scilab::SciErr sciErr;
		sciErr = Scilab::readNamedMatrixOfDouble(*Scilab::ppvApiCtx, name, &m, &n, NULL);
		if(sciErr.iErr){
			//Scilab::printError(&sciErr, 0);
			std::cout << "Can not read the matrix " << name << name << std::endl;
			assert(0);
		}
		// データの参照の取得
		void* data = NULL;
		sciErr = Scilab::readNamedPointer(*Scilab::ppvApiCtx, name, &data);
		if(sciErr.iErr){
			//Scilab::printError(&sciErr, 0);
			std::cout << "Can not find the pointer of " << name << std::endl;
			assert(0);
		}
		std::cout << (double*)data << std::endl;
		SCMatrix rv(m, n, m, (double*)data);
		return rv;
	}
	///	SciLab行列の設定。 name にScilabの文での変数名を渡す。 
	template <class AD> bool ScilabSetMatrix(const char* name, PTM::MatrixImp<AD>& a){
		int m = (int)a.height();
		int n = (int)a.width();
		double* v = new double [m*n];
		for(int j=0; j<n; ++j){
			for(int i=0; i<m; ++i){
				v[j*m + i] = a.item(i, j);
			}
		}
		Scilab::SciErr sciErr;
		sciErr = Scilab::createNamedMatrixOfDouble(*Scilab::ppvApiCtx, name, m, n, v);
		delete[] v;
		return sciErr.iErr ? false : true;
	}
	///	SciLab行列のコピーの取得。 name にScilabの文での変数名を渡す。 
	template <class AD> bool ScilabGetMatrix(PTM::MatrixImp<AD>& a, const char* name){
		// 行列数の取得
		int n, m;
		Scilab::SciErr sciErr;
		sciErr = Scilab::readNamedMatrixOfDouble(*Scilab::ppvApiCtx, name, &m, &n, NULL);
		if(sciErr.iErr){
			//Scilab::printError(&sciErr, 0);
			std::cout << "Scilab has not the matrix : " << name << std::endl;
			return false;
		}
		// データのコピーを取得
		double* data = new double [m * n];
		sciErr = Scilab::readNamedMatrixOfDouble(*Scilab::ppvApiCtx, name, &m, &n, data);
		if(sciErr.iErr){
			//Scilab::printError(&sciErr, 0);
			std::cout << "Scilab has not the matrix : " << name << std::endl;
			return false;
		}
		SCMatrix rv(m, n, m, (double*)data);
		// 行列のコピー
		a.assign(rv);
		delete[] data;
		return sciErr.iErr ? false : true;
	}
}
#endif
