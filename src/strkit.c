#include <stdlib.h>
#include <string.h>
#include "strkit.h"

char* strkit_copy(const char* str) {
	const size_t len = strlen(str) + 1;
	char* copy = (char*)calloc(sizeof(char), len);
	strncpy(copy, str, len);
	
	// Make absolutely sure that there's a trailing nul byte
	copy[len-1] = '\0';
	
	return copy;
}
