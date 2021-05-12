#include "fid.h"



//FID* FID_create(uint32_t feature_ID, uint32_t fileOffset) {
//
//        FID* newFID = calloc(1, sizeof(FID));
//        newFID->featureID = feature_ID;
//        newFID->offset = fileOffset;
//        return newFID;
//}

bool FID_insert(FID* arr, int* numEntries, FID* fIndex) {
  
	if(binarySearch(arr,(*numEntries)-1, fIndex->featureID) > 0)
        {
              return false;
        }
	//if(*numEntries == *size)
        //{
        //        *size = (*size) * 2; //resize
        //        arr = (FID*)realloc(arr, (*size) * sizeof(FID));
        //}
	arr[*numEntries] = *fIndex;
        *numEntries = *numEntries + 1;
	//qsort(arr, *numEntries, sizeof(FID), compareF);
        return true;
}
 
int binarySearch(FID* arr, int right, uint32_t id)
{
	int left = 0;
	int middle;
        while(left <= right)
        {
                middle = left + (right - left)/ 2;
                if(arr[middle].featureID == id)
                {
                        return arr[middle].offset;
                }
                if(arr[middle].featureID < id)
                {
                        left = middle + 1;
                }
                else
                {
                        right = middle - 1;
                }
        }
        return -1;
}


                       
