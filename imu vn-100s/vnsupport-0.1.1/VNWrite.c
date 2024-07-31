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

#include "VNWrite.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#ifndef MINGW
#include <termios.h>
#endif


int VNS_UART_tareDevice(struct DeviceControl *control)
{	
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, TARE_SIZE, "$VNTAR*5F\r\n", &rPacket);
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_restoreFactorySettings(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, FACTORY_SIZE, "$VNRFS*5F\r\n", &rPacket);
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_resetDevice(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, RESET_SIZE, "$VNRST*4D\r\n", &rPacket);
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_writeSettings(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, WRITESETTINGS_SIZE, "$VNWNV*57\r\n", &rPacket);
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_setAsyncOutputFrequency(struct DeviceControl *control, int frequency)
{
	char message[100];
	unsigned char checksum;
	struct returnPacket rPacket;
	
	if(control->__deviceType == VNDEVICE)
	{
		snprintf(message, 100, "$VNWRG,07,%d*", frequency);
		checksum = VNS_UART_calculateChecksum(message, strlen(message));
		snprintf(message, 100, "$VNWRG,07,%d*%X\r\n",frequency, checksum);
		
		VNS_UART_commonMessageSend(control, 2, strlen(message), message, &rPacket);
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_setAsyncOutputType(struct DeviceControl *control, int outputType)
{
	char message[100];
	unsigned char checksum;
	struct returnPacket rPacket;
	
	if(control->__deviceType == VNDEVICE)
	{
		snprintf(message, 100, "$VNWRG,06,%d*", outputType);
		checksum = VNS_UART_calculateChecksum(message, strlen(message));
		snprintf(message, 100, "$VNWRG,06,%d*%X\r\n", outputType, checksum);
		
		VNS_UART_commonMessageSend(control, 2, strlen(message), message, &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
		{
			switch(outputType)
			{
				case VNS_OFF:
					control->type = VNS_OFF;

#if !defined(MINGW) && !defined(CYGWIN)
					VNS_UART_drainBuffer(control);
#endif
					break;
				case QTA:
					control->type = QUATACC_SIZE;
					break;
				case QMR:
					control->type = QMAR_SIZE;
					break;
				default:
					return -1; //unimplemented
			}
			return rPacket.returnValue;
		}
	}
	else
	{
		//Device is a text file
		switch(outputType)
		{
			case VNS_OFF:
				control->type = VNS_OFF;
				break;
			case QTA:
				control->type = QUATACC_SIZE;
				break;
			case QMR:
				control->type = QMAR_SIZE;
				break;
			default:
				return -1; //unimplemented
		}
		return VNS_SUCCESS;
	}

		
}
/*
 The setBaudRate command is disabled for now.
 I can't seem to parse the return value from the VN-100 after i change the baud rate.
 According to the manual, the VN-100 should output the return value after it changes it's own baud rate.
 However, I can't seem to obtain it. 
 
#ifdef VNPOSIX
int setBaudRate(struct DeviceControl *control, int baudRate)
{
	char message[200];
	unsigned char checksum;
	time_t endwait;
	int n = 0;
	int rValue = 0;
	
	struct termios oldOptions;
	struct termios newOptions;
	struct returnPacket rPacket;	
	
	if(control->fd < 0)
		return -1;
	
	tcgetattr(control->fd, &oldOptions);
	newOptions = oldOptions;
	rPacket.returnValue = -1;
	
	//Still need to figure out how to set non-standard baudrates
	switch(baudRate)
	{
		case 9600:
			rValue = cfsetispeed(&newOptions, B9600);
			printf("rValue is: %d\n", rValue);
			if(rValue != 0)
			{
				printf("Error is %s\n", strerror(rValue));
				return -1;
			}
				
			break;
		case 19200:
			if(cfsetispeed(&newOptions, B19200) != 0);
			   return -1;
			break;
		case 38400:
			if(cfsetispeed(&newOptions, B38400) != 0);
			   return -1;
			break;
		case 57600:
			if(cfsetispeed(&newOptions, B57600) != 0);
			  return -1;
			break;
		case 115200:
			if(cfsetispeed(&newOptions, B115200) != 0);
				return -1;
			break;
		case 128000:
			return -1;
			break;
		case 230400:
			return -1;
			break;
		case 460800:
			return -1;
			break;
		case 921600:
			return -1;
			break;
		default:
			return -1;
	}
	tcsetattr(control->fd, TCSANOW, &newOptions);
	printf("passed termios phase\n");
	snprintf(message, 200, "$VNWRG,05,%d*", baudRate);
	checksum = calculateChecksum(message, strlen(message));
	snprintf(message, 200, "$VNWRG,05,%d*%X\r\n", baudRate, checksum);
	
	endwait = time(NULL) + 4;
	n = write(control->fd, message, strlen(message));
	if(n != strlen(message))
		return -1;
	
	while(time(NULL) < endwait)
	{
		getReturnValue(control, message, &rPacket);
		if(rPacket.returnValue != -1)
			return 0;
	}
	tcsetattr(control->fd, TCSANOW, &oldOptions);
	return rPacket.returnValue;
}
#endif
*/
 
void VNS_UART_commonMessageSend(struct DeviceControl *control, int timeOut, int type, char *message, struct returnPacket *rPacket)
{
	time_t endwait;
	int n;
	rPacket->returnValue = -1;
	
	if(control->fd < 0)
	{
		rPacket->returnValue = -1;
		return;
	}
	
	n = write(control->fd, message, type);
	
	if(n != type)
	{
		rPacket->returnValue = -1;
		return;
	}
	//initial set
	
	endwait = time(NULL) + 4;
	//look for the return value within the 'timeOut' second timeout
	while(time(NULL) < endwait)
	{
		VNS_UART_getReturnValue(control, message, rPacket);
		if(rPacket->returnValue != -1)
			return;
	}
	//failed to write to device
	rPacket->returnValue = -1;
}

int VNS_UART_getReturnValue(struct DeviceControl *control, char *expectedMessage, struct returnPacket *rPacket)
{
	//Buffer, and data parsing control 
	char buffer[500];
	char temp[500];
	char headerType[7];
	int n = 0;
	unsigned int bytesRead = 0;
	int foundBeginning = VNS_NO;
	int successfulReturnValue = VNS_NO;
	//Index is used to find the '$' in memory
	int index = 0;
	//checksum calculations
	unsigned char vnChecksum = 0;
	unsigned char calcChecksum = 0;
	char vnCheck[2];
	//character marker
	char *ptr;
	//Strings integer equivalent
	long long int hash = 0;
	long long int *pHash;
	int registerNumber = 0;
	
	//Don't bother reading in data if the file descriptor isn't valid
	if(control->fd <= -1)
	{
		return -1;
	}
	
	//Read in the header, then figure out if it's an error header or an expected header
	while(!foundBeginning)
	{
		//read in the header size
		while(bytesRead < HEADER_SIZE)
		{
			n = read(control->fd, buffer + bytesRead, HEADER_SIZE - bytesRead);
			bytesRead += n;
		}
		//If the beginning of the buffer is our identifer, determine the header
		if(buffer[0] == '$')
		{
			if(strncmp("$VNERR", buffer, HEADER_SIZE) == 0)
			{
				foundBeginning = VNS_YES;
				successfulReturnValue = VNS_NO;
			}
			else if(strncmp(expectedMessage, buffer, HEADER_SIZE) == 0)
			{
				foundBeginning = VNS_YES;
				successfulReturnValue = VNS_YES;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			/*
			 If the header wasn't found initially, there is a chance that it's further in the buffer.
			 Find the start of the header using the '$' identifier. From there, we read in the rest of the header
			 */
			ptr = (char *)memchr(buffer, '$', HEADER_SIZE);
			if(ptr != NULL)
			{
				index = ptr - buffer;
				memmove(buffer, buffer + index, HEADER_SIZE - index);
				bytesRead = HEADER_SIZE - index;
			}
			else //Header wasn't in the stream. Exit with -1 to signify a header wasn't found.
			{
				return -1;
			}
		}
		
	}
	
	//Read in the remaining bytes of the string. Either read in the rest of the error string, or the return value.
	if(successfulReturnValue == VNS_NO)
	{
		//We have an error string, read in the size of an error string
		while(bytesRead < ERROR_SIZE)
		{
			n = read(control->fd, buffer + bytesRead, ERROR_SIZE - bytesRead);
			bytesRead += n;
		}
	}
	else
	{
		//We don't know what the return value is going to be. So read in one byte at a time.
		while(buffer[bytesRead - 1] != '\n')
		{
			n = read(control->fd, buffer + bytesRead, 1);
			bytesRead += n;
		}
		//need to nullterminate the string if you wish to use strlen. strlen apparently only looks for the null-terminated string. not \r or \n
		buffer[bytesRead - 1] = '\0';
		
		//}
	}
	
	//Calculate the checksum and compare it against the VectorNavs calculated checksum
	calcChecksum = VNS_UART_calculateChecksum(buffer, bytesRead);
	strncpy(vnCheck, (buffer + bytesRead - 4), 2);
	vnChecksum = VNS_UART_parseHex(vnCheck[0], vnCheck[1]);
	
	/*
	 If the checksums don't match up, something really bad has happened.
	 Supposed we have the case of a successful write but a failed response checksum. 
	 What do we do from there? This really shouldn't ever happen however.
	 */
	if(calcChecksum != vnChecksum)
	{
		//See above comment
		printf("Failed checksum in retrieving return value from VN-100\n");
		printf("Failed string was: %s\n", buffer);
		return -1;
	}
	else
	{
		/*
		 Good checksum, so let's parse the response. If the request to the vectornav was a write request, all we need to indicate
		 back to the calling function is that the write was successful. A return of 0 will suffice.
		 If the request was a read request, we then need to parse the return value for the proper data to return.
		 */
		headerType[0] = '\0';
		strncat(headerType, buffer, 6);
		pHash = (long long int *)headerType;
		hash = *pHash & 0x0000FFFFFFFFFFFFLL;
		
		switch(hash)
		{
			case VNRRG_HASH:
				sscanf(buffer + 7, "%2d", &registerNumber);
				switch(registerNumber)
				{
					case 1:
						//Copy the Model Number
						rPacket->packet.model.modelNumber[0] = '\0';
						strncat(rPacket->packet.model.modelNumber, buffer + 10, 5);
						rPacket->packet.model.length = 5;
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 2:
						//Copy the Hardware Revision Number
						sscanf(buffer + 10, "%2d", &(rPacket->packet.hardwareRevisionRegister));
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 3:
						//Copy the Serial Number from register
						rPacket->packet.serial.serialNumber[0] = '\0';
						strncat(rPacket->packet.serial.serialNumber, buffer + 10, 24);
						rPacket->packet.serial.length = 24;
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 4:
						//Copy the firmware verion number
						sscanf(buffer + 10, "%d", &(rPacket->packet.firmwareVersion));
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 5:
						//Copy the Baudrate. In this case, the index variable is used as the length of the buffer
						temp[0] = '\0';
						ptr = (char *)memchr(buffer + 10, '*', strlen(buffer));
						index = ptr - 1 - buffer + 10;
						strncat(temp, buffer + 10, index);
						rPacket->packet.baudRate = atoi(temp);
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 6:
						//Copy the the number which represents the current asynchronous output
						temp[0] = '\0';
						ptr = (char *)memchr(buffer + 10, '*', strlen(buffer));
						index = ptr - 1 - buffer + 10;
						strncat(temp, buffer + 10, index);
						rPacket->packet.asynchronousOutput = atoi(temp);
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 7:
						//Copy the Freqeuency the output is being displayed a
						temp[0] = '\0';
						ptr = (char *)memchr(buffer + 10, '*',strlen(buffer));
						index = ptr - 1 - buffer + 10;
						strncat(temp, buffer + 10, index);
						rPacket->packet.frequency = atoi(temp);
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 8:
						//Get the Yaw, Pitch, Roll
						sscanf(buffer + 10, "%7f %*c %7f %*c %7f", &(rPacket->packet.attitude.yaw), &(rPacket->packet.attitude.pitch), &(rPacket->packet.attitude.roll));
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 9:
						//Get the quaternion
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f",
							   &(rPacket->packet.quaternion.x), &(rPacket->packet.quaternion.y), &(rPacket->packet.quaternion.z), &(rPacket->packet.quaternion.w)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 10:
						//Get the Quaternion and Magnetic
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f",
							   &(rPacket->packet.quatMag.quaternion.x), &(rPacket->packet.quatMag.quaternion.y), &(rPacket->packet.quatMag.quaternion.z), &(rPacket->packet.quatMag.quaternion.w),
							   &(rPacket->packet.quatMag.magnetic.x), &(rPacket->packet.quatMag.magnetic.y), &(rPacket->packet.quatMag.magnetic.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 11:
						//Get the Quaternion and Acceleration
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f",
							   &(rPacket->packet.quatAcc.quaternion.x),  &(rPacket->packet.quatAcc.quaternion.y),  &(rPacket->packet.quatAcc.quaternion.z),  &(rPacket->packet.quatAcc.quaternion.w),
							   &(rPacket->packet.quatAcc.acceleration.x), &(rPacket->packet.quatAcc.acceleration.y), &(rPacket->packet.quatAcc.acceleration.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 12:
						//Get the Quaternion and Angular Rate
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f %*c %8f %*c %8f %*c %8f",
							   &(rPacket->packet.quatAR.quaternion.x), &(rPacket->packet.quatAR.quaternion.y),
							   &(rPacket->packet.quatAR.quaternion.z), &(rPacket->packet.quatAR.quaternion.w),
							   &(rPacket->packet.quatAR.angularRate.x), &(rPacket->packet.quatAR.angularRate.y),
							   &(rPacket->packet.quatAR.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 13:
						//Get the Quaternion, Magnetic and Acceleration
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f",
							   &(rPacket->packet.quatMagAcc.quaternion.x), &(rPacket->packet.quatMagAcc.quaternion.y), 
							   &(rPacket->packet.quatMagAcc.quaternion.z), &(rPacket->packet.quatMagAcc.quaternion.w),
							   &(rPacket->packet.quatMagAcc.magnetic.x), &(rPacket->packet.quatMagAcc.magnetic.y), &(rPacket->packet.quatMagAcc.magnetic.z),
							   &(rPacket->packet.quatMagAcc.acceleration.x), &(rPacket->packet.quatMagAcc.acceleration.y), &(rPacket->packet.quatMagAcc.acceleration.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 14:
						//Get the Quaternion, Acceleration and Angular Rates
						sscanf(buffer + 10, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f %*c %8f %*c %8f %*c %8f",
							   &(rPacket->packet.quatAccAR.quaternion.x), &(rPacket->packet.quatAccAR.quaternion.y), 
							   &(rPacket->packet.quatAccAR.quaternion.z), &(rPacket->packet.quatAccAR.quaternion.w),
							   &(rPacket->packet.quatAccAR.acceleration.x), &(rPacket->packet.quatAccAR.acceleration.y), &(rPacket->packet.quatAccAR.acceleration.z),
							   &(rPacket->packet.quatAccAR.angularRate.x), &(rPacket->packet.quatAccAR.angularRate.y), &(rPacket->packet.quatAccAR.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 15:
						//Get the Quaternion, Magnetic, Acceleration and Angular Rates
						sscanf(buffer + 11, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %8f %*c %8f %*c %8f",
							   &(rPacket->packet.quatMagAccAR.quaternion.x), &(rPacket->packet.quatMagAccAR.quaternion.y),
							   &(rPacket->packet.quatMagAccAR.quaternion.z), &(rPacket->packet.quatMagAccAR.quaternion.w),
							   &(rPacket->packet.quatMagAccAR.magnetic.x), &(rPacket->packet.quatMagAccAR.magnetic.y), &(rPacket->packet.quatMagAccAR.magnetic.z),
							   &(rPacket->packet.quatMagAccAR.acceleration.x), &(rPacket->packet.quatMagAccAR.acceleration.y), &(rPacket->packet.quatMagAccAR.acceleration.z),
							   &(rPacket->packet.quatMagAccAR.angularRate.x), &(rPacket->packet.quatMagAccAR.angularRate.y), &(rPacket->packet.quatMagAccAR.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 16:
						//Get Directional Cosine Orientation Matrix
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.dcm.R1C1), &(rPacket->packet.dcm.R1C2), &(rPacket->packet.dcm.R1C3),
							   &(rPacket->packet.dcm.R2C1), &(rPacket->packet.dcm.R2C2), &(rPacket->packet.dcm.R2C3),
							   &(rPacket->packet.dcm.R3C1), &(rPacket->packet.dcm.R3C2), &(rPacket->packet.dcm.R3C3)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 17:
						//Get the Magnetic Measurements
						sscanf(buffer + 10, "%7f %*c %7f %*c %7f",
							   &(rPacket->packet.magnetic.x), &(rPacket->packet.magnetic.y), &(rPacket->packet.magnetic.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 18:
						//Get the Acceleration Measurements
						sscanf(buffer + 10, "%7f %*c %7f %*c %7f",
							   &(rPacket->packet.acceleration.x), &(rPacket->packet.acceleration.y), &(rPacket->packet.acceleration.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 19:
						//Get the Angular Rate Measurements
						sscanf(buffer + 10, "%8f %*c %8f %*c %8f",
							   &(rPacket->packet.angularRate.x), &(rPacket->packet.angularRate.y), &(rPacket->packet.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 20:
						//Get the Magnetic, Acceleration and Angular Rates
						sscanf(buffer + 10, "%7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %8f %*c %8f %*c %8f %*c",
							   &(rPacket->packet.magAccAR.magnetic.x), &(rPacket->packet.magAccAR.magnetic.y), &(rPacket->packet.magAccAR.magnetic.z),
							   &(rPacket->packet.magAccAR.acceleration.x), &(rPacket->packet.magAccAR.acceleration.y), &(rPacket->packet.magAccAR.acceleration.z),
							   &(rPacket->packet.magAccAR.angularRate.x), &(rPacket->packet.magAccAR.angularRate.y), &(rPacket->packet.magAccAR.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 21:
						//Get Magnetic and Gravity Reference Vector
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.magGravRef.xMagRef), &(rPacket->packet.magGravRef.yMagRef), &(rPacket->packet.magGravRef.zMagRef),
							   &(rPacket->packet.magGravRef.xGravRef), &(rPacket->packet.magGravRef.yGravRef), &(rPacket->packet.magGravRef.zGravRef)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 22:
						//Get Filter Measurements Variance Parameters
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.fmvp.vAngularWalk), &(rPacket->packet.fmvp.vXAxisAngularRate), &(rPacket->packet.fmvp.vYAxisAngularRate), &(rPacket->packet.fmvp.vZAxisAngularRate),
							   &(rPacket->packet.fmvp.vXAxisMagnetic), &(rPacket->packet.fmvp.vYAxisMagnetic), &(rPacket->packet.fmvp.vZAxisMagnetic),
							   &(rPacket->packet.fmvp.vXAxisAcceleration), &(rPacket->packet.fmvp.vYAxisAcceleration), &(rPacket->packet.fmvp.vZAxisAcceleration)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 23:
						//Get Magnetic Hard/Soft Iron Compensation Parameters
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.mhsicp.S11), &(rPacket->packet.mhsicp.S12), &(rPacket->packet.mhsicp.S13),
							   &(rPacket->packet.mhsicp.S21), &(rPacket->packet.mhsicp.S22), &(rPacket->packet.mhsicp.S23),
							   &(rPacket->packet.mhsicp.S31), &(rPacket->packet.mhsicp.S32), &(rPacket->packet.mhsicp.S33),
							   &(rPacket->packet.mhsicp.H1), &(rPacket->packet.mhsicp.H2), &(rPacket->packet.mhsicp.H3)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 24:
						//Get Filter Active Tuning Parameters
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.fatp.magneticDisturbanceGain), &(rPacket->packet.fatp.accelerationDisturbanceGain),
							   &(rPacket->packet.fatp.magneticDisturbanceMemory), &(rPacket->packet.fatp.accelerationDisturbanceMemory)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 25:
						//Get Accelerometer Compensation
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.accComp.C11), &(rPacket->packet.accComp.C12), &(rPacket->packet.accComp.C13),
							   &(rPacket->packet.accComp.C21), &(rPacket->packet.accComp.C22), &(rPacket->packet.accComp.C23),
							   &(rPacket->packet.accComp.C31), &(rPacket->packet.accComp.C32), &(rPacket->packet.accComp.C33),
							   &(rPacket->packet.accComp.B1), &(rPacket->packet.accComp.B2), &(rPacket->packet.accComp.B3)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 26:
						//Get the Reference Frame Rotation
						sscanf(buffer + 10, "%13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f %*c %13f",
							   &(rPacket->packet.rfr.C11), &(rPacket->packet.rfr.C12), &(rPacket->packet.rfr.C13),
							   &(rPacket->packet.rfr.C21), &(rPacket->packet.rfr.C22), &(rPacket->packet.rfr.C23),
							   &(rPacket->packet.rfr.C31), &(rPacket->packet.rfr.C32), &(rPacket->packet.rfr.C33)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 27:
						//Get Yaw, Pitch, Roll, Magnetic, Acceleration, and Angular Rates
						sscanf(buffer + 10, "%7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %8f %*c %8f %*c %8f",
							   &(rPacket->packet.attMagAccAR.ypr.yaw), &(rPacket->packet.attMagAccAR.ypr.pitch), &(rPacket->packet.attMagAccAR.ypr.roll),
							   &(rPacket->packet.attMagAccAR.magnetic.x), &(rPacket->packet.attMagAccAR.magnetic.y), &(rPacket->packet.attMagAccAR.magnetic.z),
							   &(rPacket->packet.attMagAccAR.acceleration.x), &(rPacket->packet.attMagAccAR.acceleration.y), &(rPacket->packet.attMagAccAR.acceleration.z),
							   &(rPacket->packet.attMagAccAR.angularRate.x), &(rPacket->packet.attMagAccAR.angularRate.y), &(rPacket->packet.attMagAccAR.angularRate.z)
							   );
						rPacket->returnValue = VNS_SUCCESS;
						return 0;
					case 28:
						//Get Accelerometer Gain
						//Not Implemented
						rPacket->returnValue = -1;
						return -1;
					default:
						rPacket->returnValue = -1;
						return -1;
				}
				break;
			case VNWRG_HASH:
			case VNWNV_HASH:
			case VNRFS_HASH:
			case VNTAR_HASH:
			case VNRST_HASH:
				rPacket->returnValue = VNS_SUCCESS;
				return 0;
			case VNERR_HASH:
				sscanf(buffer + 7, "%2X", &(rPacket->returnValue));
				return 0;
			default:
				return -1;
		}
	}
	
	return -1;
}
