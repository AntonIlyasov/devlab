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

/*! \file VNStructs.h
 \brief A list of available data structures offered.
 
 Most data retrieved from the VN-100 contain multiple parts.  These structures contain the multi-part data that you have requested.
 */

//STRUCTS

#ifndef VNSTRUCTS_H
#define VNSTRUCTS_H
#include <unistd.h>

/*! \struct DeviceControl
	\brief Structure that's connected to a VN-100/Text file
 */
struct DeviceControl
{
	/*! File descriptor that connects to a serial node. */
	int fd;
	/*! Type of data to be collected from the VN-100/Text File. */
	int type;
	/*! Time in microseconds to sleep for when the host is reading faster than incoming data */
	useconds_t sleepDelay;
	/*! Pointer to the last byte of data recorded in the buffer. */
	int __n;
	/*! Number of bytes read in the buffer. */
	unsigned int __bytesRead;
	/*! Defines if we are reading from a device or file. */
	int __deviceType;
	/*! Buffer containing data from the VN-100/Text file. */
	char __buffer[500];
};

/*! \struct ModelNumber
	\brief Holds the model number of the VN-100.
 */
struct ModelNumber
{
	/*! Model number.*/
	char modelNumber[7];
	/*! Length of the model number string in bytes*/
	unsigned int length;
};

/*! \struct SerialNumber
	\brief A 24 character serial number which represents a 12 byte number.
 */
struct SerialNumber
{
	/*! Serial Number. */
	char serialNumber[25];
	/*! Length of the serial number in bytes. */
	unsigned int length;
};

/*! \struct Quaternion
 \brief Structure to store quaternion data
 */
struct Quaternion
{
	/*! First unit in the Quaternion */
	float x;
	/*! Second unit in the Quaternion */
	float y;
	/*! Third unit in the Quaternion */
	float z;
	/*! Scalar in the Quaternion */
	float w;
};

/*! \struct Acceleration
 \brief Structure to store acceleration data
 */
struct Acceleration
{
	/*! X-Axis Acceleration */
	float x;
	/*! Y-Axis Acceleration */
	float y;
	/*! Z-Axis Acceleration */
	float z;
};

/*! \struct QuaternionAcceleration
 \brief A struct to store a quaternion and acceleration data.
 */
struct QuaternionAcceleration
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! Indicated whether or not data is useful */
	int garbageData;
};
/*! \struct Magnetic
	\brief Magnetic measurement vector.
 */
struct Magnetic
{
	/*! X-Axis Magnetic */
	float x;
	/*! Y-Axis Magnetic */
	float y;
	/*! Z-Axis Magnetic */
	float z;
};

/*! \struct QuaternionMagnetic
	\brief Storage for a Quaternion and Magnetic measurement*/
struct QuaternionMagnetic
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
};

/*! \struct AngularRate
	\brief Angular Rate measurement*/
struct AngularRate
{
	/*! X-Axis Angular Rate*/
	float x;
	/*! Y-Axis Angular Rate*/
	float y;
	/*! Z-Axis Angular Rate*/
	float z;
};

/*! \struct QuaternionAngularRate
	\brief Storage for a Quaternion and Angular Rate measurement*/
struct QuaternionAngularRate
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Angular Rate measurement*/
	struct AngularRate angularRate;
};

/*! \struct QuaternionMagneticAcceleration
 \brief Storage for a Quaternion, Magnetic and Acceleration measurement*/
struct QuaternionMagneticAcceleration
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
};

/*! \struct QuaternionMagneticAccelerationAngularRate
 \brief Storage for a Quaternion, Magnetic, Acceleration and Angular Rate measurement*/
struct QuaternionMagneticAccelerationAngularRate
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! Angular Rate measurement*/
	struct AngularRate angularRate;
	/*! Indicates whether or not the data is useful*/
	int garbageData;
};

/*! \struct QuaternionAccelerationAngularRate
	\brief Storate for a Quaternion, Acceleration and Angular Rate measurement*/
struct QuaternionAccelerationAngularRate
{
	/*! Quaternion measurement*/
	struct Quaternion quaternion;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! Angular Rate measurement*/
	struct AngularRate angularRate;
};

/*! \struct Attitude
	\brief Storage for the yaw, pitch roll*/
struct Attitude
{
	/*! Yaw measurement*/
	float yaw;
	/*! Pitch measurement*/
	float pitch;
	/*! Roll measurement*/
	float roll;
};

