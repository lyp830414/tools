#pragma once
#include <types.h>

extern "C" {
    void assert_is_utf8( const char* str, uint32_t len, const char* err_msg_cstr );
}
