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

/*! \file VNWrite.h
 \brief All functions that send commands to the VN-100.
 
 These funtions send commands to the VN-100. They conform to the VN-100 User Manual.
 NOTE: Do not call these functions while you are gathering asynchronous data. You will lose a few lines of data if you do.
 If you must call any of these functions, do so prior to colecting asynchronous data. This will be fixed in a future version.
 */


#ifndef VNWRITE_H
#define VNWRITE_H
#include "VNDefines.h"
#include "VNStructs.h"
#include "VNChecksum.h"
#include "VNControl.h"

/*! \fn int VNS_UART_tareDevice(struct DeviceControl *control)
 
 \brief Tares the VN-100 sensor.
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_tareDevice(struct DeviceControl *control);

/*! \fn int VNS_UART_restoreFactorySettings(struct DeviceControl *control)
 
 \brief Resests the VN-100 to factory defaults.
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_restoreFactorySettings(struct DeviceControl *control);

/*! \fn int VNS_UART_resetDevice(struct DeviceControl *control)
 
 \brief Resests the VN-100.
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_resetDevice(struct DeviceControl *control);

/*! \fn int VNS_UART_writeSettings(struct DeviceControl *control)
 
 \brief Writes the current settings of the VN-100 to non-volatile memory.
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_writeSettings(struct DeviceControl *control);

/*
 See VNWrite.c to see why setBaudRate is disabled
 
int setBaudRate(struct DeviceControl *control, int baudRate);
*/

/*! \fn int VNS_UART_setAsyncOutputFrequency(struct DeviceControl *control, int frequency)
 
 \brief Sets the output frequency on the VN-100
 
 Sets the frequency for the asynchronous output. Each baud rate has a limit to what frequency can be used. You will need to change the baud rate
 if you are trying to use a higher frequency.
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param frequency Frequency you wish the VN-100 to use
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_setAsyncOutputFrequency(struct DeviceControl *control, int frequency);

/*! \fn int VNS_UART_setAsyncOutputType(struct DeviceControl *control, int outputType)
 
 \brief Sets the asynchronous output on the VN-100
 
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param outputType Output you wish the VN-100 to use
 \return VNS_SUCCESS for success, VNS_ERROR_COM for failure
 */
int VNS_UART_setAsyncOutputType(struct DeviceControl *control, int outputType);

/*! \fn void VNS_UART_commonMessageSend(struct DeviceControl *control, int timeOut, int type, char *message, struct returnPacket *rPacket)
 
 \brief Sends a message to the VN-100
 
 This functions is used internally, and isn't meant to be used by the application designer.\n
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param timeOut Specifies a time in seconds to look for the return value.
 \param type Length of the message to send.
 \param message The message to be sent.
 \param rPacket structure containing the return value from the VN-100.
 */
void VNS_UART_commonMessageSend(struct DeviceControl *control, int timeOut, int type, char *message, struct returnPacket *rPacket);

/*! \fn int VNS_UART_getReturnValue(struct DeviceControl *control, char *expectedMessage, struct returnPacket *rPacket)
 
 \brief Parses the return value from the VN-100
 
 This functions is used internally, and isn't meant to be used by the application designer.\n
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param expectedMessage If the return length of the message is already known, the performace in parsing the return value can be improved.
 \param rPacket structure containing the return value from the VN-100.
 \return Return value from function call.
 */
int VNS_UART_getReturnValue(struct DeviceControl *control, char *expectedMessage, struct returnPacket *rPacket);

#endif