/*! \struct DirectionalCosineMatrix
	\brief Storage for the Attitude Directional Cosine Matrix*/
struct DirectionalCosineMatrix
{
	/*! 1st Row, 1st Column*/
	float R1C1;
	/*! 1st Row, 2nd Column*/
	float R1C2;
	/*! 1st Row, 3rd Column*/
	float R1C3;
	/*! 2nd Row, 1st Column*/
	float R2C1;
	/*! 2nd Row, 2nd Column*/
	float R2C2;
	/*! 2nd Row, 3rd Column*/
	float R2C3;
	/*! 3rd Row, 1st Column*/
	float R3C1;
	/*! 3rd Row, 2nd Column*/
	float R3C2;
	/*! 3rd Row, 3rd Column*/
	float R3C3;
};

/*! \struct MagneticAccelerationAngularRate
	\brief Storage for the Magnetic, Acceleration and Angular Rate measurement*/
struct MagneticAccelerationAngularRate
{
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! AngularRate measurement*/
	struct AngularRate angularRate;
};

/*! \struct MagneticGravityReference
	\brief Storage for the Magnetic and Gravity Reference Vectors*/
struct MagneticGravityReference
{
	/*! X-Axis Magnetic Reference*/
	float xMagRef;
	/*! Y-Axis Magnetic Reference*/
	float yMagRef;
	/*! Z-Axis Magnetic Reference*/
	float zMagRef;
	/*! X-Axis Gravity Reference*/
	float xGravRef;
	/*! Y-Axis Gravity Reference*/
	float yGravRef;
	/*! X-Axis Gravity Reference*/
	float zGravRef;
};

/*! \struct FilterMeasurementsVarianceParameters
	\brief Storage for the statistical variance of the measurements below used by the Kalman Filter*/
struct FilterMeasurementsVarianceParameters
{
	/*! Variance - Angular Walk*/
	float vAngularWalk;
	/*! Variance - X Axis Angular Rate*/
	float vXAxisAngularRate;
	/*! Variance - Y Axis Angular Rate*/
	float vYAxisAngularRate;
	/*! Variance - Z Axis Angular Rate*/
	float vZAxisAngularRate;
	/*! Variance - X Axis Magnetic*/
	float vXAxisMagnetic;
	/*! Variance - Y Axis Magnetic*/
	float vYAxisMagnetic;
	/*! Variance - Z Axis Magnetic*/
	float vZAxisMagnetic;
	/*! Variance - X Axis Acceleration*/
	float vXAxisAcceleration;
	/*! Variance - Y Axis Acceleration*/
	float vYAxisAcceleration;
	/*! Variance - Z Axis Acceleration*/
	float vZAxisAcceleration;
};

/*! \struct MagneticHardSoftIronCompensationParameters
	\brief Values representing the hard and soft iron compensation parameters*/
struct MagneticHardSoftIronCompensationParameters
{
	/*! S11*/
	float S11;
	/*! S12*/
	float S12;
	/*! S13*/
	float S13;
	/*! S21*/
	float S21;
	/*! S22*/
	float S22;
	/*! S23*/
	float S23;
	/*! S31*/
	float S31;
	/*! S32*/
	float S32;
	/*! S33*/
	float S33;
	/*! H1*/
	float H1;
	/*! H2*/
	float H2;
	/*! H3*/
	float H3;
};

/*! \struct FilterActiveTuningParameters
	\brief Values representing the active tuning gains for the Kalman Filter*/
struct FilterActiveTuningParameters
{
	/*! Magnetic Disturbance Gain*/
	float magneticDisturbanceGain;
	/*! Acceleration Disturbance Gain*/
	float accelerationDisturbanceGain;
	/*! Magnetic Disturbance Memory*/
	float magneticDisturbanceMemory;
	/*! Acceleration Disturbance Memory*/
	float accelerationDisturbanceMemory;
};

/*! \struct AccelerometerCompensation
	\brief Values representing the accelerometer compensation parameters*/
struct AccelerometerCompensation
{
	/*! C11*/
	float C11;
	/*! C12*/
	float C12;
	/*! C13*/
	float C13;
	/*! C21*/
	float C21;
	/*! C22*/
	float C22;
	/*! C23*/
	float C23;
	/*! C31*/
	float C31;
	/*! C32*/
	float C32;
	/*! C33*/
	float C33;
	/*! B1*/
	float B1;
	/*! B2*/
	float B2;
	/*! B3*/
	float B3;
};

