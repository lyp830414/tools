#pragma once

#include "types.h"

enum {
  MSG_PACK_TYPE_BIN16 = 0,
  MSG_PACK_TYPE_UINT8,
  MSG_PACK_TYPE_UINT16,
  MSG_PACK_TYPE_UINT32,
  MSG_PACK_TYPE_UINT64,
  MSG_PACK_TYPE_STR16,
  MSG_PACK_TYPE_ARRAY16
};

enum {
  ERROR_NONE,
  ERROR_SHORT_BUFFER,
  ERROR_UNPACK_TYPE_MARKER,
  ERROR_PACK_TYPE_MARKER,
  ERROR_UNPACK_DATA,
  ERROR_PACK_DATA,
  ERROR_WRONG_TYPE,
  ERROR_UNPACK_LENGTH,
  ERROR_PACK_LENGTH
};

union ObjectData {
  uint8_t   u8;
  uint16_t  u16;
  uint32_t  u32;
  uint64_t  u64;
  uint32_t  array_size;
  uint32_t  str_size;
  uint32_t  bin_size;
};

typedef struct {
  uint8_t   error;
  char      *buf;
  uint32_t  buf_len;
  uint32_t  pos;
} MsgPackCtx;

enum {
  BIN16_MARKER           = 0xC5,
  U8_MARKER              = 0xCC,
  U16_MARKER             = 0xCD,
  U32_MARKER             = 0xCE,
  U64_MARKER             = 0xCF,
  STR16_MARKER           = 0xDA,
  ARRAY16_MARKER         = 0xDC,
  MAP16_MARKER           = 0xDE,
};

typedef struct {
  uint8_t type;
  union ObjectData as;
} MPObject;

static const int32_t _i = 1;
#define is_bigendian() ((*(char *)&_i) == 0)

static uint16_t be16(uint16_t x) {
  char *b = (char *)&x;

  if (!is_bigendian()) {
    char swap = 0;

    swap = b[0];
    b[0] = b[1];
    b[1] = swap;
  }

  return x;
}

static uint32_t be32(uint32_t x) {
  char *b = (char *)&x;

  if (!is_bigendian()) {
    char swap = 0;

    swap = b[0];
    b[0] = b[3];
    b[3] = swap;

    swap = b[1];
    b[1] = b[2];
    b[2] = swap;
  }

  return x;
}

static uint64_t be64(uint64_t x) {
  char *b = (char *)&x;

  if (!is_bigendian()) {
    char swap = 0;

    swap = b[0];
    b[0] = b[7];
    b[7] = swap;

    swap = b[1];
    b[1] = b[6];
    b[6] = swap;

    swap = b[2];
    b[2] = b[5];
    b[5] = swap;

    swap = b[3];
    b[3] = b[4];
    b[4] = swap;
  }

  return x;
}

static void* memcpy (void *dest, const void *src, int len)
{
  char *d = (char*)dest;
  const char *s = (char*)src;
  while (len--)
    *d++ = *s++;
  return dest;
}

static int read(MsgPackCtx *ctx, void *data, int len)
{
  if (ctx->buf)
  {
    if (ctx->pos + len <= ctx->buf_len)
    {
      memcpy(data, &ctx->buf[ctx->pos], len);
      ctx->pos += len;
      return 1;
    }
  }
  
  return 0;
}

static  int write(MsgPackCtx *ctx, const void *data, int len)
{
  if (ctx->buf)
  {
    if (ctx->pos + len <= ctx->buf_len)
    {
      memcpy(&ctx->buf[ctx->pos], data, len);
      ctx->pos += len;
      return len;
    }
  }
  
  return 0;
}

static int read_byte(MsgPackCtx *ctx, uint8_t *x) {
  return read(ctx, x, sizeof(uint8_t));
}

static int write_byte(MsgPackCtx *ctx, uint8_t x) {
  return (write(ctx, &x, sizeof(uint8_t)) == (sizeof(uint8_t)));
}

static int read_type_marker(MsgPackCtx *ctx, uint8_t *marker) {
  if (read_byte(ctx, marker)) {
    return 1;
  }

  ctx->error = ERROR_UNPACK_TYPE_MARKER;
  return 0;
}

static int write_type_marker(MsgPackCtx *ctx, uint8_t marker) {
  if (write_byte(ctx, marker))
    return 1;

  ctx->error = ERROR_PACK_TYPE_MARKER;
  return 0;
}

static int convert_type_marker(uint8_t type_marker, uint8_t *type) {
  switch (type_marker) {
    case BIN16_MARKER:
      *type = MSG_PACK_TYPE_BIN16;
      return 1;
    case U8_MARKER:
      *type = MSG_PACK_TYPE_UINT8;
      return 1;
    case U16_MARKER:
      *type = MSG_PACK_TYPE_UINT16;
      return 1;
    case U32_MARKER:
      *type = MSG_PACK_TYPE_UINT32;
      return 1;
    case U64_MARKER:
      *type = MSG_PACK_TYPE_UINT64;
      return 1;
    case STR16_MARKER:
      *type = MSG_PACK_TYPE_STR16;
      return 1;
    case ARRAY16_MARKER:
      *type = MSG_PACK_TYPE_ARRAY16;
      return 1;
    default:
      return 0;
  }
}

