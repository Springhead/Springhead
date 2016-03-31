// SprCSUtility.h
#ifndef CSUTILITY_H
#define CSUTILITY_H

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
	virtual const char* what() const throw();
	virtual const char* trace() const;
	virtual void raise_managed_exception(char* msg) const throw();
};

// for execution tracking ...
class CSlog {
public:
	static void VPrint(const char* formt, ...);
	static void Print(const char* str);
	static void Truncate();
};

#endif //CSUTILITY_H
