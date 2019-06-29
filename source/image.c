#include <image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

ImageFormat IMAGE_FORMAT_RED  = GL_RED,
            IMAGE_FORMAT_RG   = GL_RG,
            IMAGE_FORMAT_RGB  = GL_RGB,
            IMAGE_FORMAT_RGBA = GL_RGBA;

Image
ImageFromFile (const char* path)
{
    Image image = { 0 };
    int comp = 4;
    image.pixels = stbi_load(path, &image.width, &image.height, NULL, STBI_rgb_alpha);
    
    image.path = path;
    
    switch (comp)
    {
        case STBI_grey: image.format = IMAGE_FORMAT_RED; break;
        case STBI_grey_alpha: image.format = IMAGE_FORMAT_RG; break;
        case STBI_rgb: image.format = IMAGE_FORMAT_RGB; break;
        case STBI_rgb_alpha: image.format = IMAGE_FORMAT_RGBA; break;
    }
    
    return image;
}

Image
Image_From_Memory (u8* memory, size_t size)
{
    Image image = { 0 };
    int comp;
    image.pixels = stbi_load_from_memory(memory, size, &image.width, &image.height, &comp, 0);

    image.path = "";
    return image;
} 

void
DestroyImage (Image image)
{
    stbi_image_free(image.pixels);
}