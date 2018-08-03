#pragma once

extern "C" {

    typedef unsigned long long   uint64_t;
    typedef unsigned int         uint32_t;
    typedef unsigned short       uint16_t; 
    typedef unsigned char        uint8_t;

    typedef long long            int64_t;
    typedef long                 int32_t;
    typedef short                int16_t;
    typedef char                 int8_t;
 
    	   
    typedef unsigned __int128    uint128_t;
    typedef __int128             int128_t;

    struct uint256 {
        char words[128];
    };

    struct bytes256 {
        uint64_t array[16];
    };

    struct chararray128 {
        char  array[128];
    };

    typedef unsigned int size_t;

    #define PACKED(X) __attribute__((packed)) X

    struct fixed_string16 {
        uint8_t len;
        char    str[16];
    };

    typedef fixed_string16 field_name;

    struct fixed_string32 {
        uint8_t len;
        char    str[32];
    };

    typedef fixed_string32 type_name;

    struct bytes {
        uint32_t len;
        uint8_t* data;
    };

void  assert( uint32_t test, const char* cstr );
void prints_l( const char* cstr, uint32_t len);

}

