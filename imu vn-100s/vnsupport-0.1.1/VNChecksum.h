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

/*! \file VNChecksum.h
 \brief Checksum calculations.
 
 This file gives the user the ability to calculate a checksum against against a string.
 These functions are used internally and are not mean't to be used elsewhere
 */

#ifndef VNCHECKSUM_H
#define VNCHECKSUM_H
/*! \fn unsigned char VNS_UART_calculateChecksum(char *command, int length)
 
	\brief Calculates the checksum of the given string
 
	Calculates the given string by XORing every character in between the '$' and the '*'.\n
	This function is used internally and is not meant to be used elsewhere.
	\param command The string to be calculated.
	\param length The length of the string.
	\return The checksum of the string.
*/
unsigned char VNS_UART_calculateChecksum(char *command, int length);
/*! \fn unsigned char VNS_UART_parseNibble(unsigned char c)
 
 \brief Parses a nibble and gets it's Hexadecimal value
 
 Takes a nibble of data and finds it's equivalent Hexadecimal value.
 \param c Character to be converted to a Hexadecimal value.
 \return Hexadecimal value of the nibble.
 */
unsigned char VNS_UART_parseNibble(unsigned char c);
/*! \fn unsigned char VNS_UART_parseHex(char A, char B)
 
 \brief Gets the hexadecimal value of a byte
 
 Takes two characters, converts them into a single hexadecimal value.\n
 
 \param A First byte to be chopped and converted.
 \param B Second byte to be chopped and converted.
 \return Hexadecimal value of the byte.
 */
unsigned char VNS_UART_parseHex(char A, char B);
#endif
