#include "PtrFuncs.h"

uint32_t getScatteredBytes(uint8_t* const pDest, 
			   const uint8_t* const blockAddress, uint32_t blockLength, 
			   uint32_t startOffset, uint8_t skipSize, uint32_t maxBytes) {
	const uint8_t* start = blockAddress + startOffset;
	//uint8_t* dest = pDest;
	uint32_t counter = 0;
	for(uint32_t i = 0; i < maxBytes && i * (skipSize + 1)  < (blockLength - startOffset); i++)
	{
		*(pDest+i) = *start;
		start = start + 1 + skipSize;
		counter++;
		//dest++;
		//counter++;
		//if(startOffset + (i * (1 + skipSize)) > blockLength)
		//{
		//	return counter;
		//}	
	}
	return counter;
}

uint32_t findOccurrencesOfByte(uint32_t* const pOffsets,
			       const uint8_t* const blockAddress, uint32_t blockLength,
			       uint8_t Byte, uint32_t maxBytes) {
	uint32_t counter = 0;
	uint32_t* dest = pOffsets;
	const uint8_t* pos = blockAddress;
	for(uint32_t i = 0; counter < maxBytes && i < blockLength; i++)
	{
		if(*pos == Byte)
		{
			*dest = i;
			dest++;
			counter++;
		}
		pos++;	
	}
	return counter;
}

uint32_t findOccurrencesOfPattern(uint32_t* const pOffsets,
				  const uint8_t* const blockAddress, uint32_t blockLength,
				  const uint8_t* const pPattern, uint8_t patternLength) {
	uint32_t counter = 0;
	const uint8_t* pos = blockAddress;
	uint32_t found = 1;
	for(uint32_t i = 0; i < blockLength - patternLength; i++)
	{
		found = 1;
		for(uint32_t j = 0; j < patternLength; j++)
		{
			if(*(pos + j) != *(pPattern + j))
			{
				found = 0;
			}
							
		}
		if(found == 1)
		{
			*(pOffsets + counter) = i;
			counter++;
		}
		pos++;
	}
	return counter;
}

int16_t sumSequence16(const uint8_t* const blockAddress, uint32_t blockLength, 
		      uint32_t sequenceOffset, uint8_t sequenceLength) {
	const uint8_t* pos = blockAddress + sequenceOffset;
	const uint8_t* end = blockAddress + blockLength;
	uint16_t sum = 0;
	for(uint32_t i = 0; i < sequenceLength/2 && (pos + 2) != end; i++)
	{
		int16_t lower = *(pos + 1);
		int16_t higher = *pos;
		int16_t combined = (lower << 8) | higher;
		//convert hex to decimal;			
		pos = pos + 2;
		sum = sum + combined;
	}
	return sum;
}

