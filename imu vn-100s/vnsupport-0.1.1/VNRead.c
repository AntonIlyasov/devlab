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

#include "VNRead.h"
#include "Macros.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int VNS_UART_asynchronousCapture(struct DeviceControl *control, union asyncDataList *dataType)
{
	//Index is used to find the '$' in memory
	int index = 0;
	//checksum calculations
	unsigned char vnChecksum = 0;
	unsigned char calcChecksum = 0;
	char vnCheck[2];
	//character marker
	char *ptr;
#ifdef DEBUG
	static unsigned long long failCount = 0;
	static unsigned long long passCount = 0;
	static unsigned long long total = 0;
	static FILE *fp;
	time_t rawtime;
	struct tm *timeinfo;

	if(fp == NULL)
	{
		fp = fopen("error_log.txt", "a");
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		fwrite("----------\n", 1, 11, fp);
		fwrite("Logging started at: ", 1, 20, fp);
		fwrite(asctime(timeinfo), 1, strlen(asctime(timeinfo)), fp);
		fflush(fp);
	}
	printf("Pass Count: %llu\n", passCount);
	printf("Fail Count: %llu\n", failCount);
	printf("Total: %llu\n", total);
#endif
	
	//Read in data until we reach our target bytes 
	do
	{
		//Exit the function if read returns -1
		//See Macros.h for more detail on PE_RETURN
		control->__n = PE(read(control->fd, control->__buffer + control->__bytesRead, control->type - control->__bytesRead));
		if(control->__n == -1)
			return VNS_ERROR_COM;

		//If we are on the device
		if(control->__n == 0)
		{
			switch(control->__deviceType)
			{
				case VNDEVICE:
#ifdef MINGW
					/*
					 On mingw/Cygwin there is a possibility of read() returning 0.
					 Just continue normal operation
					 */
					break;
#else
					/*
					 It should be impossible to get a return value of 0 normally since we are 
					 operating on blocking-mode.
					 */
					fprintf(stderr, "read() returned 0 in blocking mode, should not happen\n");
					break;
#endif
				case VNFILE:
					//A return value of 0 indicates that we have finished reading from the file.
					return VNS_EOF;
			}
		}
		
		control->__bytesRead += control->__n;
		
#ifdef VNSLEEPDELAY
		//Sleep for a period of time to reduce multiple calls to read()
		if((control->__bytesRead < (unsigned int)control->type))
			usleep(control->sleepDelay);
#endif
	}
	while(control->__bytesRead < (unsigned int)control->type);
	
	//Calculate the checksum and compare it against the VectorNavs calculated checksum
	calcChecksum = VNS_UART_calculateChecksum(control->__buffer, strlen(control->__buffer));
	strncpy(vnCheck, (control->__buffer + control->type - 4), 2);
	vnChecksum = VNS_UART_parseHex(vnCheck[0], vnCheck[1]);
#ifdef DEBUG
	total++;
#endif
	//If the checksums don't match up, fail the line and exit
	if(calcChecksum != vnChecksum)
	{
#ifdef DEBUG
		failCount++;
		fwrite("Invalid line: ", 1, 14, fp);
		fwrite(control->__buffer, 1, control->type, fp);
		fwrite("\n", 1, 1, fp);
		fflush(fp);
		printf("Invalid Checksum! VN: %x Cacl: %x\n",vnChecksum, calcChecksum);
#endif

		//did we find an incomplete line mixed in with a new line?
		
		/*Did we read in exactly what we wanted? 
		 If not, there maybe be a proper line packed into this current erroneous line.
		 Search for the line and append it to the beginning of the buffer*/
		
		//Find the start of the next line inside the current line
		ptr = (char *)memchr(control->__buffer + 1, '$', control->type - 1);
		//If we find the start of a new line inside the current line, save the remaining number of bytes read in for the next call to readData.
		if(ptr != NULL)
		{
			index = ptr - control->__buffer;
			memmove(control->__buffer, control->__buffer + index, control->type - index);
			control->__bytesRead = control->type - index;
		}
		else
		{
			//No potential line was found. discard the data.
			control->__n = 0; 
			control->__bytesRead = 0;
		}
		
		//let the calling function know that this line is useless
		switch(control->type)
		{
			case QUATACC_SIZE:
				dataType->quatAcc.garbageData = VNS_YES;
				break;
			case QMAR_SIZE:
				dataType->qmar.garbageData = VNS_YES;
		}
		
		return VNS_INVALID_DATA;	
	}
	else
	{
		//Parse the type of data we want
		switch(control->type)
		{
			case QUATACC_SIZE:
				sscanf(control->__buffer + 7, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f",
					   &(dataType->quatAcc.quaternion.x), &(dataType->quatAcc.quaternion.y), &(dataType->quatAcc.quaternion.z), &(dataType->quatAcc.quaternion.w), 
					   &(dataType->quatAcc.acceleration.x), &(dataType->quatAcc.acceleration.y), &(dataType->quatAcc.acceleration.z)
					   );
				dataType->quatAcc.garbageData = VNS_NO;
				break;
			case QMAR_SIZE:
				sscanf(control->__buffer + 7, "%9f %*c %9f %*c %9f %*c %9f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %7f %*c %8f %*c %8f %*c %8f",
					   &(dataType->qmar.quaternion.x), &(dataType->qmar.quaternion.y), &(dataType->qmar.quaternion.z), &(dataType->qmar.quaternion.w),
					   &(dataType->qmar.magnetic.x), &(dataType->qmar.magnetic.y), &(dataType->qmar.magnetic.z),
					   &(dataType->qmar.acceleration.x), &(dataType->qmar.acceleration.y), &(dataType->qmar.acceleration.z),
					   &(dataType->qmar.angularRate.x), &(dataType->qmar.angularRate.y), &(dataType->qmar.angularRate.z)
					   );
				dataType->qmar.garbageData = VNS_NO;
		}
#ifdef DEBUG
		passCount++;
#endif
	}
	//clear all static variables
	control->__n = 0; 
	control->__bytesRead = 0;
	return VNS_SUCCESS;
}

