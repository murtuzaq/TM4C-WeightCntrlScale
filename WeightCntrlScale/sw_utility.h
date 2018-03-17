/*
 * sw_utility.h
 *
 *  Created on: Jul 21, 2016
 *      Author: mquaizar
 */

#ifndef SW_UTILITY_H_
#define SW_UTILITY_H_

#include <stdint.h>

uint16_t ASCII2Dec(char* str);
void ConcatStr2Num(char* str, uint32_t num, char* pArray);
void ConcatStr(char* str1, char* str2, char* pArray);
void Float2ASCII(float num, char* str, unsigned char precision);
void MakeArray2Equal1(char* pArray1, char* pArray2, uint8_t ArraySize);
void MakeString2Equal1(char* pArray1, char* pArray2);
uint8_t SetArray2Zero(char* pArray, char ArraySize);
uint8_t ArraysAreEqual(char* pArray1, char* pArray2, uint8_t ArraySize);
unsigned char GetSubstring(char* target, char* startStr, char endChar, char* substring, unsigned char arraySize);
unsigned char FindSubstring(char* target, char* str);
unsigned char StringLength(char* target);
#endif /* SW_UTILITY_H_ */
