//
//  serializer.c
//  
//
//  Created by Martin Kim Dung-Pham on 01/01/16.
//
//

#include "serializer.h"
#include <Pebble.h>
#include <stdlib.h>

char** arrayFromString(char * aString, int * count) {
    char* new_str = malloc(strlen(aString));
    strcpy(new_str, aString);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, new_str);
    
    char ** res  = NULL;
    char * p = strtok (new_str, "|");
    int n_spaces = 0;
    
    while (p) {
        res = realloc (res, sizeof(char*) * ++n_spaces);

        if (res == NULL)
            APP_LOG(APP_LOG_LEVEL_DEBUG, "memory allocation failed");
        
        res[n_spaces-1] = p;
        
        p = strtok (NULL, "|");
    }
    
    *count = n_spaces;
    
    return res;
}