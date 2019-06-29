#ifndef MESH_H
#define MESH_H

#include "common.h"
#include "shader.h"

typedef struct Vertex
{
    vec2 position;
    vec2 texcoord;
} Vertex_t;

typedef struct Mesh Mesh_t;

Mesh_t *
New_Mesh (const Vertex_t* pVertices, size_t vertexCount, const u32* pIndices, size_t indexCount);

void
Mesh_Draw(Mesh_t *);

void Mesh_Delete(Mesh_t *);

#endif // MESH_H