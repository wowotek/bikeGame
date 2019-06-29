#ifndef ANIMATION_H
#define ANIMATION_H

#include <texture.h>
#include <common.h>

typedef
struct Animation
{
    const char *pName;
    Texture2D_t **ppFrames;
    u8 frameCount;
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
AnimationFromFiles (const char *pName, float duration, u8 frameCount, const char** ppFramesFiles, Sampler2D sampler);

u8 GetCurrentAnimationIndex (Animation_t anim, float time);
Texture2D_t *GetCurrentAnimationFrame (Animation_t anim, float time);

void
DestroyAnimation (Animation_t anim);

#endif // ANIMATION_H