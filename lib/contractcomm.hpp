
#include "types.hpp"
#include "db.h"
#include "string.hpp"
#include "msgpack.hpp"
#include "const.h"
//#include <emscripten/emscripten.h>

extern "C" {
    void prints(char *str, int len);
    void printi(uint32_t u);
    uint32_t getParam(unsigned char *param, uint32_t buf_len);
    void init();
    int start(int method);

    int getSender(char *str , int len);
    int getCtxName(char *str , int len);



    static inline void myprints(char *s) 
    {
        prints(s, strlen(s));
    }

    int  callTrx(char *contract , char *method , char *buf , uint32_t buf_len );
    bool isAccountExist(char *mame);
    
}


#define EMSCRIPTEN_KEEPALIVE

#define STR_ARRAY_LEN(STR_LEN) (STR_LEN + 1)

#define PRINT_I(VAR_NAME, ELEM_NAME)  do { char elemstr[] = #ELEM_NAME ; myprints(elemstr); printi(VAR_NAME->ELEM_NAME); }while(0);
#define PRINT_STR(VAR_NAME, ELEM_NAME)  do { char elemstr[] = #ELEM_NAME ; myprints(elemstr); myprints(VAR_NAME->ELEM_NAME); }while(0);
#define PRINT_STRUCT(VAR_NAME, ELEM_NAME) do { print_struct(&VAR_NAME->ELEM_NAME); }while(0);




#define UNPACK_STR(VAR_NAME, ELEM_NAME, ELEM_LEN) do { if (!unpack_str(ctx, VAR_NAME->ELEM_NAME, ELEM_LEN, &size)) return 0; }while(0);
#define UNPACK_U32(VAR_NAME, ELEM_NAME) do { if (!unpack_u32(ctx, &VAR_NAME->ELEM_NAME)) return 0; }while(0); 
#define UNPACK_U64(VAR_NAME, ELEM_NAME) do { if (!unpack_u64(ctx, &VAR_NAME->ELEM_NAME)) return 0; }while(0);
#define UNPACK_ARRAY(SIZE_NUM)  do { if (!unpack_array(ctx, &size)) return 0; if (SIZE_NUM != size) return 0; }while(0);
#define UNPACK_STRUCT(VAR_NAME, ELEM_NAME) do { if (!unpack_struct(ctx, &VAR_NAME->ELEM_NAME)) return 0; }while(0);

#define PACK_ARRAY16(SIZE_NUM) do { if (!pack_array16(ctx, SIZE_NUM)) return 0; }while(0);
#define PACK_U32(VAR_NAME, ELEM_NAME) do { if (!pack_u32(ctx, VAR_NAME->ELEM_NAME)) return 0; }while(0);
#define PACK_U64(VAR_NAME, ELEM_NAME) do { if (!pack_u64(ctx, VAR_NAME->ELEM_NAME)) return 0; }while(0);
#define PACK_STR16(VAR_NAME, ELEM_NAME) do { if (!pack_str16(ctx, VAR_NAME->ELEM_NAME, strlen(VAR_NAME->ELEM_NAME))) return 0; }while(0);


#define CHECK_ACCOUNT_VALID(ACCOUNT) do { if (!isAccountExist(info->ACCOUNT)) return false;} while(0);







struct AssetInfo {
    char userName[STR_ARRAY_LEN(USER_NAME_MAX_LEN)];
    char assetName[MAX_STR_LEN];
    uint64_t assetType;
    char featureTag[MAX_STR_LEN];
    char sampleHash[MAX_STR_LEN];
    char storageHash[STR_ARRAY_LEN(HASH_ID_LEN)];
    uint32_t      expireTime;
    uint32_t      opType;
    char          tokenType[MAX_TOKEN_TYPE_LEN];
    uint64_t      price; 
    char description[MAX_CONTENT_LEN];
    
};






static bool unpack_struct(MsgPackCtx *ctx, AssetInfo *info)
{
    uint32_t size = 0;

    UNPACK_ARRAY(11)
    UNPACK_STR(info, userName, STR_ARRAY_LEN(USER_NAME_MAX_LEN))
    UNPACK_STR(info, assetName, MAX_STR_LEN)
    UNPACK_U64(info, assetType)
    UNPACK_STR(info, featureTag, MAX_STR_LEN)
    UNPACK_STR(info, sampleHash, MAX_STR_LEN)
    UNPACK_STR(info, storageHash, STR_ARRAY_LEN(HASH_ID_LEN))
    UNPACK_U32(info, expireTime)    
    UNPACK_U32(info, opType)  
    UNPACK_STR(info, tokenType, MAX_TOKEN_TYPE_LEN      )
    UNPACK_U64(info, price)    
    UNPACK_STR(info, description, MAX_CONTENT_LEN)
    
    return 1;
}



struct TransferFromParam4XTO  {
	char from[STR_ARRAY_LEN(USER_NAME_MAX_LEN)];
	char to[STR_ARRAY_LEN(USER_NAME_MAX_LEN)];
    char tokenType[STR_ARRAY_LEN(MAX_TOKEN_TYPE_LEN)];
	uint64_t value;
};

static bool unpack_struct(MsgPackCtx *ctx, TransferFromParam4XTO *info)
{
    uint32_t size = 0;

    UNPACK_ARRAY(4)
    UNPACK_STR(info, from, STR_ARRAY_LEN(USER_NAME_MAX_LEN))
    UNPACK_STR(info, to, STR_ARRAY_LEN(USER_NAME_MAX_LEN))
    UNPACK_STR(info, tokenType, STR_ARRAY_LEN(MAX_TOKEN_TYPE_LEN))
    UNPACK_U64(info, value)
    
    return 1;
}

static bool pack_struct(MsgPackCtx *ctx, TransferFromParam4XTO *info)
{ 
    PACK_ARRAY16(4)
    
    PACK_STR16(info,from)
    PACK_STR16(info,to)
    PACK_STR16(info,tokenType)
    PACK_U64(info, value)
    
    return 1;
}
