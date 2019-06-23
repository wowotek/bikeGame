#include <image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

ImageFormat IMAGE_FORMAT_RED  = GL_RED,
            IMAGE_FORMAT_RG   = GL_RG,
            IMAGE_FORMAT_RGB  = GL_RGB,
            IMAGE_FORMAT_RGBA = GL_RGBA;

Image
image_from_file (const char* path)
{
    Image image = { 0 };
    int comp = 4;
    image.pixels = stbi_load(path, &image.width, &image.height, NULL, STBI_rgb_alpha);
    
    image.path = path;
    
    switch (comp)
    {
        case 1: image.format = IMAGE_FORMAT_RED; break;
        case 2: image.format = IMAGE_FORMAT_RG; break;
        case 3: image.format = IMAGE_FORMAT_RGB; break;
        case 4: image.format = IMAGE_FORMAT_RGBA; break;
    }
    
    return image;
}

Image
image_from_memory (u8* memory, size_t size)
{
    Image image = { 0 };
    int comp;
    image.pixels = stbi_load_from_memory(memory, size, &image.width, &image.height, &comp, 0);

    image.path = "";
    return image;
} 

void
destroy_image (Image image)
{
    stbi_image_free(image.pixels);
}