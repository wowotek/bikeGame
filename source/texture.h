#ifndef TEXTURE_H
#define TEXTURE_H

#include <common.h>
#include <image.h>

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

typedef struct Texture Texture;

Texture*
new_texture (Image, Sampler);

void
bind_texture (Texture*, u32 index);

void
destroy_texture (Texture*);

#endif // TEXTURE_H