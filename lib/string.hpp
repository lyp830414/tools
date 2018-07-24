#pragma once

static inline uint32_t strlen(const char* str)  
{  
    uint32_t n = 0;  
    while(*str++)   
    {  
        n++;      
    }  
    return n;  
}

static inline int strcmp (const char * s1, const char * s2)
{
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == 0)
            return 0;
    return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1;
}

static inline char *strcpy(char *to, const char *from)
{
    char * result = to;
    while( (*to++ = *from++) != '\0')
    {
    }
    return result;       
}


static inline char *strcat(char *strDes, const char *strSrc)   
{   
    char *address = strDes;   
    while (*strDes != '\0')   
        ++ strDes;   
    while ((*strDes ++ = *strSrc ++) != '\0')   
    {        
    }
    return address;   
}