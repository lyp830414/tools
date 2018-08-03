
#pragma once

extern "C" {

uint32_t setStrValue(char* object, uint32_t objlen, char* key, uint32_t keylen, char *value, uint32_t vallen);

uint32_t getStrValue(char *contract, uint32_t contractlen, char* object, uint32_t objlen, char* key, uint32_t keylen, char *value_buf, uint32_t value_buf_len);

uint32_t removeStrValue(char* object, uint32_t objlen, char* key, uint32_t keylen);

uint32_t setStringValue(string object, uint32_t objlen, string key, uint32_t keylen, string value, uint32_t vallen);

uint32_t getStringValue(string contract, uint32_t contractlen, string object, uint32_t objlen, string key, uint32_t keylen, string value_buf, uint32_t value_buf_len);

uint32_t removeStringValue(string object, uint32_t objlen, string key, uint32_t keylen);


}