/*! \struct ReferenceFrameRotation
	\brief Transformation matrix for acceleration, magnetic and angular rates*/
struct ReferenceFrameRotation
{
	/*! C11*/
	float C11;
	/*! C13*/
	float C12;
	/*! C13*/
	float C13;
	/*! C21*/
	float C21;
	/*! C22*/
	float C22;
	/*! C23*/
	float C23;
	/*! C31*/
	float C31;
	/*! C32*/
	float C32;
	/*! C33*/
	float C33;
};

/*! \struct AttitudeMagneticAccelerationAngularRate
	\brief Storage for the Attitude(yaw, pitch roll), Magnetic, Acceleration and Angular Rate measurement*/
struct AttitudeMagneticAccelerationAngularRate
{
	/*! Attitude (yaw, pitch, roll) measurement*/
	struct Attitude ypr;
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! Angular Rate measurement*/
	struct AngularRate angularRate;
};

/*! \union asyncDataList
	\brief Type of data obtained using the asynchronousCapture function.*/
union asyncDataList
{
	/*! Quaternion and Acceleration Data*/
	struct QuaternionAcceleration quatAcc;
	/*! Quaternion Magnetic Acceleration and Angular Rate data*/
	struct QuaternionMagneticAccelerationAngularRate qmar;
};

/*! \union dataPacket
	\brief Data stored inside this union depending on the return value of a called function
 
	This structure is used internally. You should have no reason to use this union inside your own program.*/
union dataPacket
{
	/*! Hardware Revision number*/
	int hardwareRevisionRegister;
	/*! Firmware Version of the VN-100*/
	int firmwareVersion;
	/*! Baud rate of the VN-100*/
	int baudRate;
	/*! Type of data leaving asynchronously*/
	int asynchronousOutput;
	/*! Asynchronous Frequency*/
	int frequency;
	/*! Model Number of the VN-100*/
	struct ModelNumber model;
	/*! Serial Number of the VN-100*/
	struct SerialNumber serial;
	/*! Attitude measurement*/
	struct Attitude attitude;
	/*! Quaternion meamsurement*/
	struct Quaternion quaternion;
	/*! Quaternion and Magnetic measurement*/
	struct QuaternionMagnetic quatMag;
	/*! Quaternion and Acceleration measurement*/
	struct QuaternionAcceleration quatAcc;
	/*! Quaternion and Angular Rate measurement*/
	struct QuaternionAngularRate quatAR;
	/*! Quaternion, Magnetic and Acceleration measurement*/
	struct QuaternionMagneticAcceleration quatMagAcc;
	/*! Quaternion, Acceleration and Angular Rate measurement*/
	struct QuaternionAccelerationAngularRate quatAccAR;
	/*! Quaternion, Magnetic, Acceleration and Angular Rate measurement*/
	struct QuaternionMagneticAccelerationAngularRate quatMagAccAR; 
	/*! Direction Cosine Matrix*/
	struct DirectionalCosineMatrix dcm;
	/*! Magnetic measurement*/
	struct Magnetic magnetic;
	/*! Acceleration measurement*/
	struct Acceleration acceleration;
	/*! Angular Rate measurement*/
	struct AngularRate angularRate;
	/*! Magnetic, Acceleration and Angular Rate measurement*/
	struct MagneticAccelerationAngularRate magAccAR;
	/*! Magnetic Gravity Reference values*/
	struct MagneticGravityReference magGravRef;
	/*! Filter Measurements Variance Prameters*/
	struct FilterMeasurementsVarianceParameters fmvp;
	/*! Magnetic Hard Soft Iron Compensation Parameters*/
	struct MagneticHardSoftIronCompensationParameters mhsicp;
	/*! Filter Active Tuning Parameters*/
	struct FilterActiveTuningParameters fatp;
	/*! Accelerometer Compensation*/
	struct AccelerometerCompensation accComp;
	/*! Reference Frame Rotation*/
	struct ReferenceFrameRotation rfr;
	/*! Attitude, Magnetic, Acceleration and Angular Rate measurement*/
	struct AttitudeMagneticAccelerationAngularRate attMagAccAR;
};

/*! \struct returnPacket
	\brief Holds the return value and data captured from the VN-100
 
	This structure is used internally and is not meant to be used by the programmer.*/
struct returnPacket
{
	/*! Return value from the VN-100 or from the function itself*/
	int returnValue;
	/*! Storage for the data obtained from the VN-100*/
	union dataPacket packet;
};

#endif
