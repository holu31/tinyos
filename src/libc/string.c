#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../include/string.h"

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char *strchr(const char *s, const char c){
    while (*s && *s != c)
        ++s;
    return (*s) ? (char*) s : NULL;
}
 
char *strtok(char * str, const char * delim){
    static char *next;
    
    if(str){
        next = str;
        while (*next && strchr(delim, *next))
            *next++ = '\0';
    }
    
    if (!*next)
        return NULL;
    
    str = next;
    
    while (*next && ! strchr(delim, *next))
        ++next;
    while (*next && strchr(delim, *next))
        *next++ = '\0';
    
    return str;
}

int strcmp(char *st1,char *st2){
    int n = 0;
    while(*(st1+n) != '\0' ||*(st2+n) != '\0' )
    {
        if (*(st1+n) > *(st2+n))
        return 1;
        if(*(st1+n) < *(st2+n))
        return -1;
        n++;
    }
    return 0;
}