/*
 This file is part of the VectorNav Support Library.
 
 The VectorNav Support Library is free software: you can redistribute it and/or modify
 it under the terms of the Lesser GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 The VectorNav Support Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 Lesser GNU General Public License for more details.
 
 You should have received a copy of the Lesser GNU General Public License
 along with the VectorNav Support Library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file Macros.h
 \brief Useful macros for checking return values
 
 */

#ifndef PE_H_
#define PE_H_

#include <errno.h>
#include <stdio.h>
#include <string.h> // for strerror()
#include <stdlib.h> // for exit()

extern int PEReturnValue; 

// The PE macro is for wrapping functions to detect whether a function call
//	returns -1 to indicate an error.  If -1 is returned, an error message
//  including strerror() is inserted on stderr.
//  Program continues to execute
#define PE(function) (((PEReturnValue = (function)) == -1) ? \
( \
(fprintf(stderr, "\n\n**** Function invocation: " #function \
				 "\n at line: %d" \
				 "\n of file: " __FILE__ \
				 "\n resulted in error: %s\n", __LINE__, strerror(errno))), -1) : PEReturnValue)

// The PE_NOT macro is like PE but is intended for situations where a 
// specific return value is required.
#define PE_NOT(function, desiredRv) (((PEReturnValue = (function)) != desiredRv) ? \
	( \
	(fprintf(stderr, "\n\n**** Function invocation: " #function \
		"\n at line: %d" \
		"\n of file: " __FILE__, __LINE__ )), \
	((PEReturnValue == -1) ? \
		(fprintf(stderr, "\n resulted in error: %s", strerror(errno))) : \
		(fprintf(stderr, "\n returned: %d" \
			"\n instead of: %d", (PEReturnValue), (desiredRv)))), \
	(fprintf(stderr, "\n")), \
	0,0) : \
	PEReturnValue)

#endif /*PE_H_*/
