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
   /**
    *  @defgroup consoleapi Console API
    *  @brief Enables applications to log/print text messages
    *  @ingroup contractdev
    *
    */

   /**
    *  @defgroup consolecapi Console C API
    *  @brief C API to log/print text messages
    *  @ingroup consoleapi
    *  @{
    */

   /**
    *  Prints string
    *  @brief Prints string
    *  @param cstr - a null terminated string
    *
    *  Example:
    *  @code
    *  prints("Hello World!"); // Output: Hello World!
    *  @endcode
    */
   //void prints( const char* cstr );

   /**
    *  Prints string up to given length
    *  @brief Prints string
    *  @param cstr - pointer to string
    *  @param len - len of string to be printed
    *
    *  Example:
    *  @code
    *  prints_l("Hello World!", 5); // Output: Hello
    *  @endcode
    */
   void prints_l( const char* cstr, uint32_t len);

   /**
    * Prints value as a 64 bit unsigned integer
    * @brief Prints value as a 64 bit unsigned integer
    * @param Value of 64 bit unsigned integer to be printed
    *
    *  Example:
    *  @code
    *  printi(1e+18); // Output: 1000000000000000000
    *  @endcode
    */
   //void printi( uint64_t value );

   /**
    * Prints value as a 128 bit unsigned integer
    * @brief Prints value as a 128 bit unsigned integer
    * @param value 128 bit integer to be printed
    *
    *  Example:
    *  @code
    *  uint128_t large_int(87654323456);
    *  printi128(large_int); // Output: 87654323456
    *  @endcode
    */
   //void printi128( const uint128_t* value );

   /**
    * Prints value as double
    * @brief Prints value as double
    * @param Value of double (interpreted as 64 bit unsigned integer) to be printed
    *
    *  Example:
    *  @code
    *  uint64_t double_value = double_div( i64_to_double(5), i64_to_double(10) );
    *  printd(double_value); // Output: 0.5
    *  @endcode
    */
   //void printd(uint64_t value);

   /**
    * Prints a 64 bit names as base32 encoded string
    * @brief Prints a 64 bit names as base32 encoded string
    * @param Value of 64 bit names to be printed
    *
    * Example:
    * @code
    * printn(N(abcde)); // Output: abcde
    * @endcode
    */
   //void printn( uint64_t name );

   /**
    */
   //void printhex( void* data, uint32_t datalen );
   /// @}
}

