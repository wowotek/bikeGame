#ifndef SHADER_H
#define SHADER_H

#include <common.h>

typedef struct Shader Shader_t;
typedef struct ShaderStage ShaderStage_t;

typedef u32 ShaderStageType_t;

extern ShaderStageType_t SHADER_STAGE_TYPE_VERTEX;
extern ShaderStageType_t SHADER_STAGE_TYPE_PIXEL;

//typedef enum ShaderStageType ShaderStageType_t;

// ShaderStage
ShaderStage_t *
New_ShaderStage (const char* pStageSource, ShaderStageType_t stageType);

void
ShaderStage_Delete (ShaderStage_t* shaderStage);


// Shader

// The following function doesn't accept NULL arguments.
Shader_t *
New_Shader_Default (ShaderStage_t* pVertexStage, ShaderStage_t* pPixelStage);

void
Shader_Bind (Shader_t* pShader);

int GetAttribLocation (Shader_t* this, const char* pAttribName);
int GetUniformLocation (Shader_t* this, const char* pUniformName);
void SetSampler2D (const Shader_t* this, const char* name, i32 value);
void SetUniformFloat (Shader_t *this, const char *name, float value);
void SetUniformVec2 (Shader_t *this, const char *name, vec2 value);
void SetUniformVec4 (Shader_t *this, const char *name, vec4 value);

void
Shader_Delete (Shader_t* pShader);

#endif // SHADER_H