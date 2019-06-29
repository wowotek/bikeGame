#ifndef ANIMATION_H
#define ANIMATION_H

#include "texture.h"

typedef
struct Animation
{
    const char *pName;
    Texture2D_t **ppFrames;
    uint8_t frameCount;
    float duration;
} Animation_t;

typedef
struct Sprite
{
    Texture2D_t *pTex;
    Animation_t anim;
    bool enableAnimation;
}
Sprite_t;

Animation_t
AnimationFromFiles (const char *pName, float duration, uint8_t frameCount, const char** ppFramesFiles, Sampler2D sampler);

uint8_t GetCurrentAnimationIndex (Animation_t anim, float time);
Texture2D_t *GetCurrentAnimationFrame (Animation_t anim, float time);

void
DestroyAnimation (Animation_t anim);

#endif // ANIMATION_H