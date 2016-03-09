// SprCSUtility.h
#ifndef CSUTILITY_H
#define CSUTILITY_H

#include <stdexcept>

// Class for containing information on a Win32 Structured Exception
class SEH_Exception: public std::exception {
private:
	SEH_Exception() {}
	unsigned int _code;
	void* _ep;
public:
	SEH_Exception(unsigned int code, void* ep) : _code(code), _ep(ep) {}
	~SEH_Exception() {}
	virtual const char* what() const throw();
	virtual const char* trace() const;
};

#endif //CSUTILITY_H
