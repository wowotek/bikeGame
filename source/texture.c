#include <texture.h>

#include <glad/glad.h>

Wrapping CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
         CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
         REPEAT          = GL_REPEAT,
         MIRRORED_REPEAT = GL_MIRRORED_REPEAT;

Filtering NEAREST = GL_NEAREST,
          LINEAR  = GL_LINEAR;

struct Texture2D
{
    GLuint id;  
};

Texture2D_t *
NewTexture2D (Image image, Sampler2D sampler)
{
    Texture2D_t *pTexture = New(Texture2D_t);

    glGenTextures(1, &pTexture->id);
    glBindTexture(GL_TEXTURE_2D, pTexture->id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.filterMag);

    glBindTexture(GL_TEXTURE_2D, 0);

    return pTexture;
}

void
BindTexture2D (const Texture2D_t* this, u32 index)
{
    glBindTexture(GL_TEXTURE_2D, this->id);
    glActiveTexture(GL_TEXTURE0 + index);
}

void
DeleteTexture2D (const Texture2D_t *this)
{
    glDeleteTextures(1, &this->id);
    free(this);
}

i32 GetTexture2D (const Texture2D_t* this)
{
    return this->id;
}