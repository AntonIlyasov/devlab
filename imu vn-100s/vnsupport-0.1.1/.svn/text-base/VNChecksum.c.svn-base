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

#include "VNChecksum.h"

/*
 Calculates the checksum of the command, then returns it
 */

unsigned char VNS_UART_calculateChecksum(char *command, int length)
{
	unsigned char xor = 0;
	int i;	
	for(i = 0; i < length; i++)
	{
		//Do not include the '$' sign in the xor calculation
		if(command[i] == '$')
			continue;
		else if(command[i] == '*') //exit the for loop right away as soon as the '*' is detected. 
			return xor;
		else
		{
			xor ^= (unsigned char)command[i];
		}
		
	}
	return xor;
}

//convert an integer number that is representing a hexadecimal number into a hexadecimal number. eg: int 70 to hex 0x70
unsigned char VNS_UART_parseNibble(unsigned char c)
{
	unsigned char converted = 0;
	switch (c)
	{
		case '0': case '1': case '2': case '3':	case '4':
		case '5': case '6': case '7': case '8': case '9':
			converted = c -'0';
			break;
		case 'A': case 'B': case 'C': 
		case 'D': case 'E':	case 'F':
			converted = c -'A'+10;
			break;
		case 'a': case 'b':	case 'c': 
		case 'd': case 'e': case 'f':
			converted =  c -'a'+10;
	}
	
	return converted;
}

unsigned char VNS_UART_parseHex(char A, char B)
{
	char converted = 0;
	
	converted = VNS_UART_parseNibble(B);
	converted = (converted & 0x0F) | (VNS_UART_parseNibble(A) << 4);
	return converted;
}
