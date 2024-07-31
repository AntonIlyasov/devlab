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

/*! \file VNRead.h
 \brief All functions that retrieve data from the VN-100.
 
 These functions retrieve data from the VN-100. They are equivalent to the commands offered by
 VectorNav, with the addition of an asynchronous capture function.\n
 NOTE: Do not call these functions (other than readData()) while you are gathering asynchronous data. You will lose a few lines of data if you do.\n
 If you must call any of these functions, do so prior to colecting asynchronous data. This will be fixed in a future version.
 */

#ifndef VNREAD_H
#define VNREAD_H
#include "VNDefines.h"
#include "VNStructs.h"
#include "VNChecksum.h"
#include "VNWrite.h"

/*! \fn int VNS_UART_getHardwareRevision(struct DeviceControl *control)
 
 \brief Gets the hardware revision number from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return The hardware revision number. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getHardwareRevision(struct DeviceControl *control);

/*! \fn int VNS_UART_getFirmwareVersion(struct DeviceControl *control)
 
 \brief Gets the firmware version number from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return The firmware version number. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getFirmwareVersion(struct DeviceControl *control);

/*! \fn int VNS_UART_getSerialNumber(struct DeviceControl *control, struct SerialNumber *serialNumber)
 
 \brief Gets the serial number from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param serialNumber Structure to store the serial number in.
 \return The firmware version number. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getSerialNumber(struct DeviceControl *control, struct SerialNumber *serialNumber);

/*! \fn int VNS_UART_getModelNumber(struct DeviceControl *control, struct ModelNumber *modelNumber)
 
 \brief Gets the model number from the VN-100 and copies it to the modelNumber structure
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param modelNumber The structure to copy the serial number to.
 \return 0 for success. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getModelNumber(struct DeviceControl *control, struct ModelNumber *modelNumber);

/*! \fn int VNS_UART_getAsyncOutputType(struct DeviceControl *control)
 
 \brief Gets the output type from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return The integer type of the output data. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getAsyncOutputType(struct DeviceControl *control);

/*! \fn int VNS_UART_getAsyncOutputFrequency(struct DeviceControl *control)
 
 \brief Gets the output frequency from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \return The output frequency of the VN-100. Otherwise VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getAsyncOutputFrequency(struct DeviceControl *control);

/*! \fn int VNS_UART_getYawPitchRoll(struct DeviceControl *control, struct Attitude *ypr)
 
 \brief Gets the Yaw Pitch Roll from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param ypr Structure to store the Yaw Pitch Roll obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getYawPitchRoll(struct DeviceControl *control, struct Attitude *ypr);

/*! \fn int VNS_UART_getQuaternion(struct DeviceControl *control, struct Quaternion *quaternion)
 
 \brief Gets the Quaternion from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternion Structure to store the quaternion obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternion(struct DeviceControl *control, struct Quaternion *quaternion);

/*! \fn int VNS_UART_getQuaternionMagnetic(struct DeviceControl *control, struct QuaternionMagnetic *quaternionMagnetic)
 
 \brief Gets the Quaternion and Magnetic reading from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternionMagnetic Structure to store the quaternion and magnetic reading obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternionMagnetic(struct DeviceControl *control, struct QuaternionMagnetic *quaternionMagnetic);

/*! \fn int VNS_UART_getQuaternionAcceleration(struct DeviceControl *control, struct QuaternionAcceleration *quaternionAcceleration)
 
 \brief Gets the Quaternion and Acceleration reading from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternionAcceleration Structure to store the quaternion and acceleration reading obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternionAcceleration(struct DeviceControl *control, struct QuaternionAcceleration *quaternionAcceleration);

/*! \fn int VNS_UART_getQuaternionAngularRate(struct DeviceControl *control, struct QuaternionAngularRate *quaternionAngularRate)
 
 \brief Gets the Quaternion and Angular Rate reading from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternionAngularRate Structure to store the quaternion and angular rate reading obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternionAngularRate(struct DeviceControl *control, struct QuaternionAngularRate *quaternionAngularRate);

/*! \fn int VNS_UART_getQuaternionMagneticAcceleration(struct DeviceControl *control, struct QuaternionMagneticAcceleration *quaternionMagneticAcceleration)
 
 \brief Gets the Quaternion, Magnet and Acceleration reading from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternionMagneticAcceleration Structure to store the quaternion, magnetic and acceleration reading obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternionMagneticAcceleration(struct DeviceControl *control, struct QuaternionMagneticAcceleration *quaternionMagneticAcceleration);

/*! \fn int VNS_UART_getQuaternionAccelerationAngularRate(struct DeviceControl *control, struct QuaternionAccelerationAngularRate *quaternionAccelerationAngularRate)
 
 \brief Gets the Quaternion, Acceleration and Angular Rate reading from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100.
 \param quaternionAccelerationAngularRate Structure to store the quaternion, acceleration and angular rate reading obtained from the VN-100
 \return VNS_SUCCESS for success. VNS_ERROR_COM for failure (or VNS_INVALID_OP if device is a textfile).
 */
int VNS_UART_getQuaternionAccelerationAngularRate(struct DeviceControl *control, struct QuaternionAccelerationAngularRate *quaternionAccelerationAngularRate);

/*! \fn int VNS_UART_getBaudrate(struct DeviceControl *control)
 
 \brief Gets the baud rate from the VN-100
 
 NOTE: If this function is called while asynchronous data is being captured, you could lose perfectly good asynchronous data.\n
 This function discards any data that it isn't looking for. This will be fixed in future versions.
 
 \param control The control structure that points to the VN-100 (or VNS_INVALID_OP if device is a textfile).
 \return Gets the baud rate of the VN-100. Returns VNS_ERROR_COM if there was a problem in reading the baud rate from the device.
 */
int VNS_UART_getBaudrate(struct DeviceControl *control);

/*! \fn int VNS_UART_asynchronousCapture(struct DeviceControl *control, union asyncDataList *dataType)
 
 \brief Gets a line of data from the VN-100
 
 Captures a line of data from the VN-100's asynchronous output\n
 Data captured can be any of the 14 types specified in the VN-100 Manual.  The control structure holds the file descriptor to the VN-100,
 the type of data from the VN-100 to be captured, and an optional sleep delay.
 If the library is compiled with SLEEPDELAY, the function will sleep for the specified number of microseconds to avoid multiple calls
 to POSIX read().\n\n
 
 The dataType contains a list of structures that will hold the line of captured data from the VN-100. Depending on the 'type',
 the appropriate structure inside the union will be filled in.
 
 \param control The control structure that points to the VN-100.
 \param dataType Union that has multiple structures. The proper structure will be filled in depending on 'type' in control.
 \return VNS_SUCCESS for success, VNS_EOF for end of stream, VNS_INVALID_DATA for invalid data in stream, VNS_ERROR_COM for error in communication with the device.
 */
int VNS_UART_asynchronousCapture(struct DeviceControl *control, union asyncDataList *dataType);
#endif
