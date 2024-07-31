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

/*! \file VNSetup.h
 \brief VectorNav Initialization.
 
 Initializes the VN-100. Only compile this module in if you have a system that includes termios.s
 */

#ifndef VNSETUP_H
#define VNSETUP_H

#include "VNStructs.h"
#include <termios.h>

/*! \fn int VNS_UART_initDevice(char *serialDevice, struct DeviceControl *control, int deviceType, speed_t baudRate)
 
 \brief Initializes the VN-100
 
 Connects the control structure to the serial device that is connected to the VN-100.
 Nearly all functions require you pass in a structure that points to a VN-100.
 This function can read from the actual hardware or from a text file contain data from
 a VN-100.
 \n\n
 This Function will only be available to you if you are using a system that supports termios.h
 You will have to initialize the structure yourself otherwise. (see the sample vntest.c program).
 
 \param control The control structure that will point to the VN-100.
 \param deviceType VNDEVICE for the VN-100, VNFILE for a text file.
 \param baudRate Baud rate to set the serial port at (must match VN-100's baud rate).
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure
 */
int VNS_UART_initDevice(char *serialDevice, struct DeviceControl *control, int deviceType, speed_t baudRate);

/*! \fn int VNS_UART_setDeviceBaudrate(struct DeviceControl *control, speed_t baudRate)
 
 \brief Set the baud rate on the serial node
 
 Sets the baud rate on the serial device that the VN-100 is connected to.
 This does NOT set the baud rate on the VN-100. This function is mainly meant to be used internally
 by the library.
 
 \param control The control structure that points to the VN-100.
 \param baudRate Baud rate to set on the serial node.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure
 */
int VNS_UART_setDeviceBaudrate(struct DeviceControl *control, speed_t baudRate);

/*! \fn int VNS_UART_shutdown(struct DeviceControl *control)
 
 \brief Uninitializes the VN-100
 
 Closes the file descriptor that's connected to the VN-100. Anymore function calls that use
 the control structure will fail.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure
 */
int VNS_UART_shutdown(struct DeviceControl *control);
#endif
