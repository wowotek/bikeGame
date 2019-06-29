#ifndef IMAGE_H
#define IMAGE_H

#include <common.h>

typedef u32 ImageFormat;

extern
ImageFormat IMAGE_FORMAT_RED,
            IMAGE_FORMAT_RG,
            IMAGE_FORMAT_RGB,
            IMAGE_FORMAT_RGBA;

typedef struct
{
    const char* path;
    u8* pixels;
    u32 width, height;
    ImageFormat format;
}
Image;

Image
ImageFromFile (const char* path);

Image
Image_From_Memory (u8* memory, size_t size);

void
DestroyImage (Image image);

#endif // IMAGE_H