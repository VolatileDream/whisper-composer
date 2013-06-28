#ifndef _ILLEGAL_ARGS_EXCEPTION_
#define _ILLEGAL_ARGS_EXCEPTION_

#include <exception>

class IllegalArgumentsException : public std::exception {
	virtual const char* what() const throw() {
		return "Function passed illegal arguments.";
	}
};

#endif