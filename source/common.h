#ifndef COMMON_H
#define COMMON_H

#include <glm/glm.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

static void *
_New (size_t _s)
{
    return malloc(_s);
}

#define New(T) (T*)_New(sizeof(T))
#define Delete(p) free(p)

#define ASSERT(cond, message) assert((cond) && message)
#define LOG_INFO(err) printf("[INFO]: %s\n", err)
#define LOG_ERROR(err) printf("[ERROR]: %s\n", err)
#define PRINT_INFO(fmt, ...) printf("[INFO]: " fmt, __VA_ARGS__)
#define PRINT_ERROR(fmt, ...) printf("[INFO]: " fmt, __VA_ARGS__)

// Returns string which you have reponsibility to free.
static char *
ReadTextFile (const char* pPath)
{
    FILE *pFile = fopen(pPath, "rb");
    char *pData = NULL;

    if (pFile == NULL) return NULL;

    fseek(pFile, 0, SEEK_END);
    size_t size = ftell(pFile);
    pData = malloc(size + 1);

    fseek(pFile, 0, SEEK_SET);

    fread(pData, 1, size, pFile);
    pData[size] = '\0';

    return pData;
}

#endif // COMMON_H