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

#ifndef VNCONTROL_H
#define VNCONTROL_H
#include "VNDefines.h"
#include "VNStructs.h"
#include <unistd.h>

/*! \file VNControl.h
 \brief Functions to set the sleep delay, find error meanings and buffer drains
 
 */

/*! \fn int VNS_UART_setSleepDelay(struct DeviceControl *control, useconds_t sleepDelay)
 
 \brief Sets the amount of time to sleep if the serial buffer doesn't contain a full line of data.
 
 The sleep delay is used to sleep the parent program for a specified number of microseconds.
 The asyncronousCapture function calls the POSIX read function multiple times if it cannot read in exactly
 what the user has requested. A short sleep should ensure that the next call to POSIX read will read in
 what the user has requested. There may be many sleep calls as the computer will read in data from the buffer
 faster than the VN-100 can transmit.\n\n
 
 In order to use this feature, the VectorNav Support Library must be compiled with -DVNSLEEPDELAY.
 Without it, setting the sleep delay will be useless and will have no effect.
 
 \param control The control structure that points to the VN-100.
 \param sleepDelay Amount of time to sleep for in microseconds.
 \return VNS_SUCCESS for succesfully setting the delay, VNS_ERROR_COM for failure.
 */
int VNS_UART_setSleepDelay(struct DeviceControl *control, useconds_t sleepDelay);

/*! \fn const char* VNS_UART_VNStrerror(int errornum)
 
 \brief Returns a pointer to a string explaining the error.
 
 All functions that read/write to the VN-100 have a return value. If You get an error, 
 you can pass the return value to this function in order to get an explanation of the error you received.
 The string returned cannot be modified.
 
 \param errornum The error number you received from the VN-100.
 \return A constant pointer to a string explaining the error encountered.
 */
const char* VNS_UART_VNStrerror(int errornum);

/*! \fn int VNS_UART_drainBuffer(struct DeviceControl *control)
 
 \brief Drains the serial buffer.
 
 This function will drain the buffer so that no data is left inside the buffer. This function is used internally and generally shouldn't
 be used by the programmer. This function is called when the user decides to turn off asyncronous output by calling setAsynchOutputType
 with the OFF parameter.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure.
 */
#ifndef MINGW
int VNS_UART_drainBuffer(struct DeviceControl *control);
#endif
#endif