static int read_type_size(MsgPackCtx *ctx, uint8_t type_marker,
                                           uint8_t type,
                                           uint32_t *size) {
  uint8_t u8temp = 0;
  uint16_t u16temp = 0;
  uint32_t u32temp = 0;

  switch (type) {
    case MSG_PACK_TYPE_BIN16:
      if (!read(ctx, &u16temp, sizeof(uint16_t))) {
        ctx->error = ERROR_UNPACK_LENGTH;
        return 0;
      }
      *size = be16(u16temp);
      return 1;
    
    case MSG_PACK_TYPE_UINT8:
      *size = 1;
      return 1;
    case MSG_PACK_TYPE_UINT16:
      *size = 2;
      return 1;
    case MSG_PACK_TYPE_UINT32:
      *size = 4;
      return 1;
    case MSG_PACK_TYPE_UINT64:
      *size = 8;
      return 1;
    case MSG_PACK_TYPE_STR16:
      if (!read(ctx, &u16temp, sizeof(uint16_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      *size = be16(u16temp);
      return 1;
    case MSG_PACK_TYPE_ARRAY16:
      if (!read(ctx, &u16temp, sizeof(uint16_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      *size = be16(u16temp);
      return 1;
    default:
      ctx->error = ERROR_WRONG_TYPE;
      return 0;
  }
}

static int read_obj_data(MsgPackCtx *ctx, uint8_t type_marker,
                                          MPObject *obj) {
  switch (obj->type) {
    case MSG_PACK_TYPE_UINT8:
      if (!read(ctx, &obj->as.u8, sizeof(uint8_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      return 1;
    case MSG_PACK_TYPE_UINT16:
      if (!read(ctx, &obj->as.u16, sizeof(uint16_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      obj->as.u16 = be16(obj->as.u16);
      return 1;
    case MSG_PACK_TYPE_UINT32:
      if (!read(ctx, &obj->as.u32, sizeof(uint32_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      obj->as.u32 = be32(obj->as.u32);
      return 1;
    case MSG_PACK_TYPE_UINT64:
      if (!read(ctx, &obj->as.u64, sizeof(uint64_t))) {
        ctx->error = ERROR_UNPACK_DATA;
        return 0;
      }
      obj->as.u64 = be64(obj->as.u64);
      return 1;
    case MSG_PACK_TYPE_BIN16:
      return read_type_size(ctx, type_marker, obj->type, &obj->as.bin_size);
    case MSG_PACK_TYPE_STR16:
      return read_type_size(ctx, type_marker, obj->type, &obj->as.str_size);
    case MSG_PACK_TYPE_ARRAY16:
      return read_type_size(ctx, type_marker, obj->type, &obj->as.array_size);
    default:
      break;
  }

  ctx->error = ERROR_WRONG_TYPE;
  return 0;
}

static int unpack_object(MsgPackCtx *ctx, MPObject *obj) {
  uint8_t type_marker = 0;

  if (!read_type_marker(ctx, &type_marker))
    return 0;

  if (!convert_type_marker(type_marker, &obj->type)) {
    ctx->error = ERROR_WRONG_TYPE;
    return 0;
  }

  return read_obj_data(ctx, type_marker, obj);
}

static void msgpack_init(MsgPackCtx *ctx, char *buf, uint32_t buf_len)
{
  ctx->error = ERROR_NONE;
  ctx->buf = buf;
  ctx->buf_len = buf_len;
  ctx->pos = 0;
}

static int pack_u8(MsgPackCtx *ctx, uint8_t c) {
  if (!write_type_marker(ctx, U8_MARKER))
    return 0;

  return write(ctx, &c, sizeof(uint8_t));
}

static int pack_u16(MsgPackCtx *ctx, uint16_t s) {
  if (!write_type_marker(ctx, U16_MARKER))
    return 0;

  s = be16(s);

  return write(ctx, &s, sizeof(uint16_t));
}

static int pack_u32(MsgPackCtx *ctx, uint32_t i) {
  if (!write_type_marker(ctx, U32_MARKER))
    return 0;

  i = be32(i);
  
  return write(ctx, &i, sizeof(uint32_t));
}

static int pack_u64(MsgPackCtx *ctx, uint64_t l) {
  if (!write_type_marker(ctx, U64_MARKER))
    return 0;

  l = be64(l);

  return write(ctx, &l, sizeof(uint64_t));
}

static int pack_str16_marker(MsgPackCtx *ctx, uint16_t size) {
  if (!write_type_marker(ctx, STR16_MARKER))
    return 0;

  size = be16(size);

  if (write(ctx, &size, sizeof(uint16_t)))
    return 1;

  ctx->error = ERROR_PACK_LENGTH;
  return 0;
}

static int pack_str16(MsgPackCtx *ctx, const char *data, uint16_t size) {
  if (!pack_str16_marker(ctx, size))
    return 0;
    
  if (size == 0)
    return 1;
    
  if (write(ctx, data, size))
    return 1;

  ctx->error = ERROR_PACK_DATA;
  return 0;
}

static int pack_bin16_marker(MsgPackCtx *ctx, uint16_t size) {
  if (!write_type_marker(ctx, BIN16_MARKER))
    return 0;

  size = be16(size);

  if (write(ctx, &size, sizeof(uint16_t)))
    return 1;

  ctx->error = ERROR_PACK_LENGTH;
  return 0;
}

static int pack_bin16(MsgPackCtx *ctx, const void *data, uint16_t size) {
  if (!pack_bin16_marker(ctx, size))
    return 0;

  if (size == 0)
    return 1;

  if (write(ctx, data, size))
    return 1;

  ctx->error = ERROR_PACK_DATA;
  return 0;
}

static int pack_array16(MsgPackCtx *ctx, uint16_t size) {
  if (!write_type_marker(ctx, ARRAY16_MARKER))
    return 0;

  size = be16(size);

  if (write(ctx, &size, sizeof(uint16_t)))
    return 1;

  ctx->error = ERROR_PACK_LENGTH;
  return 0;
}

static int unpack_u8(MsgPackCtx *ctx, uint8_t *c) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  if (obj.type != MSG_PACK_TYPE_UINT8) {
    ctx->error = ERROR_WRONG_TYPE;
    return 0;
  }

  *c = obj.as.u8;
  return 1;
}

static int unpack_u16(MsgPackCtx *ctx, uint16_t *s) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  if (obj.type != MSG_PACK_TYPE_UINT16) {
    ctx->error = ERROR_WRONG_TYPE;
    return 0;
  }

  *s = obj.as.u16;
  return 1;
}

static int unpack_u32(MsgPackCtx *ctx, uint32_t *i) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  if (obj.type != MSG_PACK_TYPE_UINT32) {
    ctx->error = ERROR_WRONG_TYPE;
    return 0;
  }

  *i = obj.as.u32;
  return 1;
}

static int unpack_u64(MsgPackCtx *ctx, uint64_t *l) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  if (obj.type != MSG_PACK_TYPE_UINT64) {
    ctx->error = ERROR_WRONG_TYPE;
    return 0;
  }

  *l = obj.as.u64;
  return 1;
}

static int unpack_str_size(MsgPackCtx *ctx, uint32_t *size) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  switch (obj.type) {
    case MSG_PACK_TYPE_STR16:
      *size = obj.as.str_size;
      return 1;
    default:
      ctx->error = ERROR_WRONG_TYPE;
      return 0;
  }
}

static int unpack_str(MsgPackCtx *ctx, char *data, uint32_t buf_size, uint32_t *strsize) {
  uint32_t str_size = 0;

  if (!unpack_str_size(ctx, &str_size))
    return 0;

  if ((str_size + 1) > buf_size) {
    *strsize = str_size;
    ctx->error = ERROR_SHORT_BUFFER;
    return 0;
  }

  if (!read(ctx, data, str_size)) {
    ctx->error = ERROR_UNPACK_DATA;
    return 0;
  }

  data[str_size] = 0;

  *strsize = str_size;
  return 1;
}

static int unpack_bin_size(MsgPackCtx *ctx, uint32_t *size) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  switch (obj.type) {
    case MSG_PACK_TYPE_BIN16:
      *size = obj.as.bin_size;
      return 1;
    default:
      ctx->error = ERROR_WRONG_TYPE;
      return 0;
  }
}

static int unpack_bin(MsgPackCtx *ctx, void *data, uint32_t *size) {
  uint32_t bin_size = 0;

  if (!unpack_bin_size(ctx, &bin_size))
    return 0;

  if (bin_size > *size) {
    ctx->error = ERROR_SHORT_BUFFER;
    return 0;
  }

  if (!read(ctx, data, bin_size)) {
    ctx->error = ERROR_UNPACK_DATA;
    return 0;
  }

  *size = bin_size;
  return 1;
}

static int unpack_array(MsgPackCtx *ctx, uint32_t *size) {
  MPObject obj;

  if (!unpack_object(ctx, &obj))
    return 0;

  switch (obj.type) {
    case MSG_PACK_TYPE_ARRAY16:
      *size = obj.as.array_size;
      return 1;
    default:
      ctx->error = ERROR_WRONG_TYPE;
      return 0;
  }
}



