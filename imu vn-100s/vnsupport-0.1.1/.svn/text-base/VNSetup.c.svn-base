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

#include "VNSetup.h"
#include "VNDefines.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int VNS_UART_initDevice(char *serialDevice, struct DeviceControl *control, int deviceType, speed_t baudRate)
{
	struct termios options;
	
	//Open the port in Non-Blocking Mode
	if(deviceType == VNFILE)
		control->fd = open(serialDevice, O_RDONLY);
	else if(deviceType == VNDEVICE)
		control->fd = open(serialDevice, O_RDWR | O_NOCTTY | O_NDELAY);
	else
		return -1;
	
	if(control->fd == -1)
	{
		printf("open_port: Unable to open %s -  - %s\n", serialDevice, strerror( errno ));
		return VNS_ERROR_COM;
	}

	//Turn on Blocking
	fcntl(control->fd, F_SETFL, 0);
	
	control->__n = 0;
	control->__bytesRead = 0;
	
	if(deviceType == VNFILE)
	{
		control->__deviceType = VNFILE;
		return VNS_SUCCESS;
	}
	
	tcgetattr(control->fd, &options);
	//set IN/OUT baudrate

	if((cfsetispeed(&options, baudRate) != 0) || (cfsetospeed(&options, baudRate) != 0))
	{
		close(control->fd);
		return VNS_ERROR_COM;
	}
	
	options.c_cflag &= ~CRTSCTS; //disable hardware flow control
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
	options.c_cflag &= ~PARENB; //disable parity bit
	options.c_cflag &= ~CSTOP; //1 stop bit
	options.c_cflag &= ~CSIZE; // Mask the character size bits
	options.c_cflag |= CS8; // select 8 data bits 
	options.c_cflag |= (CLOCAL | CREAD); //local line, enable receiver
	//set options on serial port immediately
	if(tcsetattr(control->fd, TCSANOW, &options) != 0)
	{
		close(control->fd);
		return VNS_ERROR_COM;
	}
	
	control->__deviceType = VNDEVICE;
	return VNS_SUCCESS;
}

//Close file descriptor and clean up dynamic memory
int VNS_UART_shutdown(struct DeviceControl *control)
{
	if(close(control->fd) != 0)
	{
		printf("Error in closing serial device (what?)\n");
		return VNS_ERROR_COM;
	}
	else
	{
		return VNS_SUCCESS;
	}
}

int VNS_UART_setDeviceBaudrate(struct DeviceControl *control, speed_t baudRate)
{
	struct termios options;
	/*VectorNav VN-100 Sensor has been preset to run at 115200 baud.
	 If baud rate is undesired, make sure you change it and sync up properly.
	 The following code is POSIX compliant
	 */
	tcgetattr(control->fd, &options);
	//return failure if we can't set the baudrate
	if(cfsetispeed(&options, baudRate) != 0)
		return VNS_ERROR_COM;
	if(cfsetospeed(&options, baudRate) != 0)
		return VNS_ERROR_COM;
	
	//set options on serial port immediately
	if(tcsetattr(control->fd, TCSANOW, &options) != 0)
		return VNS_ERROR_COM;
	return VNS_SUCCESS;
}


