#ifndef FID_H
#define FID_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

struct _FID {
        uint32_t featureID;
        uint32_t offset;
};

typedef struct _FID FID;

//FID* FID_create(uint32_t feature_ID, uint32_t fileOffset);

bool FID_insert(FID* arr, int* numEntries, FID* fIndex);

int binarySearch(FID* arr, int right, uint32_t id);



#endif
