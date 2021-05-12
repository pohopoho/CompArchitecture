#ifndef PTRFUNCS_H
#define PTRFUNCS_H
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

//  DO NOT MODIFY THIS FILE!! //

/**  Uses pointer-based logic to access a specified portion of a region of
 *   memory and copy bytes at an interval of skipSize to a supplied array.
 *   For example, if skipSize == 0, then consecutive bytes are copied, and
 *   if skipSize == 2 then every third byte is copied.
 * 
 *   Pre:   pDest points to the beginning of a region of memory large
 *             enough to hold maxBytes offsets;
 *          blockAddress points to the first byte of the memory region;
 *          blockLength is the number of bytes in the memory region;
 *          startOffset is the location, relative to blockAddr, of the first
 *             relevant byte of the memory region;
 *          skipSize is the number of bytes between bytes to be copied
 *             (so copies consecutive bytes if skipSize == 0)
 *          maxBytes is the maximum number of bytes that are to be copied
 * 
 *   Post:  the specified bytes have been copied, in order, into the memory
 *             region that is pointed to by pBytes;
 *          no changes have been made to any other memory locations
 * 
 *   Returns:
 *          number of bytes that were copied
 * 
 *   Restrictions:
 *      You must use only pointer syntax in accessing memory.
 */
uint32_t getScatteredBytes(uint8_t* const pDest, 
                           const uint8_t* const blockAddress, uint32_t blockLength,
		                     uint32_t startOffset, uint8_t skipSize, uint32_t maxBytes);


/**  Uses pointer-based logic to search a specified portion of a region of
 *   memory for occurrences of a specified one-byte value.
 * 
 *   Pre:   pOffsets points to an array large enough to hold the results;
 *          blockAddress points to the first byte of the memory region;
 *          blockLength is the number of bytes in the memory region;
 *          Byte is the value to be searched for
 *          maxBytes is the maximum number of bytes that are to be copied
 * 
 *   Post:  the file offsets of the matching bytes are copied, in order,
 *          into the array pointed to by pOffsets
 * 
 *   Returns:  the number of occurrences of Byte found in the memory region
 * 
 *   Restrictions:
 *      You must use only pointer syntax in accessing memory.
 */
uint32_t findOccurrencesOfByte(uint32_t* const pOffsets,
                               const uint8_t* const blockAddress, uint32_t blockLength,
                               uint8_t Byte, uint32_t maxBytes);


/**  Uses pointer-based logic to search a specified portion of a region of
 *   memory for occurrences of a specified pattern (sequence of bytes).
 * 
 *   Pre:  pOffsets points to an array of dimension at least 256
 *         blockAddr points to the first byte of the memory region
 *         blockLength is number of bytes in the memory region
 *         pPattern points to a copy of the pattern that is to be found
 *         patternLength is the number of bytes in the target pattern
 * 
 *   Post: The offset of each occurrence of the pattern have been stored
 *            in pOffsets, in ascending order, starting at index 0.
 * 
 *   Returns:  the number of occurrences of the pattern found in the memory region
 * 
 *   Restrictions:
 *      You must use only pointer syntax in accessing memory.
 */
uint32_t findOccurrencesOfPattern(uint32_t* const pOffsets,
                                  const uint8_t* const blockAddress, uint32_t blockLength,
                                  const uint8_t* const pPattern, uint8_t patternLength);

/**  Interprets the bytes of a given sequence as int16_t values and sums them.
 * 
 *   Pre:  blockAddr points to the first byte of the memory region
 *         blockLength is number of bytes in the memory region
 *         sequenceOffset is the offset of the first byte of the sequence that
 *           is to be summed
 *         sequenceLength is the number of bytes in the sequence, and
 *           sequenceLength > 0
 *
 *   Returns:  the sum of the int16_t values obtained from the given sequence;
 *             if the sequence contains an odd number of bytes, the final byte
 *             is ignored
 * 
 *   Restrictions:
 *      You must use only pointer syntax in accessing memory.
 */
int16_t sumSequence16(const uint8_t* const blockAddress, uint32_t blockLength, 
                      uint32_t sequenceOffset, uint8_t sequenceLength);

#endif
