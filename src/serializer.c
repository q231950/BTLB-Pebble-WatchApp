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

char** arrayFromString(char * aString) {
    char ** res  = NULL;
    char *  p    = strtok (aString, "|");
    int n_spaces = 0;
    
    /* split string and append tokens to 'res' */
    
    while (p) {
        res = realloc (res, sizeof(char*) * ++n_spaces);

        if (res == NULL)
            APP_LOG(APP_LOG_LEVEL_DEBUG, "memory allocation failed");
            
        
        res[n_spaces-1] = p;
        
        p = strtok (NULL, "|");
    }

    /* realloc one extra element for the last NULL */
    
    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;
    
    return res;
}