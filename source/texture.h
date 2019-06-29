#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "image.h"
#include "shader.h"

typedef u32 Wrapping, Filtering;

extern Wrapping CLAMP_TO_EDGE,
                CLAMP,
                CLAMP_TO_BORDER,
                REPEAT,
                MIRRORED_REPEAT;

extern Filtering NEAREST,
                 LINEAR;

typedef struct
{
    Wrapping wrapS, wrapT;
    Filtering filterMin, filterMag;
} Sampler2D;

typedef struct Texture2D Texture2D_t;

Texture2D_t *NewTexture2D (Image image, Sampler2D sampler);
void BindTexture2D (const Texture2D_t* this, u32 index);
void DeleteTexture2D (const Texture2D_t* this);
i32 GetTexture2D (const Texture2D_t* this);

#endif // TEXTURE_H