#ifndef SHADER_H
#define SHADER_H

#include "common.h"

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

int Shader_GetAttribLocation (Shader_t* pShader, const char* pAttribName);
int Shader_GetUniformLocation (Shader_t* pShader, const char* pUniformName);

void
Shader_Delete (Shader_t* pShader);

#endif // SHADER_H