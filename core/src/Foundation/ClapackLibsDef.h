#ifndef CLAPACK_LIBS_DEF_H
#define CLAPACK_LIBS_DEF_H

#ifdef USE_LAPACK

#define BOOST_NUMERIC_BINDINGS_USE_CLAPACK
#if defined _MSC_VER
#  pragma warning(push)
#  pragma warning(disable:4267)
#  pragma warning(disable:4005)
#endif

//#include <boost/numeric/ublas/fwd.hpp>
#include <boost/numeric/bindings/lapack/driver/sygv.hpp>
#include <boost/numeric/bindings/lapack/driver/sygvx.hpp>
#include <boost/numeric/bindings/lapack/driver/gesv.hpp>
#include <boost/numeric/bindings/lapack/driver/gels.hpp>
#include <boost/numeric/bindings/lapack/driver/gelsd.hpp>
#include <boost/numeric/bindings/lapack/driver/gesdd.hpp>
#include <boost/numeric/bindings/noop.hpp>
#include <boost/numeric/bindings/ublas/banded.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/ublas/symmetric.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/vector_proxy.hpp>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#if defined(_MSC_VER)

// for DLL
#if defined(_DLL)

  // Trace
  #if defined(TRACE)
    #if (_MSC_VER == 1900)	// Visual Studio 2015
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c14.0Tx64.lib")
        #pragma comment(lib, "blas14.0Tx64.lib")
        #pragma comment(lib, "clapack14.0Tx64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c14.0TWin32.lib")
        #pragma comment(lib, "blas14.0TWin32.lib")
        #pragma comment(lib, "clapack14.0TWin32.lib")
      #endif
    #elif (_MSC_VER >= 1910)	// Visual Studio 2017
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c15.0Tx64.lib")
        #pragma comment(lib, "blas15.0Tx64.lib")
        #pragma comment(lib, "clapack15.0Tx64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c15.0TWin32.lib")
        #pragma comment(lib, "blas15.0TWin32.lib")
        #pragma comment(lib, "clapack15.0TWin32.lib")
      #endif
    #else	// _MSC_VER
	// add code here for new version
    #endif	// _MSC_VER

  // Debug
  #elif defined(_DEBUG)
    #if (_MSC_VER == 1900)	// Visual Studio 2015
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c14.0Dx64.lib")
        #pragma comment(lib, "blas14.0Dx64.lib")
        #pragma comment(lib, "clapack14.0Dx64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c14.0DWin32.lib")
        #pragma comment(lib, "blas14.0DWin32.lib")
        #pragma comment(lib, "clapack14.0DWin32.lib")
      #endif
    #elif (_MSC_VER >= 1910)	// Visual Studio 2017
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c15.0Dx64.lib")
        #pragma comment(lib, "blas15.0Dx64.lib")
        #pragma comment(lib, "clapack15.0Dx64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c15.0DWin32.lib")
        #pragma comment(lib, "blas15.0DWin32.lib")
        #pragma comment(lib, "clapack15.0DWin32.lib")
      #endif
    #else	// _MSC_VER
	// add code here for new version
    #endif	// _MSC_VER

  // Release
  #else
    #if (_MSC_VER == 1900)	// Visual Studio 2015
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c14.0x64.lib")
        #pragma comment(lib, "blas14.0x64.lib")
        #pragma comment(lib, "clapack14.0x64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c14.0Win32.lib")
        #pragma comment(lib, "blas14.0Win32.lib")
        #pragma comment(lib, "clapack14.0Win32.lib")
      #endif
    #elif (_MSC_VER >= 1910)	// Visual Studio 2017
      #if defined(_WIN64)	//	for x64
        #pragma comment(lib, "libf2c15.0x64.lib")
        #pragma comment(lib, "blas15.0x64.lib")
        #pragma comment(lib, "clapack15.0x64.lib")
      #else			//	for x86
        #pragma comment(lib, "libf2c15.0Win32.lib")
        #pragma comment(lib, "blas15.0Win32.lib")
        #pragma comment(lib, "clapack15.0Win32.lib")
      #endif
    #else	// _MSC_VER
	// add code here for new version
    #endif	// _MSC_VER
  #endif // TRACE

#else	// _DLL
  // add code here for static version
#endif	// _DLL

#endif	// _MSC_VER

#endif	//USE_LAPACK
#endif	//CLAPACK_LIBS_DEF_H
