#pragma once
#include <types.h>

static inline uint32_t strlen(const char* str)  
{  
    uint32_t cnt = 0;  
    while(*str++)   
    {  
       cnt++;      
    }  
    return cnt;  
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


static inline char *strcat(char *Des, const char *Src)   
{   
    char *addr = Des;   
    while (*Des != '\0')   
        ++ Des;   
    while ((*Des ++ = *Src ++) != '\0')   
    {        
    }
    return addr;   
}

inline size_t cstrlen(const char* cstr) {
    size_t len = 0;
    while(*cstr != '\0') {
      len++;
      cstr++;
    }
    return len;
}
    
class string {

  private:
    size_t size; 
    char* data; 
    bool if_memory_allocated; 
    uint32_t* refcnt; 

    
    void check_free_data() {
      if (if_memory_allocated && refcnt != nullptr) {
        (*refcnt)--;
        if (*refcnt == 0) {
          free(data);
        }
      }
    }

  public:
    string() : size(0), data(nullptr), if_memory_allocated(false), refcnt(nullptr) {
    }

    string(size_t s) : size(s) {
      if (s == 0) {
        data = nullptr;
        if_memory_allocated = false;
        refcnt = nullptr;
      } else {
        data = (char *)malloc(s * sizeof(char));
        if_memory_allocated = true;
        refcnt = (uint32_t*)malloc(sizeof(uint32_t));
        *refcnt = 1;
      }
    }

    string(char* d, size_t s, bool copy) {
      assign(d, s, copy);
    }

    string(const string& obj) {
      if (this != &obj) {
        data = obj.data;
        size = obj.size;
        if_memory_allocated = obj.if_memory_allocated;
        refcnt = obj.refcnt;
        if (refcnt != nullptr) (*refcnt)++;
      }
    }

    string(const char* cstr) {
      size = cstrlen(cstr) + 1;
      data = (char *)malloc(size * sizeof(char));
      memcpy(data, cstr, size * sizeof(char));
      if_memory_allocated = true;
      refcnt = (uint32_t*)malloc(sizeof(uint32_t));
      *refcnt = 1;
    }

    ~string() {
      check_free_data();
    }

    const size_t get_size() const {
      return size;
    }
    
    const size_t get_strlen() const {
      if(size >= 1){
	return size - 1;
      } else {
	return 0;
      }
    }

    char* get_data() const {
      return data;
    }
    
    char* c_str() const {
      return data;
    }

    const bool is_if_memory_allocated() const {
      return if_memory_allocated;
    }

    const uint32_t get_refcnt() const {
      return *refcnt;
    }

    string& assign(char* d, size_t s, bool copy) {
      if (s == 0) {
        clear();
      } else {
        check_free_data();
        if (copy) {
          data = (char *)malloc(s * sizeof(char));
          memcpy(data, d, s * sizeof(char));
          if_memory_allocated = true;
          refcnt = (uint32_t*)malloc(sizeof(uint32_t));
          *refcnt = 1;
        } else {
          data = d;
          if_memory_allocated = false;
          refcnt = nullptr;
        }
        size = s;
      }

      return *this;
    }
    
    void clear() {
      check_free_data();
      data = nullptr;
      size = 0;
      if_memory_allocated = false;
      refcnt = nullptr;
    }

    string substr(size_t offset, size_t substr_size, bool copy) {
      assert((offset < size) && (offset + substr_size < size), "out of bound");
      return string(data + offset, substr_size, copy);
    }

    char operator [] (const size_t index) {
      assert(index < size, "index out of bound");
      return *(data + index);
    }

    string& operator = (const string& obj) {
      if (this != &obj) {
        check_free_data();
        data = obj.data;
        size = obj.size;
        if_memory_allocated = obj.if_memory_allocated;
        refcnt = obj.refcnt;
        if (refcnt != nullptr) (*refcnt)++;
      }
      return *this;
    }

    string& operator = (const char* cstr) {
        check_free_data();
        size = cstrlen(cstr) + 1;
        data = (char *)malloc(size * sizeof(char));
        memcpy(data, cstr, size * sizeof(char));
        if_memory_allocated = true;
        refcnt = (uint32_t*)malloc(sizeof(uint32_t));
        *refcnt = 1;
        return *this;
    }

    string& operator += (const string& str){
      assert((size + str.size > size) && (size + str.size > str.size), "overflow");

      char* new_data;
      size_t new_size;
      if (size > 0 && *(data + size - 1) == '\0') {
        // Null terminated string, remove the \0 when concatenates
        new_size = size - 1 + str.size;
        new_data = (char *)malloc(new_size * sizeof(char));
        memcpy(new_data, data, (size - 1) * sizeof(char));
        memcpy(new_data + size - 1, str.data, str.size * sizeof(char));
      } else {
        new_size = size + str.size;
        new_data = (char *)malloc(new_size * sizeof(char));
        memcpy(new_data, data, size * sizeof(char));
        memcpy(new_data + size, str.data, str.size * sizeof(char));
      }

      check_free_data();
      data = new_data;

      size = new_size;
      if_memory_allocated = true;
      refcnt = (uint32_t*)malloc(sizeof(uint32_t));
      *refcnt = 1;

      return *this;
    }

    int32_t compare(const string& str) const {
      int32_t result;
      if (size == str.size) {
        result = memcmp(data, str.data, size);
      } else if (size < str.size) {
        result = memcmp(data, str.data, size);
        if (result == 0) {
          result = 0 - (unsigned char)str.data[size];
        }
      } else if (size > str.size) {
        result = memcmp(data, str.data, str.size);
        if (result == 0) {
          result = (unsigned char)data[str.size];
        }
      }
      return result;
    }

    friend bool operator < (const string& lhs, const string& rhs) {
      return lhs.compare(rhs) < 0;
    }

    friend bool operator > (const string& lhs, const string& rhs) {
      return lhs.compare(rhs) > 0;
    }

    friend bool operator == (const string& lhs, const string& rhs) {
      return lhs.compare(rhs) == 0;
    }

    friend bool operator != (const string& lhs, const string& rhs) {
      return lhs.compare(rhs) != 0;
    }

    friend string operator + (string lhs, const string& rhs) {
      return lhs += rhs;
    }
	
  };

