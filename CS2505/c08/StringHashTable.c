// hashTable.c
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "StringHashTable.h"

#define defaultNumOffsets 4

// Node type for the chained hash table for C-strings.
struct _StringNode {
	char*     key;              // ptr to proper C-string
	uint32_t* locations;        // ptr to dynamic array of int locations
	uint32_t  numLocations;     // number of elements currently in locations array
	uint32_t  maxLocations;     // dimension of locations array
	struct _StringNode* next;   // ptr to next node object in table slot
};

// Declare "private" helper functions here (one is already included):
static void StringNode_display(FILE* fp, const StringNode* const pNode);


///////////////////////////////////////////////////////////////// StringNode implementation
//
// You should write implementations of any "private" functions related to the
// StringNode type here; FWIW the reference solution includes two such functions
// in addition to the StringNode_display() function that's given:
//

/** Writes a formatted display of the fields of a StringNode object; for
 *  example:
 *            New Tank:    [359569, 411752, 857960, 942772]
 * 
 *  Pre:  fp is open on an output file
 *        pNode points to a valid StringNode object
 */
static void StringNode_display(FILE* fp, const StringNode* const pNode) {
	
	assert( pNode != NULL );
	
	fprintf(fp, "     %s:  ", pNode->key);
	
   fprintf(fp, "  [%"PRIu32, pNode->locations[0]);
   for (uint32_t idx = 1; idx < pNode->numLocations; idx++) {
	   fprintf(fp, ", %"PRIu32, pNode->locations[idx]);
   }
   fprintf(fp, "]\n");
}


///////////////////////////////////////////////////////////////// StringHashTable implementation
//
// You should complete the implementations of the "public" functions related to the
// StringHashTable type here:
//

/** Creates a new StringHashTable object such that:
 *    - the array has dimension Size
 *    - the slots in the array are set to NULL
 *    - the hash pointer is set to Hasher (so the hash table will use
 *      that user-supplied hash function)
 * 
 *  Pre: Size equals the desired number of slots in the table
 *       Hasher is the name of the hash function the table is to use,
 *         and that function conforms to the specified interface
 * 
 *  Returns: pointer to a newly-allocated StringHashTable object;
 *           blows up an assert() if creation fails
 */
StringHashTable* StringHashTable_create(uint32_t Size, 
                                        uint32_t (*Hasher)(const char* str)) {
	
	// Modify the following code as needed:
	
	StringHashTable* newTable = calloc(1, sizeof(StringHashTable));
	newTable->table = calloc(Size, sizeof(StringNode*));
	newTable->hash = Hasher;
	newTable->tableSz = Size;	
	return newTable;
}

/** Adds an entry to the hash table corresponding to the given key/location.
 *  
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 *        location is set to a corresponding location
 * 
 *  Post: if the key/location pair is already in the table, fails; otherwise
 *        if the table does not contain an entry for key, a new index entry
 *           has been created and location has been installed in it;
 *        otherwise, location has been installed into the existing index entry
 *           for key;
 *        the user's C-string is duplicated, not linked to the table
 * 
 *  Returns: true iff the key/location have been properly added to the table
 */
bool StringHashTable_addEntry(StringHashTable* const pTable, 
                              char* key, uint32_t location) {
	
	// Implement code as needed:
	
       	uint32_t index = pTable->hash(key)%pTable->tableSz;
	StringNode* current = pTable->table[index];
	uint32_t a;
	while(current)
	{
		if(!strcmp(current->key, key))
		{
			a = 0;
			while(a < current->numLocations)
			{
				if(current->locations[a] == location)
				{
					return false;
				}
				a++;
			}
			if(current->numLocations == current->maxLocations)
			{
				current->maxLocations = current->maxLocations << 1;
				uint32_t* newloc = (uint32_t*)realloc(current->locations, current->maxLocations*(sizeof(uint32_t)));
				current->locations = newloc;
			}
			current->locations[current->numLocations] = location;
			current->numLocations++;
			//pTable->numEntries++;
			return true;
	
		}
		current = current->next;
	}
	current = calloc(1, sizeof(StringNode));
	current->key = calloc((strlen(key)+1), sizeof(char));
	strcpy(current->key, key);
	current->maxLocations = 20;
	current->locations = calloc(current->maxLocations, sizeof(uint32_t));
	current->locations[0] = location;
	current->numLocations = 1;
	current->next = pTable->table[index];
	pTable->table[index] = current;
	pTable->numEntries++;
	return true;
}

