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

/*! \file VNDefines.h
 \brief VNClient defines.
 
 The list of defines aid in the use of the functions offered by the library.
 */

#ifndef VNDEFINES_H
#define VNDEFINES_H

/*! \def QUATACC_SIZE
	\brief Size of a Quaternion and Acceleration measurement from a VN-100*/
#define QUATACC_SIZE 75

/*! \def QMAR_SIZE
	\brief Size of a Quaternion, Magnetic, Acceleration and Angular Rate measurement from a VN-100*/
#define QMAR_SIZE 126

/*! \def TARE_SIZE
	\brief Size of the Tare command*/
#define TARE_SIZE 11

/*! \def FACTORY_SIZE
	\brief Size of the Restore Factory Settings Command*/
#define FACTORY_SIZE 11

/*! \def RESET_SIZE
	\brief Size of the Reset Device Command*/
#define RESET_SIZE 11

/*! \def HEADER_SIZE
	\brief Size of the header of any command sent/received*/
#define HEADER_SIZE 6

/*! \def ERROR_SIZE
	\brief Size of an error string returned by the VN-100*/
#define ERROR_SIZE 14

/*! \def WRITESETTINGS_SIZE
	\brief Size of the string returned by the VN-100 after writting the current settings to the device*/
#define WRITESETTINGS_SIZE 11

/*! \def VNS_NO
	\brief Integer value representing NO*/
#define VNS_NO 0

/*! \def VNS_YES
	\brief Integer value representing YES*/
#define VNS_YES 1

/*! \def VNS_ERROR_COM
	\brief Error in communicating with the device*/
#define VNS_ERROR_COM -1

/*! \def VNS_SUCCESS
	\brief Represents successful operation*/
#define VNS_SUCCESS 0

/*! \def VNS_INVALID_DATA
	\brief Invalid data has been captured from the device (failed checksum)*/
#define VNS_INVALID_DATA 2

/*! \def VNS_INVALID_OP
	\brief User tried to do an invalid operation
 
	Generally this value is returned if a user tries to write or get data from a text file (eg, getFirmware)*/
#define VNS_INVALID_OP -2

/*! \def VNS_EOF
	\brief End of file has been reached*/
#define VNS_EOF 1

/*! \def OFF
	\brief Value to turn off Asyncronous Output*/
#define VNS_OFF 0

/*! \def YPR
	\brief Value to set Asynchronous Output to Yaw, Pitch, Roll*/
#define YPR 1

/*! \def QTN
	\brief Value to set Asynchronous Output to Quaternion*/
#define QTN 2

/*! \def QTM
	\brief Value to set Asynchronous Output to Quaternion and Magnetic*/
#define QTM 3

/*! \def QTA
	\brief Value to set Asynchronous Output to Quaternion and Acceleration*/
#define QTA 4

/*! \def QTR
	\brief Value to set Asynchronous Output to Quaternion and Angular Rates*/
#define QTR 5

/*! \def QMA
	\brief Value to set Asynchronous Output to Quaternion, Magnetic and Acceleration*/
#define QMA 6

/*! \def QAR
	\brief Value to set Asynchronous Output to Quaternion, Acceleration and Angular Rates*/
#define QAR 7

/*! \def QMR
	\brief Value to set Asynchronous Output to Quaternion, Magnetic, Acceleration and Angular Rates*/
#define QMR 8

/*! \def DCM
	\brief Value to set Asynchronous Output to the Directional Cosine matrix*/
#define DCM 9

/*! \def MAG
	\brief Value to set Asynchronous Output to Magnetic measurements*/
#define MAG 10

/*! \def ACC
	\brief Value to set Asynchronous Output to Acceleration measurements*/
#define ACC 11

/*! \def GYR
	\brief Value to set Asynchronous Output to Angular Rate measurements*/
#define GYR 12

/*! \def MAR
	\brief Value to set Asynchronous Output to Magnetic, Acceleration, Angular Rate measurements */
#define MAR 13

/*! \def YMR
	\brief Value to set Asynchronous Output to Attitude, Magnetic, Acceleration and Angular Rate measurements*/
#define YMR 14

/*! \def VNDEVICE
	\brief Specifies that the control structure will be reading from a VN-100*/
#define VNDEVICE 1

/*! \def VNFILE
	\brief Specifies that the control structure will be reading from a file*/
#define VNFILE 0

/*! \def VNRRG_HASH
	\brief 64bit integer representing a VNRRG string. Used internally*/
#define VNRRG_HASH 78418893755940LL

/*! \def VNWRG_HASH
	\brief 64bit integer representing a VNWRG string. Used internally*/
#define VNWRG_HASH 78418977642020LL

/*! \def VNWNV_HASH
	\brief 64bit integer representing a VNWNV string. Used internally*/
#define VNWNV_HASH 94894472189476LL

/*! \def VNRFS_HASH
	\brief 64bit integer representing a VNRFS string. Used internally*/
#define VNRFS_HASH 91561493681700LL

/*! \def VNTAR_HASH
	\brief 64bit integer representing a VNTAR string. Used internally*/
#define VNTAR_HASH 90440540771876LL

/*! \def VNRST_HASH
	\brief 64bit integer representing a VNRST string. Used internally*/
#define VNRST_HASH 92716839884324LL

/*! \def VNERR_HASH
	\brief 64bit integer representing a VNERR string. Used internally*/
#define VNERR_HASH 90513303557668LL


#endif
