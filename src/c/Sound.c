#include <stdlib.h>
#include "Sound.h"

WC_Sound* WC_S_Allocate(void){
	return malloc(sizeof(WC_Sound));
}

void WC_S_Free(WC_Sound* sound){
	free(sound);
}