int VNS_UART_getBaudrate(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,05*76\r\n" ,&rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
			return rPacket.packet.baudRate;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getAsyncOutputFrequency(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,07*74\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
			return rPacket.packet.frequency;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getHardwareRevision(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,02*71\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
			return rPacket.packet.hardwareRevisionRegister;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getFirmwareVersion(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,04*77\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
			return rPacket.packet.firmwareVersion;
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getSerialNumber(struct DeviceControl *control, struct SerialNumber *serialNumber)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,03*70\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
		{
			strncpy(serialNumber->serialNumber, rPacket.packet.serial.serialNumber, 24);
			serialNumber->length = 24;
			return rPacket.returnValue;
		}
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getModelNumber(struct DeviceControl *control, struct ModelNumber *modelNumber)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,01*72\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
		{
			strncpy(modelNumber->modelNumber, rPacket.packet.model.modelNumber, 6);
			modelNumber->length = strlen(modelNumber->modelNumber);
			return rPacket.returnValue;
		}
	}
	else
	{
		return VNS_INVALID_OP;
	}
	
}

int VNS_UART_getAsyncOutputType(struct DeviceControl *control)
{
	struct returnPacket rPacket;
	
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,06*75\r\n", &rPacket);
		if(rPacket.returnValue == -1)
			return VNS_ERROR_COM;
		else
			return rPacket.packet.asynchronousOutput;
	}
	else
	{
		switch(control->type)
		{
			case QUATACC_SIZE:
				return QTA;
			case QMAR_SIZE:
				return QMR;
			case VNS_OFF:
				return VNS_OFF;
			default:
				return -1;
		}
	}
	
}

int VNS_UART_getYawPitchRoll(struct DeviceControl *control, struct Attitude *ypr)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,08*7B\r\n", &rPacket);
		*ypr = rPacket.packet.attitude;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}

	
}

int VNS_UART_getQuaternion(struct DeviceControl *control, struct Quaternion *quaternion)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,09*7A\r\n", &rPacket);
		*quaternion = rPacket.packet.quaternion;
		return rPacket.returnValue;
	}
	else
	{
		return VNS_INVALID_OP;
	}
}

int VNS_UART_getQuaternionMagnetic(struct DeviceControl *control, struct QuaternionMagnetic *quaternionMagnetic)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,10*72\r\n", &rPacket);
		*quaternionMagnetic = rPacket.packet.quatMag;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}

	
}

int VNS_UART_getQuaternionAcceleration(struct DeviceControl *control, struct QuaternionAcceleration *quaternionAcceleration)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,11*73\r\n", &rPacket);
		*quaternionAcceleration = rPacket.packet.quatAcc;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}

}

int VNS_UART_getQuaternionAngularRate(struct DeviceControl *control, struct QuaternionAngularRate *quaternionAngularRate)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,12*70\r\n", &rPacket);
		*quaternionAngularRate = rPacket.packet.quatAR;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}
}

int VNS_UART_getQuaternionMagneticAcceleration(struct DeviceControl *control, struct QuaternionMagneticAcceleration *quaternionMagneticAcceleration)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,13*71\r\n", &rPacket);
		*quaternionMagneticAcceleration = rPacket.packet.quatMagAcc;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}
}

int VNS_UART_getQuaternionAccelerationAngularRate(struct DeviceControl *control, struct QuaternionAccelerationAngularRate *quaternionAccelerationAngularRate)
{
	struct returnPacket rPacket;
	if(control->__deviceType == VNDEVICE)
	{
		VNS_UART_commonMessageSend(control, 2, 14, "$VNRRG,14*76\r\n", &rPacket);
		*quaternionAccelerationAngularRate = rPacket.packet.quatAccAR;
		return rPacket.returnValue;
	}
	else 
	{
		return VNS_INVALID_OP;
	}	
}
