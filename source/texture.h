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
} Sampler;

typedef struct Texture2D Texture2D;

Texture2D *
NewTexture2D (Image, Sampler);

void
BindTexture2D (Texture2D* this, u32 index);

void
DeleteTexture2D (Texture2D* this);

i32 GetTextureID(const Texture2D* this);

#endif // TEXTURE_H