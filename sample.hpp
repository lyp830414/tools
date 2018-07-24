#include "/home/bottos/EOS/tools/types.h"

//@abi table assetreginfo:[index_type:string, key_names:abc, key_types:string]
struct AssetInfo {
    string userName;
    string assetName;
    uint64_t assetType;
    string featureTag;
    string sampleHash;
    string storageHash;
    uint32_t      expireTime;
    uint32_t      opType;
    string          tokenType;
    uint64_t      price; 
    char acc[12];
    string description;
   
    
};

//@abi table assetreginfo:[index_type:string, key_names:abc, key_types:string]
struct BssetInfo {
    string userName;
    string assetName;
    uint64_t assetType;
    string featureTag;
    string sampleHash;
    string storageHash;
    uint32_t      expireTime;
    uint32_t      opType;
    string          tokenType;
    uint64_t      price; 
    char Bcc[12];
    string description;
   
    
};

//@abi action assetreg
struct RegAssetReq {
    string  assetId;
    AssetInfo  info;
};


//@abi action Bssetreg
struct RegBssetReq {
    string  assetId;
    AssetInfo  info;
};
