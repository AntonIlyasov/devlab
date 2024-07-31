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

/*! \file VectorNav.h
	\brief Header file to include in your project.
	
	This library contains all data structures and functions necessary to communicate with the VN-100 Sensor.
 */


/*! \mainpage VectorNav Support Library
 \section intro_sec Introduction
 
 The VectorNav Support Library is a community supported C-based API that is designed to communicate with products (particularly the VN-100) developed by VectorNav Technologies. 
 It is freely available to the public, and it’s licensed under the LGPLv3.\n\n
 
 The sample program "vntest.c" shows you how some of the functions are used. Please see the sample program for a quick start on how to use the library.
 
 */


#ifndef VECTORNAV_H
#define VECTORNAV_H

#include "VNDefines.h"
#include "VNStructs.h"
#include "VNChecksum.h"
#include "VNWrite.h"
#include "VNRead.h"
#include "VNControl.h"
#include "Macros.h"

#ifndef MINGW
#include "VNSetup.h"
#endif

#endif
