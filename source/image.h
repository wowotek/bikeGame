#ifndef IMAGE_H
#define IMAGE_H

#include <common.h>

typedef u32 ImageFormat;

extern ImageFormat
    IMAGE_FORMAT_R8,
    IMAGE_FORMAT_R8G8,
    IMAGE_FORMAT_R8G8B8,
    IMAGE_FORMAT_R8G8B8A8;

typedef struct
{
    const char* path;
    u8* pixels;
    u32 width, height;
    ImageFormat format;
}
Image;

Image
image_from_file (const char* path);

Image
image_from_memory (u8* memory, size_t size);

void
destroy_image (Image image);

#endif // IMAGE_H