/*
 * sw_utility.c
 *
 *This file will contain all functions not directly related to
 *the central logic of any one module;
 *
 *  Created on: Jul 21, 2016
 *      Author: mquaizar
 */

#include <stdint.h>

uint16_t ASCII2Dec(char* str)
{
	uint8_t ndigit = 0;
	char* Oldstr;
	uint16_t result;

	result = 0xffff;

	Oldstr = str;
	while (*str != 0)
	{
		if (*str >= '0' && *str <= '9')
		{
			ndigit++;
		}
		else
		{
			return(0xffff);//fail
		}
		str++;
	}

	//reset str pointer;
	str = Oldstr;

	switch (ndigit)
	{
	case 1:
		//return(str[0] - '0');
		result  = str[0] - '0';
		break;
	case 2:
		//return((str[0] - '0') * 10 + (str[1] - '0'));
		result = (str[0] - '0') * 10 + (str[1] - '0');
		break;

	case 3:
		//return((str[0] - '0') * 100 + (str[1] - '0') * 10 + (str[2] - '0'));
		result = (str[0] - '0') * 100 + (str[1] - '0') * 10 + (str[2] - '0');
		break;

	default:
		break;
	}
	return(result);//fail
}

void ConcatStr2Num(char* str, uint32_t num, char* pArray)
{
	uint8_t indexStr = 0;

	while (str[indexStr] != 0)
	{
		pArray[indexStr] = str[indexStr];
		indexStr++;
	}

	//convert num string;
	if (num >= 1000)
	{
		pArray[indexStr++] = num / 1000 + 0x30;  //thousand
	}
	if (num >= 100)
	{
		pArray[indexStr++] = (num % 1000) / 100 + 0x30;  //hun
	}
	if (num >= 10)
	{
		pArray[indexStr++] = (num % 100) / 10 + 0x30;  //ten
	}

	pArray[indexStr++] = (num % 10) + 0x30;  //ones;


	pArray[indexStr++] = 0;  //end with a NULL character;

}

void ConcatStr(char* str1, char* str2, char* pArray)
{
	uint8_t indexStr1 = 0;
	uint8_t indexStr2 = 0;
	uint8_t indexStr = 0;

	while (str1[indexStr1] != 0)
	{
		pArray[indexStr] = str1[indexStr1];
		indexStr++;
		indexStr1++;
	}

	while (str2[indexStr2] != 0)
	{
		pArray[indexStr] = str2[indexStr2];
		indexStr++;
		indexStr2++;
	}
	pArray[indexStr] = 0;
}

void Float2ASCII(float num, char* str, unsigned char precision)
{
	unsigned char indexStr = 0;

	//convert num string;
	if (num >= 1000)
	{
		str[indexStr++] = (unsigned int)num / 1000 + 0x30;  //thousand
	}
	if (num >= 100)
	{
		str[indexStr++] = ((unsigned int)num % 1000) / 100 + 0x30;  //hun
	}
	if (num >= 10)
	{
		str[indexStr++] = ((unsigned int)num % 100) / 10 + 0x30;  //ten
	}

	str[indexStr++] = ((unsigned int)num % 10) + 0x30;  //ones;

	if (precision >= 2)
	{
		num = num*100;

		str[indexStr++] = '.';
		str[indexStr++] = ((unsigned int)num % 100) / 10 + 0x30;  //0.X0
		str[indexStr++] = ((unsigned int)num % 10)  + 0x30;  // 0.0X
	}

	else if (precision == 1)
	{
		num = num*10;

		str[indexStr++] = '.';
		str[indexStr++] = ((unsigned int)num % 10)  + 0x30;  //0.X
	}
	str[indexStr] = 0;

}


void MakeArray2Equal1(char* pArray1, char* pArray2, uint8_t ArraySize) {
	uint8_t index = 0;

	for (index = 0; index <= ArraySize; index++) {
		pArray2[index] = pArray1[index];
	}

}


void MakeString2Equal1(char* pArray1, char* pArray2) {
	uint8_t index = 0;

	while(pArray1[index] > 0)
	{
		pArray2[index] = pArray1[index];
		index++;
	}
	pArray2[index] = 0;

}

uint8_t SetArray2Zero(char* pArray, uint8_t ArraySize) {
	uint8_t index = 0;

	for (index = 0; index < ArraySize; index++) {
		pArray[index] = 0;
	}
	return (1);
}
uint8_t ArraysAreEqual(char* pArray1, char* pArray2, uint8_t ArraySize) {
	uint8_t index = 0;

	for (index = 0; index <= ArraySize; index++) {
		if (pArray1[index] != pArray2[index]) {
			return (0);
		}
	}
	return (1);
}

/*
 * [IN] target - large string, from which a sub string is requested
 * [IN] startStr - Begins copying after startStr;
 * [IN] endChar - copy till endChar;
 * [OUT] substring - copy results here;
 * [IN] size off substring;
 *
 * retrun - # of characters copied;
 */
unsigned char GetSubstring(char* target, char* startStr, char endChar, char* substring, unsigned char arraySize)
{
	unsigned int Tindex; //Target string Index;
	unsigned char Sindex; //start String index;
	unsigned char subIndex;
	unsigned char STATEsubstring;
	unsigned char complete;

	STATEsubstring = 0;
	Tindex = 0;
	Sindex = 0;
	subIndex = 0;
	complete = 0;

	while(!complete)
	{
		switch(STATEsubstring)
		{
			case 0:
				if (target[Tindex] == 0)
				{
					complete = 1;
					subIndex = 0;
				}
				else if(target[Tindex] == startStr[Sindex])
				{
					if(startStr[++Sindex] == 0)
					{
						STATEsubstring = 1; //move to copy string;
					}
				}
				else
				{
					Sindex = 0;
				}
				Tindex++;
			break;

			case 1:
				if ((target[Tindex] == 0) || (subIndex > arraySize))
				{
					//terminated without finding the endChar
					complete = 1;
					subIndex = 0;
				}
				else if(target[Tindex] == endChar)
				{
					complete = 1;
					substring[subIndex++] = 0;
				}
				else
				{
					//copy string;
					substring[subIndex++] = target[Tindex];
				}
				Tindex++;
				break;

			default:
				break;
		}
	}

	return(subIndex);

}

unsigned char FindSubstring(char* target, char* str)
{

	unsigned int Tindex = 0;
	unsigned int Sindex = 0;
	unsigned char StartIndex;

	StartIndex = 0;
	while(target[Tindex] != 0)
	{
		if (target[Tindex] == str[Sindex])
		{
			if(Sindex == 0)
			{
				StartIndex = Tindex;
			}
			if(str[++Sindex] == 0)
			{
				return(StartIndex);
			}
		}
		else
		{
			Sindex = 0;
		}
		Tindex++;
	}
	return(0); // no matching string found;

}

unsigned char StringLength(char* target)
{

	unsigned int index = 0;

	while(target[index] != 0)
	{
		index++;
	}
	return(index); // no string matching string found;

}

