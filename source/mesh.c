#include "mesh.h"

#include <glad/glad.h>
#include <stddef.h>

struct Mesh
{
    GLuint vao, vbo, ibo;
    bool isIndexed;
    size_t drawCount;
};


Mesh_t *
New_Mesh (const Shader_t* pShader, const Vertex_t* pVertices, size_t vertexCount, const u32* pIndices, size_t indexCount)
{
    Mesh_t* pMesh = New(Mesh_t);
    
    glGenVertexArrays(1, &pMesh->vao);
    glBindVertexArray(pMesh->vao);

    {
        glGenBuffers(1, &pMesh->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pMesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex_t), pVertices, GL_STATIC_DRAW);

        unsigned int posLocation = Shader_GetAttribLocation(pShader, "inPosition");
        unsigned int uvLocation = Shader_GetAttribLocation(pShader, "inTexcoord");

        GLsizei stride = sizeof(Vertex_t);
        glEnableVertexAttribArray(posLocation);
        glEnableVertexAttribArray(uvLocation);
        glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, stride, (const void*)0);
        glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offsetof(Vertex_t, texcoord));
    }

    //pMesh->isIndexed = false;
    //pMesh->drawCount = vertexCount;

    if ( pMesh->isIndexed = (pIndices != NULL && indexCount > 0) )
    {
        glGenBuffers(1, &pMesh->ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indexCount, pIndices, GL_STATIC_DRAW);
    
        pMesh->drawCount = indexCount;
    }
    else
    {
        pMesh->drawCount = vertexCount;
    }


    glBindVertexArray(0);

    return pMesh;
}

void
Mesh_Draw (Mesh_t* pMesh)
{
    assert(pMesh != NULL);
    glBindVertexArray(pMesh->vao);

    if (pMesh->isIndexed)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->ibo);
        glDrawElements(GL_TRIANGLES, pMesh->drawCount, GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, pMesh->vbo);
        glDrawArrays(GL_TRIANGLES, 0, pMesh->drawCount);
    }

    glBindVertexArray(0);
}