/** Finds the locations, if any, that correspond to the given key.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 * 
 *  Returns: NULL if there is no entry for key in the table; otherwise
 *           a pointer to an array storing the locations corresponding
 *           to the key, and terminated by a zero location (which would
 *           never be logically valid)
 */
uint32_t* StringHashTable_getLocationsOf(const StringHashTable* const pTable, 
                                         const char* key) {
	
	// Implement code as needed:
	uint32_t index = pTable->hash(key)%pTable->tableSz;
	StringNode* current = pTable->table[index];
	while(current)
	{
		if(!strcmp(current->key, key))
		{
			uint32_t* loccpy = calloc((current->numLocations) + 1, sizeof(uint32_t));
			uint32_t a = 0;
			while(a < current->numLocations)
			{
				loccpy[a] = current->locations[a];
				a++;
			}
			return loccpy;
			
		}
		current = current->next;
	}	
	return NULL;
}

uint32_t StringHashTable_getNum(const StringHashTable* const pTable, const char* key) {

	uint32_t index = pTable->hash(key)%pTable->tableSz;
	StringNode* current = pTable->table[index];
        while(current)
        {
                if(!strcmp(current->key, key))
                {
                        return current->numLocations;
                }
                current = current->next;
        }
        return 0;
}


/** Deallocates memory associated with a StringHashTable.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 * 
 *  Post: the following have been deallocated
 *           - the key belonging to every index entry in the table
 *           - the array of locations belonging to every index entry in the table
 *           - every index entry in the table
 *           - the array for the table
 *
 *   Note: the function does not attempt to deallocate the StringHashTable object
 *         itself, because that object may or may not have been allocated dynamically;
 *         cleanup that up is the responsibility of the user.
 */
void StringHashTable_clear(StringHashTable* const pTable) {
	
	// Implement code as needed:
	uint32_t a = 0;
	StringNode* current = pTable->table[a];
	StringNode* next;
	while(a < pTable->tableSz)
	{
		current = pTable->table[a];
		while(current)
		{
			next = current->next;
			free(current->key);
			free(current->locations);
			free(current);
			current = next;
		}		
		a++;	
	}
	free(pTable->table);
}

/** Writes a clearly-formatted display of the contents of a given
 *  StringHashTable.
 * 
 *  Pre:  fp points to an open file, or is stdout
 *        pTable points to a proper StringHashTable object
 */
void StringHashTable_display(FILE* fp, const StringHashTable* const pTable) {
	
	fprintf(fp, "Hash table contains the following %"PRIu32" entries:\n", 
	            pTable->numEntries);
	
	uint32_t pos = 0;
	while ( pos < pTable->tableSz ) {
		
		StringNode* currEntry = pTable->table[pos];
		if ( currEntry != NULL ) {
		
   		fprintf(fp, "%5"PRIu32": ", pos);
   		StringNode_display(fp, currEntry);
	      currEntry = currEntry->next;
   		while ( currEntry != NULL ) {
				fprintf(fp, "       ");
      		StringNode_display(fp, currEntry);
		      currEntry = currEntry->next;
			}
			fprintf(fp, "\n");
		}
		pos++;
	}
}


//
// You should write implementations of any "private" functions related to the
// StringHashTable type here; FWIW the reference solution includes one such 
// function.
//
