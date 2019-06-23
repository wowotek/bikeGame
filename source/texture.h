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
make_texture (Image, Sampler);

void
bind_texture (const Texture *const, u32 index);

void
destroy_texture (Texture *const);

#endif // TEXTURE_H