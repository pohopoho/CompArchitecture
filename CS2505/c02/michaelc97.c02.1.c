//  On my honor:
//
//  -I have not discussed the C language code in my program with
//   anyone other than my instructor or the teaching assistants 
//   assigned to this course.
//
//  -I have not used C language code obtained from another student, 
//   the Internet, or any other unauthorized source, either modified   
//   or unmodified.  
//
//  -If any C language code or documentation used in my program   
//   was obtained from an authorized source, such as a text book or   
//   course notes, that has been clearly noted with a proper citation   
//   in the comments of my program.
//
//  -I have not designed this program in such a way as to defeat or   
//   interfere with the normal operation of the grading code.  
//
//   <Michael Cheung>   
//   <michaelc97>


#include <stdio.h>
#include <stdlib.h>

int taxi(FILE* output, int long1Degs, int long1Mins, int long1Secs, int long2Degs, int long2Mins, int long2Secs,
	 int lat1Degs, int lat1Mins, int lat1Secs, int lat2Degs, int lat2Mins, int lat2Secs);

int main (int argc, char** argv) {

	FILE* inputFile = fopen(argv[1], "r");
	FILE* outputFile = fopen(argv[2], "w");

	int longDegs, longMins, longSecs, longDegs2, longMins2, longSecs2;
	int latDegs, latMins, latSecs, latDegs2, latMins2, latSecs2;
	char longDir, latDir, longDir2, latDir2;
	
	fprintf(outputFile, "%s %s %s %s", "First coordinate\t", "\tSecond coordinate\t", "\tseconds\t", "\tDMS\n");
	fprintf(outputFile, "%s", "-------------------------------------------------------------------------------------------------\n");
	while (fscanf(inputFile, "%*c%3d%2d%2d%c%*c %2d%2d%2d%c%*c %*c%3d%2d%2d%c%*c %2d%2d%2d%c%*c\n",
		 &longDegs, &longMins, &longSecs, &longDir, &latDegs, &latMins, &latSecs, &latDir,
		 &longDegs2, &longMins2, &longSecs2, &longDir2, &latDegs2, &latMins2, &latSecs2, &latDir2) == 16)
	{
		fprintf(outputFile, "%c%03d%s%02d%s%02d%s%c%s", '(', longDegs, "d ", longMins, "m ", longSecs, "s ", longDir, ", ");
		fprintf(outputFile, "%02d%s%02d%s%02d%s%c%s", latDegs, "d ", latMins, "m ", latSecs, "s ", latDir, ")\t");		

		fprintf(outputFile, "%c%03d%s%02d%s%02d%s%c%s", '(', longDegs2, "d ", longMins2, "m ", longSecs2, "s ", longDir2, ", ");
		fprintf(outputFile, "%02d%s%02d%s%02d%s%c%s", latDegs2, "d ", latMins2, "m ", latSecs2, "s ", latDir2, ")\t");
		
		taxi(outputFile, longDegs, longMins, longSecs, longDegs2, longMins2, longSecs2,
                                latDegs, latMins, latSecs, latDegs2, latMins2, latSecs2);	
	}
	
	fclose(inputFile);
	fclose(outputFile);

	return(0);


}

int taxi(FILE* output, int long1Degs, int long1Mins, int long1Secs, int long2Degs, int long2Mins, int long2Secs,
         int lat1Degs, int lat1Mins, int lat1Secs, int lat2Degs, int lat2Mins, int lat2Secs)
{
	int x2 = (long2Degs * 3600) + (long2Mins * 60) + long2Secs;
	int x1 = (long1Degs * 3600) + (long1Mins * 60) + long1Secs;
	
	int y2 = (lat2Degs * 3600) + (lat2Mins * 60) + lat2Secs;
	int y1 = (lat1Degs * 3600) + (lat1Mins * 60) + lat1Secs;

	int taxiSeconds = abs(x2-x1) + abs(y2-y1);

	fprintf(output, "%d%c", taxiSeconds, '\t');
	fprintf(output, "%c%d%s%02d%s%02d%s", '\t', taxiSeconds/3600, "d ", (taxiSeconds%3600)/60, "m ", (taxiSeconds%3600)%60, "s\n");	
	return(0);	 	
}
