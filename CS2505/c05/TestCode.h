#ifndef TESTCODE_H
#define TESTCODE_H
#include <stdio.h>
#include <inttypes.h>

//  DO NOT MODIFY THIS FILE!! //

uint32_t testGetScatteredBytes(const uint8_t* const dBlock, uint32_t Length);

uint32_t testFindOccurrencesOfByte(const uint8_t* const dBlock, uint32_t Length);

uint32_t testFindOccurrencesOfPattern(const uint8_t* const dBlock, uint32_t Length);

uint32_t testSumSequence16(const uint8_t* dBlock, uint32_t Length);

#endif
