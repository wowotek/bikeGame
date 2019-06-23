#include <image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

ImageFormat IMAGE_FORMAT_R8       = GL_R8,
            IMAGE_FORMAT_R8G8     = GL_RG8,
            IMAGE_FORMAT_R8G8B8   = GL_RGB8,
            IMAGE_FORMAT_R8G8B8A8 = GL_RGBA8;

Image
image_from_file (const char* path)
{
    Image image = { 0 };
    int comp;
    image.pixels = stbi_load(path, &image.width, &image.height, &comp, 0);
    image.path = path;
    
    switch (comp)
    {
        case 1: image.format = IMAGE_FORMAT_R8; break;
        case 2: image.format = IMAGE_FORMAT_R8G8; break;
        case 3: image.format = IMAGE_FORMAT_R8G8B8; break;
        case 4: image.format = IMAGE_FORMAT_R8G8B8A8; break;
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