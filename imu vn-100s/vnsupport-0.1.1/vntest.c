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

/*
  How to use the test program:
  This program is just a quick demonstration of how the vnsupport library works.
  Please read through the sourcecode before executing the program.
  Run the program with a parameter that points to the VN-100 or a textfile with asynchronous data.
	eg. ./vntest /dev/tty.usbserial12
  The program expects the VN-100 to run at 115200 baud. Change your VN-100 or the program to match
  the baudrate you want.
 */
#include "VectorNav.h"
#include <stdio.h>

#ifdef MINGW
#include <fcntl.h>
int _CRT_fmode = _O_BINARY; //need to treat CR-LF as two separate characters
int initDevice(char *serialDevice, struct DeviceControl *control, int deviceType); //MinGW function to open the serial device.
#endif

int main (int argc, char * argv[]) 
{
	FILE *fp;
	union asyncDataList qa; //Struct to store data from the asynchronous output
	struct DeviceControl control; //Control structure. Holds file decriptor and buffer
	int rValue; //return value
	int asyncDataType;
	int deviceType = VNDEVICE; //We want to gather data from the VN-100 hardware. Use VNFILE to read from a text file

	
	//depending on how the code is compiled, we either use the built-in initialization, or a custom initialization function
#ifdef MINGW
	rValue = initDevice(argv[1], &control, deviceType);
#else
	rValue = VNS_UART_initDevice(argv[1],&control, deviceType, B115200);
#endif
	
	if(rValue == VNS_ERROR_COM)
	{
		fprintf(stderr, "Failed to open device\n");
		return -1;
	}
	
	if((fp = fopen("log.txt", "w")) == NULL)
	{
		fprintf(stderr, "failed to open log file\n");
		return -1;
	}
	
	//Set the sleep delay to 1/10th of a second
	VNS_UART_setSleepDelay(&control, 100000);
	
	/*
	 If the type of device we want to read from is a VN-100, we can make use of all functions available to the library.
	 If you are reading from the textfile, you can only capture asynchronous data, or you will get unexpected results.
	 (e.g, you can't get the baudrate from a textfile)
	 
	 If you are reading from a text file, this means you probably have captured asynchronous data from a serial terminal.
	 The only useful function in this library would then be the asynchronousCapture function.
	 */
	
	if(deviceType == VNDEVICE)
	{
		printf("Asynchronous Output Type is currently: %d\n", VNS_UART_getAsyncOutputType(&control));
		
		//Disable the asynchronous output
		rValue = VNS_UART_setAsyncOutputType(&control, VNS_OFF);
		if(rValue != VNS_SUCCESS)
		{
			fprintf(stderr, "Failed to set the output type\n");
			fprintf(stderr, "Error is: %s\n", VNS_UART_VNStrerror(rValue));
			return -1;
		}
		else
			printf("Device async output turned off\n");
	
		//Tare device
		if(VNS_UART_tareDevice(&control) != VNS_SUCCESS)
		{
			fprintf(stderr, "Failed to tare device\n");
			return -1;
		}
		else
			printf("Device tared\n");
	
		//Display some details from the VN-100
		printf("Baud Rate is %d\n", VNS_UART_getBaudrate(&control));
		printf("Current asynchronous Frequency is: %d\n", VNS_UART_getAsyncOutputFrequency(&control));
		
		//Set the Asynchronous output Frequency
		rValue = VNS_UART_setAsyncOutputFrequency(&control, 100);
		if(rValue != VNS_SUCCESS)
		{
			fprintf(stderr, "Failed to set the output frequency\n");
			fprintf(stderr, "Error is: %s\n", VNS_UART_VNStrerror(rValue));
			return -1;
		}
		else
			printf("Set asynchronous Frequency to: %d\n", VNS_UART_getAsyncOutputFrequency(&control));
		
		//Turn on asynchronous output, and set it to Quaternion and Acceleration
		rValue = VNS_UART_setAsyncOutputType(&control, QTA);
		if(rValue != VNS_SUCCESS)
		{
			fprintf(stderr, "Failed to set the output type\n");
			fprintf(stderr, "%s\n", VNS_UART_VNStrerror(rValue));
			return -1;
		}
		else
			printf("Device output set to Quaternion Acceleration\n");
		
	}
	else
	{
		/*
		 We are reading from a textfile. Make sure you set the output type to whatever type of 
		 asynchronous data is stored inside the file. Currently the only supported data types are:
		 Quaternion and Acceleration, and Quaternion Magnetic Acceleration and Angular Rate*/
		VNS_UART_setAsyncOutputType(&control, QTA);
	}
		
	
	asyncDataType = VNS_UART_getAsyncOutputType(&control);
	
	/*
	 This loop either captures data from the VN-100 or a textfile. If you are reading from the VN-100,
	 just press ctrl-c to stop capturing. This program isn't meant to exit cleanly. You can then take 
	 a look at log.txt to see how the data was captured. You will also be able to see if any lines failed.
	 Try pressing the tare button during the capture phase. Analyze log.txt and you will see "Invalid Data".
	 If you are reading from a textfile, you can let the program end cleanly*/
	printf("Capturing Asynchronous Data, press Ctrl-C to exit\n");
	for(;;)
	{
		rValue = VNS_UART_asynchronousCapture(&control, &qa);
		
		if(rValue == VNS_SUCCESS)
		{
			switch(asyncDataType)
			{
				case QTA:
					fprintf(fp, "%+09.6f %+09.6f %+09.6f %+09.6f %+07.3f %+07.3f %+07.3f\n", qa.quatAcc.quaternion.w, qa.quatAcc.quaternion.x, qa.quatAcc.quaternion.y, qa.quatAcc.quaternion.z, qa.quatAcc.acceleration.x, qa.quatAcc.acceleration.y, qa.quatAcc.acceleration.z);
					break;
				case QMR:
					//Even though there is more data in qmar, we're just going to print out the quaternion and acceleration values
					fprintf(fp, "%+09.6f %+09.6f %+09.6f %+09.6f %+07.3f %+07.3f %+07.3f\n", qa.qmar.quaternion.w, qa.qmar.quaternion.x, qa.qmar.quaternion.y, qa.qmar.quaternion.z, qa.qmar.acceleration.x, qa.qmar.acceleration.y, qa.qmar.acceleration.z);
					break;
			}
				   
		}
		else if(rValue == VNS_INVALID_DATA)
			fprintf(fp, "Invalid Data\n");
		else if(rValue == VNS_EOF)
		{
			printf("end of file found\n");
			break;
		}
		else if(rValue == VNS_ERROR_COM)
		{
			fprintf(stderr, "Error in communicating with the device\n");
			return -1;
		}
	}
	
	printf("File has been read in\n");
#ifndef MINGW
	VNS_UART_shutdown(&control);
#else
	close(control.fd);
#endif
	fclose(fp);
    return 0;
}

/* Used for MinGW. Use control panel to set baud rate on device. Make sure the VN-100 is at the same baud
   as the serial port
 */
#ifdef MINGW
int initDevice(char *serialDevice, struct DeviceControl *control, int deviceType)
{	
	//No blocking turned on. Will fix in future release
	if(deviceType == VNFILE)
		control->fd = open(serialDevice, O_RDONLY);
	else if(deviceType == VNDEVICE)
		control->fd = open(serialDevice, O_RDWR);
	else
		return -1;
	
	if(control->fd == -1)
	{
		printf("open_port: Unable to open %s - %s\n", serialDevice, strerror( errno ));
		return -1;
	}
	
	control->__deviceType = deviceType;
	control->__n = 0;
	control->__bytesRead = 0;
	
	return 0;
}
#endif
