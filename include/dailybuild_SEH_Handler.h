#ifndef SEH_HANDLER_H
#define SEH_HANDLER_H

#include <stdexcept>

// Class for containing information on a Win32 Structured Exception
class SEH_Exception : public std::exception {
private:
	SEH_Exception() {}
	unsigned int _code;
	void* _ep;
public:
	SEH_Exception(unsigned int code, void* ep) : _code(code), _ep(ep) {}
	~SEH_Exception() {}
	virtual const int exitcode() const;
};
const int SEH_Exception::exitcode() const {
	const int SEH_INTR = -999;
	return SEH_INTR;
}
//typedef void (*_se_translator_function)(unsigned int, struct _EXCEPTION_POINTERS* );
void __cdecl se_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	SEH_Exception se(code, ep);
	throw se;
}

#ifdef	DAILYBUILD
  #define SEH_HANDLER_DEF \
	_se_translator_function se_trans = _set_se_translator(se_translator);
  #define SEH_HANDLER_TRY try {
  #define SEH_HANDLER_CATCH } catch (SEH_Exception e) { exit(e.exitcode()); }
#else
  #define SEH_HANDLER_DEF
  #define SEH_HANDLER_TRY
  #define SEH_HANDLER_CATCH
#endif

#endif //SEH_HANDLER_H
