// $(RUBY_INCLUDE)/win32/win32.h, windows.hのマクロ定義との衝突回避
%{
#undef write			// std
#undef read				// std
#undef CreateWindow		// FWAppGLUT
%}

// 型キャストのWarningを無効化
%{
#ifdef _MSC_VER
# pragma warning(disable: 4311 4312)
#endif	
%}
