#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

/**  Driver for testing PtrFuncs.c
 * 
 *   Invocation:  c05 [-all | -copybytes | -findbyte | -findpattern] [-sum16] [-repeat]
 *   Examples:    c05 -scatteredbytes
 *                c05 -findbyte -repeat
 *                c05 -all
 * 
 *   Switches: 
 *      -help          display invocation syntax
 *      (none)         run all tests, randomize testing
 *      -all           run tests for all four functions
 *      -copybytes     run test only for getScatteredBytes()
 *      -findbyte      run test only for findOccurrencesOfByte()
 *      -findpattern   run test only for findOccurrencesOfPattern()
 *      -sum16         run test only for sumSequence16()
 *      -repeat        repeat with previous test data
 * 
 *   Note: -repeat is only guaranteed to repeat the previous test data
 *         if the other switches are kept the same
 */
#include <string.h>

#include "PtrFuncs.h"
#include "Generator.h"
#include "TestCode.h"

static void writeInstructions(FILE* fp);

static const char* const outputFileName = "c05Results.txt";
static const char* const dataBlockFileName = "dataBlock.txt";

#define BLOCK_LENGTH 256   // length in bytes of random data block

int main(int argc, char** argv) {
   
   // Check for help switch:
   if ( argc == 2 && strcmp(argv[1], "-help") == 0 ) {
		writeInstructions(stdout);
		return 0;
	}
	
	// Create output file:
   FILE* Out = fopen(outputFileName, "w");
   
   // Boolean flags for controlling execution, based on the parameter(s) we
   // received on the command-line:
   bool testGet = false, testFind = false, testFindPattern = false, testSum16 = false;
   bool randomize = false;
   
   if ( argc == 1 ) {  // run all tests and randomize
      testGet = true;
      testFind = true;
      testFindPattern = true;
      testSum16 = true;
      randomize = true;
   }
   else if ( (argc == 2) && (strcmp(argv[1], "-repeat") == 0) ) { // run all tests, do not randomize
      testGet = true;
      testFind = true;
      testFindPattern = true;
      testSum16 = true;
   }
   else {  // could be 2 or 3 arguments
      // Determine which tests are to be performed from argv[1]:
      char* modeSelector = argv[1];
      if ( strcmp(modeSelector, "-all") == 0 ) {
		   testGet = testFind = testFindPattern = testSum16 = true;
	   }
	   else if ( strcmp(modeSelector, "-scatteredbytes") == 0 ) {
		   testGet = true;
	   }
	   else if ( strcmp(modeSelector, "-findbyte") == 0 ) {
		   testFind = true;
	   }
	   else if ( strcmp(modeSelector, "-findpattern") == 0 ) {
		   testFindPattern = true;
	   }
	   else if ( strcmp(modeSelector, "-sum16") == 0 ) {
			testSum16 = true;
		}
	   else {
	   	printf("Unrecognized mode selector:  %s\n", modeSelector);
	   	exit(2);
	   }
      if ( argc == 2 ) {
         randomize = true;
      }
      if ( (argc == 3) && (strcmp(argv[2], "-repeat") == 0) ) {
         randomize = false;
      }
      else if ( argc == 3 ) {
	   	printf("Unrecognized option:  %s\n", argv[2]);
	   	exit(3);
      }
   }
	
   // In regular mode, we will read a seed value from a file (written by
   // a previous run in randomization mode), and generate the same test
   // data that was used in that previous run.
   unsigned int seed = 0;
   if ( randomize ) {
      seed = time(NULL);
      FILE* fp = fopen("seed.txt", "w");
      fprintf(fp, "%u\n", seed);
      fclose(fp);
   }
   else {
      FILE* fp = fopen("seed.txt", "r");
      if ( fp == NULL ) {
			fprintf(stdout, "Could not find seed file.\n");
			fprintf(stdout, "You can only use -repeat if a seed file already exists.\n");
			exit(4);
		}
      fscanf(fp, "%u", &seed);
      fclose(fp);
   }
   
   // Seed the random generator:
   srand(seed);

   // Generate test data block:   
   uint8_t* dataBlock = genDataBlock( BLOCK_LENGTH );
   
   // Write memory block hex dump to file so students can see it
   FILE* Dump = fopen(dataBlockFileName, "w");
   writeDataBlock(Dump, dataBlock, BLOCK_LENGTH);
   fclose(Dump);

   // Variables to hold scores from individual tests:
   uint32_t scoreGetBytes = 0; 
   uint32_t scoreFindOccurrencesOfByte = 0;
   uint32_t scoreFindOccurrencesOfPattern= 0;
   uint32_t scoreSumSequence16 = 0;
   
   // Tests that do (should) not modify the content of the data block:
   if ( testGet ) {
		fprintf(Out, "Running test of getScatteredBytes().\n");
      scoreGetBytes = testGetScatteredBytes(dataBlock, BLOCK_LENGTH);
	}
   if ( testFind ) {
		fprintf(Out, "Running test of findOccurrencesOfByte().\n");
      scoreFindOccurrencesOfByte = testFindOccurrencesOfByte(dataBlock, BLOCK_LENGTH);
   }
   if ( testFindPattern ) {
		fprintf(Out, "Running test of findOccurrencesOfPattern().\n");
      scoreFindOccurrencesOfPattern = testFindOccurrencesOfPattern(dataBlock, BLOCK_LENGTH);
   }
   if ( testSum16 ) {
		fprintf(Out, "Running test of sumSequence16().\n");
      scoreSumSequence16 = testSumSequence16(dataBlock, BLOCK_LENGTH);
   }
   
   // Summarize results:
   fprintf(Out, "\n");
   fprintf(Out, ">>Scores from testing<<\n");
   if ( testGet )
      fprintf(Out, "1 >> Score for getScatteredBytes:         %3"PRIu32" / 100\n", scoreGetBytes);
   if ( testFind )
      fprintf(Out, "2 >> Score for findOccurrencesOfByte:     %3"PRIu32" / 100\n", scoreFindOccurrencesOfByte);
   if ( testFindPattern )
      fprintf(Out, "3 >> Score for findOccurrencesOfPattern:  %3"PRIu32" / 100\n", scoreFindOccurrencesOfPattern);
   if ( testSum16 )
      fprintf(Out, "4 >> Score for sumSequence16:             %3"PRIu32" / 100\n", scoreSumSequence16);
   uint32_t totalScore = scoreGetBytes + scoreFindOccurrencesOfByte 
                         + scoreFindOccurrencesOfPattern + scoreSumSequence16;
   fprintf(Out, "Total Score:                              %3"PRIu32" / 100\n", totalScore);

   free(dataBlock);
   fclose(Out);
      
   return 0;
}

static void writeInstructions(FILE* fp) {
	
	fprintf(fp, "Driver for testing PtrFuncs.c\n"
	            "\n" 
	            "Invocation:  c05 [-all | -copybytes | -findbyte | -findpattern] [-sum16] [-repeat]\n"
	            "Examples:    c05 -scatteredbytes\n"
	            "             c05 -findbyte -repeat\n"
	            "             c05 -all\n"
	            "\n" 
               "Switches:\n"
	            "   -help          display invocation syntax\n"
	            "   (none)         run all tests, randomize testing\n"
	            "   -all           run tests for all three functions\n"
	            "   -copybytes     run test only for getScatteredBytes()\n"
	            "   -findbyte      run test only for findOccurrencesOfByte()\n"
	            "   -findpattern   run test only for findOccurrencesOfPattern()\n"
	            "   -sum16         run test only for sumSequence16()\n"
	            "   -repeat        repeat with previous test data\n"
	            "\n" 
               "Note: -repeat is only guaranteed to repeat the previous test data\n"
	            "         if the other switches are kept the same\n"
	 );	
}
