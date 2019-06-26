#include "shader.h"

#include <glad/glad.h>

extern ShaderStageType_t SHADER_STAGE_TYPE_VERTEX = GL_VERTEX_SHADER;
extern ShaderStageType_t SHADER_STAGE_TYPE_PIXEL = GL_FRAGMENT_SHADER;

//#define GET_OBJECT_NAME(pObject) (pObject->objectBase->pName)

struct ShaderStage
{
    ShaderStageType_t type;
    GLuint id;
};

// ShaderStage
ShaderStage_t *
New_ShaderStage (const char* pStageSource, ShaderStageType_t stageType)
{
    ShaderStage_t *pShaderStage = New(ShaderStage_t);

    pShaderStage->type = stageType;
    pShaderStage->id = glCreateShader(stageType);
    glShaderSource(pShaderStage->id, 1, &pStageSource, NULL);
    glCompileShader(pShaderStage->id);

#ifndef NDEBUG
    GLboolean success = GL_FALSE;
    glGetShaderiv(pShaderStage->id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLsizei logLength;
        glGetShaderiv(pShaderStage->id, GL_INFO_LOG_LENGTH, &logLength);
        GLchar infoLog[logLength + 1];
        glGetShaderInfoLog(pShaderStage->id, logLength, &logLength, infoLog);

        LOG_INFO(infoLog);
        LOG_ERROR("Failed to create ShaderStage!");

        ShaderStage_Delete(pShaderStage);
        return NULL;
    }
#endif

    return pShaderStage;
}

void
ShaderStage_Delete (ShaderStage_t* pShaderStage)
{
    ASSERT(pShaderStage, "pShaderStage is NULL!");
    glDeleteShader(pShaderStage->id);
}


// Shader

struct Shader
{
    GLuint id;
};

Shader_t *
New_Shader_Default (ShaderStage_t* pVertexStage, ShaderStage_t* pPixelStage)
{
    ASSERT(pVertexStage != NULL && pPixelStage != NULL, "pVertexStage or pPixelStage are NULL!");

    if ( !(pVertexStage->type == SHADER_STAGE_TYPE_VERTEX
    && pPixelStage->type == SHADER_STAGE_TYPE_PIXEL) )
    {
        LOG_ERROR("Failed to create shader!");
        return NULL;
    }

    Shader_t* pShader = New(Shader_t);
    pShader->id = glCreateProgram();

    glAttachShader(pShader->id, pVertexStage->id);
    glAttachShader(pShader->id, pPixelStage->id);
    
    glLinkProgram(pShader->id);

    glDetachShader(pShader->id, pVertexStage->id);
    glDetachShader(pShader->id, pPixelStage->id);

#ifndef NDEBUG
    GLboolean success = GL_FALSE;
    glGetProgramiv(pShader->id, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLsizei logLength;
        glGetProgramiv(pShader->id, GL_INFO_LOG_LENGTH, (GLint*)&logLength);
        GLchar infoLog[logLength + 1];
        glGetProgramInfoLog(pShader->id, logLength, &logLength, infoLog);

        LOG_INFO(infoLog);
        LOG_ERROR("Failed to create Shader!");

        Shader_Delete(pShader);
        return NULL;
    }
#endif

    return pShader;
}

void
Shader_Delete (Shader_t* pShader)
{
    assert(pShader != NULL);
    glDeleteProgram(pShader->id);
    Delete(pShader);
}

void
Shader_Bind (Shader_t* pShader)
{
    assert(pShader != NULL);
    glUseProgram(pShader->id);
}


int Shader_GetAttribLocation (Shader_t* pShader, const char* pAttribName)
{
    return glGetAttribLocation(pShader->id, pAttribName);
}

int Shader_GetUniformLocation (Shader_t* pShader, const char* pUniformName)
{
    return glGetUniformLocation(pShader->id, pUniformName);
}
