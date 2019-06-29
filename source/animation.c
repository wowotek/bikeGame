#include <math.h>
#include "animation.h"

Animation_t
AnimationFromFiles (const char *pName, float duration, uint8_t frameCount, const char** ppFramesFiles, Sampler2D sampler)
{
    Animation_t anim;
    anim.frameCount = frameCount;
    anim.duration = duration;

    anim.pName = malloc(strlen(pName) + 1);
    strcpy(anim.pName, pName);
    anim.ppFrames = malloc(sizeof(Texture2D_t*) * frameCount);

    for (uint8_t i = 0; i < frameCount; ++i)
    {
        Image image = ImageFromFile(ppFramesFiles[i]);
        anim.ppFrames[i] = NewTexture2D(image, sampler);
        DestroyImage(image);
    }

    return anim;
}

uint8_t GetCurrentAnimationIndex (Animation_t anim, float time)
{
    return (uint8_t) (fmodf(time, anim.duration) / anim.duration * anim.frameCount);
}

Texture2D_t *GetCurrentAnimationFrame(Animation_t anim, float time)
{
    return anim.ppFrames[GetCurrentAnimationIndex(anim, time)];
}

void
DestroyAnimation (Animation_t anim)
{
    for (uint8_t i = 0; i < anim.frameCount; ++i)
    {
        DeleteTexture2D(anim.ppFrames[i]);
    }

    free(anim.pName);
}