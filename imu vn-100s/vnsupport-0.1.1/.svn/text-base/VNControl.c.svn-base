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

#include "VNControl.h"
#include "VNWrite.h"
#include <stdio.h>
#include <fcntl.h>

const char *errors[14] = {
	"VN-100: Hard Fault", "VN-100: Serial Buffer Overflow", "VN-100: Invalid Checksum",
	"VN-100: Invalid Command", "VN-100: Not Enough Parameters", "VN-100: Too Many Parameters",
	"VN-100: Invalid parameter", "VN-100: Invalid Register", "VN-100: Unauthorized Access",
	"VN-100: Watchdog Reset", "VN-100: Output Buffer Overflow", "VN-100: Insufficient Baud Rate",
	"VNSupport: Invalid Error Code", "VNSupport: Error In Communicating With Device"
};

int VNS_UART_setSleepDelay(struct DeviceControl *control, useconds_t sleepDelay)
{
	if(control == NULL)
		return VNS_ERROR_COM;
	else
	{
		control->sleepDelay = sleepDelay;
		return VNS_SUCCESS;
	}
}

const char* VNS_UART_VNStrerror(int errornum)
{
	if((errornum >= 1) && (errornum <= 12))
		return errors[errornum - 1];
	else if(errornum == -1)
		return errors[13];
	else
		return errors[12];
}

#ifndef MINGW
int VNS_UART_drainBuffer(struct DeviceControl *control)
{
	int n = 0;
	int flags = 0;
	char buffer[50];
	
	flags = fcntl(control->fd, F_GETFL);
	fcntl(control->fd, F_SETFL, flags | O_NONBLOCK);
	
	do
	{
		n = read(control->fd,  buffer, 50);
	} while(n != -1);
	
	//Turn on Blocking
	fcntl(control->fd, F_SETFL, 0);
	
	return VNS_SUCCESS;
}
#endif

