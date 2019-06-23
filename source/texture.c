#include <texture.h>

#include <glad/glad.h>

Wrapping CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
         CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
         REPEAT          = GL_REPEAT,
         MIRRORED_REPEAT = GL_MIRRORED_REPEAT;

Filtering NEAREST = GL_NEAREST,
          LINEAR  = GL_LINEAR;

struct Texture
{
    GLuint id;  
};

Texture*
make_texture (Image image, Sampler sampler)
{
    Texture* this = malloc(sizeof(Texture));
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.filterMag);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void
bind_texture (const Texture *const this, u32 index)
{
    glBindTexture(GL_TEXTURE_2D, this->id);
    glActiveTexture(GL_TEXTURE0 + index);
}

void
destroy_texture (Texture *const this)
{
    glDeleteTextures(1, &this->id);
    free(this);
}