#include "StringBundle.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/***  DELETE THE FOLLOWING DECLARATIONS IF YOU  ***/
/***  DON'T CHOOSE TO IMPLEMENT THESE FUNCTIONS ***/
//static char* copyOf(const char* const str);
static char* nextField(const char* const Line);

/***  PLACE DECLRATIONS OF ANY OTHER HELPER  ***/
/***           FUNCTIONS HERE                ***/


/**  Parses *str and creates a new StringBundle object containing the
 *   separate fields of *str.
 * 
 *   Pre:     str points to a GIS record string, properly terminated
 * 
 *   Returns: a pointer to a new proper StringBundle object
 */
StringBundle* createStringBundle(const char* const str) {
	StringBundle *Bundle = malloc(sizeof(StringBundle));
	Bundle->nTokens = 20;
	Bundle->Tokens = malloc(Bundle->nTokens * sizeof(char *));

	//strcpy(field, nextField(str));
	//Bundle->Tokens[0] = calloc(strlen(field), sizeof(char) + 1);
	Bundle->Tokens[0] = nextField(str);
	//strcpy(Bundle->Tokens[0], field);
	
	for(int i = 1; i <= 19; i++)
	{
		//char field[200] = {'\0'};
		//strcpy(field, nextField(NULL));
		//Bundle->Tokens[i] = calloc(strlen(field), sizeof(char) + 1);
		//strcpy(Bundle->Tokens[i], field);
		Bundle->Tokens[i] = nextField(NULL);	
	}	
	return Bundle;
}

static char* nextField(const char* const Line) {
   static const char* pos;
   if(Line != NULL)
   {
	pos = Line;
   }
   if(*pos == '|')
   {
	//char* empty = malloc(sizeof(char));
	//*empty = "";
	pos = pos + 1;
	return calloc(1, sizeof(char));
   }
   char* field = calloc(200, sizeof(char));
   int track = 0;
   sscanf(pos, "%[^|]%n", field, &track);
   pos = pos + track + 1;
   field = (char *) realloc(field, strlen(field)+1);
   return field;
   
}

/**  Frees all the dynamic memory content of a StringBundle object.
 *   The StringBundle object that sb points to is NOT deallocated here,
 *   because we don't know whether that object was allocated dynamically.
 * 
 *   Pre:     *sb is a proper StringBundle object
 * 
 *   Post:    all the dynamic memory involved in *sb has been freed;
 *            *sb is proper
 */
void clearStringBundle(StringBundle* sb) {
	
   /***  COMPLETE THE IMPLEMENTATION OF THIS FUNCTION  ***/
   	for(int i = 0; i < 20; i++)
	{
		free(sb->Tokens[i]);
	}
	free(sb->Tokens);  
}

/**  Prints a human-friendly display of the contents of a StringBuffer
 *   object to the supplied output stream.
 * 
 *   Pre:  *sb is a proper StringBundle object
 *         fp is open on an output file
 */
void printStringBundle(FILE* fp, const StringBundle* const sb) {
	
	//fprintf(fp, "There are %"PRIu32" tokens:\n", sb->nTokens);
	
	for (uint32_t idx = 0; idx < sb->nTokens; idx++) {
		fprintf(fp, "   %3"PRIu32":  [%s]\n", idx, sb->Tokens[idx]);
	}
}

/***  WRITE THE IMPLEMENTATIONS OF YOUR HELPER FUNCTIONS BELOW  ***/

