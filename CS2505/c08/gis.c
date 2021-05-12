#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fid.h"
#include "StringBundle.h"
#include "StringHashTable.h"

#define PI     3.14159265358979323846   // best approximation as double
#define RADIUS 6371.0088 // IUGG mean radius of Earth, in km

uint32_t elfhash(const char* str);
int compareF(const void* left, const void*right);


int main(int argc, char ** argv) {

	if(argc < 2)
	{
		printf("Error. Files not specified.\n");
		exit(1);
	}
	
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");
	FILE* gisFile;

	if ( input == NULL ) {
                fprintf(output, "GIS record file %s was not found.\n", argv[1]);
                exit(2);
        }

	int size = 256;
	FID* array = (FID*)calloc(size, sizeof(FID));
	int numEntries = 0;

	StringHashTable* hashTable;	
	uint32_t tableSize;

	char command[500];

	//int y = 0;
	uint32_t eCounter;
	
	while(fgets(command, 500, input))
	{
		if(command[0] != ';')
		{	
			char method[20];
			sscanf(command, "%s", method);
			//printf("%s\n", command);
			//printf("%s\n", method);
			
			if(!strcmp(method, "db_file"))
			{
				char fName[20];
				sscanf(command, "%*s%s", fName);
				gisFile = fopen(fName, "r");
				//printf("%s %s\n", "DB", fName);
			}
			else if(!strcmp(method, "table_sz"))
			{
				sscanf(command, "%*s%d", &tableSize);
				hashTable = StringHashTable_create(tableSize, elfhash);
				//printf("%d\n", tableSize);
				char* skip = calloc(500, sizeof(char));
				char* record = calloc(500, sizeof(char));
				fgets(skip, 500, gisFile);
				free(skip);
				while(fgets(record, 500, gisFile))
				{
					//record = (char*)realloc(record, strlen(record) + 1);    
					char* cpy = calloc(strlen(record) + 1, sizeof(char));
					strcpy(cpy, record);
					StringBundle* rBundle = createStringBundle(cpy);
										
					FID newFid;
					newFid.featureID = (uint32_t)atoi(rBundle->Tokens[0]);
					newFid.offset = ftell(gisFile) - strlen(cpy);

					if(numEntries == size)
					{
						size = size * 2;
						array = (FID*)realloc(array, size * sizeof(FID));
					}
					
					FID_insert(array, &numEntries, &newFid);
					
					qsort(array, numEntries, sizeof(FID), compareF);
					//printf("%d\t%d\t%d\n", array[y]->featureID, array[y]->offset, numEntries);
					//y++;
					
					char* fName = rBundle->Tokens[1];
					char* abb = rBundle->Tokens[3];
					
					strcat(fName, " ");
					strcat(fName, abb);
					StringHashTable_addEntry(hashTable, fName, ftell(gisFile) - strlen(cpy));
					//clearStringBundle(rBundle);
					free(rBundle);
					free(cpy);		
				}
				free(record);

			}
			else if(!strcmp(method, "exists"))
			{
				eCounter++;
				char find[200];
				char abb[2];
				char p[200];
				sscanf(command, "%*s\t%[^\t]%s", find, abb);
				//printf("%s ", find);
				strcpy(p, find);
				strcat(find, " ");
				strcat(find, abb);
				uint32_t num = StringHashTable_getNum(hashTable, find);
				
				fprintf(output, "Cmd   %d:  exists\t%s\t%s\n\n", eCounter, p, abb);
				if(num!=0)
				{
					fprintf(output, "  %d occurrences: %s\n", num, find);
				}
				else
				{
					fprintf(output, "  Not found in index: %s\n", find);
				}
				fprintf(output, "--------------------------------------------------------------\n");
				//printf("%d\n", num);
					
			}
			else if(!strcmp(method, "details_of"))
			{
				eCounter++;
				char find[200];
				char abb[2];
				char p[200];
				sscanf(command, "%*s\t%[^\t]%s", find, abb);
                                //printf("%s ", find);
                                strcpy(p, find);
                                strcat(find, " ");
                                strcat(find, abb);
				//printf("%s ", find); 
				uint32_t num = StringHashTable_getNum(hashTable, find);
				//printf("%d ", num);
				fprintf(output, "Cmd   %d:  details_of\t%s\t%s\n\n", eCounter, p, abb);
				if(num!=0)
				{
					uint32_t* offsets = StringHashTable_getLocationsOf(hashTable, find);
					uint32_t q = 0;
					while(q < num)
					{
						fseek(gisFile, offsets[q], SEEK_SET);
						//printf("%d\n", offsets[q]);
						char record[500];
						fgets(record, 500, gisFile);
						StringBundle* rBundle = createStringBundle(record);
						fprintf(output, "  FID:       %s\n", rBundle->Tokens[0]);
						fprintf(output, "  Name:      %s\n", rBundle->Tokens[1]);
						fprintf(output, "  Type:      %s\n", rBundle->Tokens[2]);
						fprintf(output, "  State:     %s\n", rBundle->Tokens[3]);
						fprintf(output, "  County:    %s\n", rBundle->Tokens[5]);
						int deg, min, sec;
						char dir;
						char* longDirection;
						sscanf(rBundle->Tokens[8], "%3d%2d%2d%c", &deg, &min, &sec, &dir);
						if(dir == 'W')
						{
							longDirection = "West";
						}
						else
						{
							longDirection = "East";
						}
						fprintf(output, "  Longitude: %03dd %2dm %2ds %s", deg, min, sec, longDirection);
						double lDec = atof(rBundle->Tokens[10]);
						fprintf(output, "   (%.6f)\n", lDec);
						char* latDir;
						sscanf(rBundle->Tokens[7], "%2d%2d%2d%c", &deg, &min, &sec, &dir);
						if(dir == 'N')
						{
							latDir = "North";
						}
						else
						{
							latDir = "South";
						}
						fprintf(output, "  Latitude:   %02dd %2dm %2ds %s", deg, min, sec, latDir);
						lDec = atof(rBundle->Tokens[9]);
						fprintf(output, "   (%.6f)\n", lDec);
						//printf("%s %s %s %s %s %s %s %s %s\n", rBundle->Tokens[0], rBundle->Tokens[1], rBundle->Tokens[2], rBundle->Tokens[3], rBundle->Tokens[5], rBundle->Tokens[7], rBundle->Tokens[8], rBundle->Tokens[9], rBundle->Tokens[10]);
						//printStringBundle(output, rBundle);
						clearStringBundle(rBundle);
						free(rBundle);
						q++;
						if(num > 1)
						{

							fprintf(output, "\n");
						}
							
					}
					if(num > 1)
					{
						fprintf(output, "--------------------------------------------------------------\n");
					}
					else
					{
						fprintf(output, "\n--------------------------------------------------------------\n");
					}
				}
				else
				{
					fprintf(output, "  Not found in index: %s\n", find);
					fprintf(output, "  -----------------------------------------------------------------------\n");	
				}
				//printf("%s\n", "details");
			}
			else if(!strcmp(method, "distance_between"))
			{
				eCounter++;
				uint32_t p1, p2;
				sscanf(command, "%*s\t%d\t%d", &p1, &p2);
				//printf("%d %d\n", p1, p2);
				fprintf(output, "Cmd   %d:  distance_between\t%d\t%d\n\n", eCounter, p1, p2);
				if(binarySearch(array, numEntries-1, p1) < 0) 
				{
					//printf("%d \n", binarySearch(array, numEntries-1, p1));
					fprintf(output, "  Invalid feature ID: %d\n", p1);
					//printf("%s\n", "invalid id");	
				}
				else if(binarySearch(array, numEntries-1, p2) < 0)
				{
					fprintf(output, "  Invalid feature ID: %d\n", p2);
					//printf("%d \n", binarySearch(array, numEntries-1, p2));
				}
				else
				{
					//printf("%d \n", binarySearch(array, numEntries-1, p1));
					//printf("%d \n", binarySearch(array, numEntries-1, p2));
					double lambda1, lambda2, phi1, phi2;
					
					fseek(gisFile, binarySearch(array, numEntries-1, p1), SEEK_SET);
					char record[500];
					fgets(record, 500, gisFile);
					StringBundle* rBundle = createStringBundle(record);
					
					phi1 = atof(rBundle->Tokens[9]);
					lambda1 = atof(rBundle->Tokens[10]);
					//printf("%f %f\t",phi1, lambda1);
			
					
					fseek(gisFile, binarySearch(array,numEntries-1, p2), SEEK_SET);
					char record2[500];
					fgets(record2, 500, gisFile);
					StringBundle* rBundle2 = createStringBundle(record2);

					phi2 = atof(rBundle2->Tokens[9]);
					lambda2 = atof(rBundle2->Tokens[10]);
					//printf("%f %f\n", phi2, lambda2);
					
					
					phi1 = (phi1 * PI)/180;
					lambda1 = (lambda1 * PI)/180;
					phi2 = (phi2 * PI)/180;
					lambda2 = (lambda2 * PI)/180;
					
					double cAngle = acos((sin(phi1)*sin(phi2)) + (cos(phi1) * cos(phi2) * cos(fabs(lambda1 -lambda2))));
					double distance = RADIUS * cAngle;
					

					int deg, min, sec;
                                        char dir;
                                        char* longDirection;
                                        sscanf(rBundle->Tokens[8], "%3d%2d%2d%c", &deg, &min, &sec, &dir);
                                        if(dir == 'W')
                                        {
                                       		 longDirection = "West";                                       
					}
                                        else
                                        {
                                                 longDirection = "East";
                                        }
					fprintf(output, "  First:     ( %03dd %02dm %02ds %s,  ", deg, min, sec, longDirection);  
					char* latDir;
                                        sscanf(rBundle->Tokens[7], "%2d%2d%2d%c", &deg, &min, &sec, &dir);
                                        if(dir == 'N')
                                        {
                                                 latDir = "North";
                                        }
                                        else
                                        {
                                                 latDir = "South";
                                        }
					fprintf(output, "%dd %dm %ds %s )  %s, %s\n", deg, min, sec, latDir, rBundle->Tokens[1], rBundle->Tokens[3]);
					
                                        
                                        sscanf(rBundle2->Tokens[8], "%3d%2d%2d%c", &deg, &min, &sec, &dir);
                                        if(dir == 'W')
                                        {
                                                 longDirection = "West";
                                        }
                                        else
                                        {
                                                 longDirection = "East";
                                        }
                                        fprintf(output, "  Second:    ( %03dd %02dm %02ds %s,  ", deg, min, sec, longDirection);
                                        sscanf(rBundle2->Tokens[7], "%2d%2d%2d%c", &deg, &min, &sec, &dir);
                                        if(dir == 'N')
                                        {
                                                 latDir = "North";
                                        }
                                        else
                                        {
                                                 latDir = "South";
                                        }
                                        fprintf(output, "%dd %dm %ds %s )  %s, %s\n", deg, min, sec, latDir, rBundle2->Tokens[1], rBundle2->Tokens[3]);
					fprintf(output, "  Distance:  %.1fkm\n", distance);	
					clearStringBundle(rBundle);
					clearStringBundle(rBundle2);
					free(rBundle);
					free(rBundle2);//printf("%f\n", distance);
				}	
				fprintf(output, "--------------------------------------------------------------------------\n");
				//printf("%s\n", "distance");
			}
		}
		else if(tableSize > 0)
		{
			char* section = calloc(498, sizeof(char));
			sscanf(command, "%*1c%*[ ]%s", section);
			section = (char*)realloc(section, strlen(section)+1);
			if(!strcmp(section, "Existence"))
			{
				fprintf(output, ";\n%s", command);	
			}
			else if(!strcmp(section, "Details"))
			{
				fprintf(output, ";\n%s", command);
			}
			else if(!strcmp(section, "Distance_between"))
                        {
                                fprintf(output, ";\n%s", command);
                        }
			else if(!strcmp(section, "Some"))
                        {
				fprintf(output, ";\n%s", command);
                                //fprintf(output, ";\n%s", command);
                        }
			else if(!strcmp(section, "Distance"))
                        {
                                fprintf(output, ";\n%s", command);
                        }
			free(section);
			//printf("%s\n", section);
		}
	}
	StringHashTable_clear(hashTable);
	free(hashTable);

	//int k;
	//while(k < size)
	//{
	//	free(array[k]);
	//	k++;
	//}
	//free(array);
	//StringHashTable_display(output, hashTable);
	//printf("%s\n",command);
	
	//while(y < numEntries)
	//{
	///printf("%d\t%d\t%d\n", array[y].featureID, array[y].offset, numEntries);
        //y++;
	//}
	free(array);
	fclose(gisFile);
	fclose(input);
	fclose(output);
}

/** Hashes a C-string to an unsigned integer.
 *  Pre:
 *       str points to a zero-terminated char array
 */
uint32_t elfhash(const char* str) {

   assert(str != NULL ); // self-destuct if called with NULL     

   uint32_t hashvalue = 0,    // value to be returned
            high;	      // high nybble of current hashvalue

   while ( *str ) {	      // continue until *str is '\0'
                
      hashvalue = (hashvalue << 4) + *str++;    // shift high nybble out,
                                                //   and add in next char

      if ( (high = (hashvalue & 0xF0000000)) ) {  // if high nybble != 0000
         hashvalue = hashvalue ^ (high >> 24);    //   fold it back in
      }

      hashvalue = hashvalue & 0x7FFFFFFF;	  // zero high nybble

   }

   return hashvalue;
}

int compareF(const void* left, const void* right)
{
        int l = ((FID*)left)->featureID;
        int r = ((FID*)right)->featureID;
        return (l - r);
}
