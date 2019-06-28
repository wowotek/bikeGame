#include "common.h"
#include "image.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#define GLM_FORCE_INLINE
#include <glm/glm.h>

#define WIDTH 500
#define HEIGHT 500

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

typedef struct { vec2 x, v, a; } Motion2D;

typedef struct
{
    vec2 scale;
    vec2 position;
    float angle;
} Transform2D;

typedef struct
{
    Texture2D* pTexture;
} Sprite;

typedef struct
{
    Transform2D transform;
    Motion2D motion;
    u8 spriteID;
} Entity;

typedef struct
{
    Transform2D transform;
    float aspect;
    float zoom;
} Camera2D;


#define MOTION_DEFAULT (Motion){vec2(0), vec2(0), vec2(0)}

static inline void
integrate_modified_euler (float dt, vec2* x, vec2* v, vec2* a)
{
    *v = mul(*a, dt); 
    *x = mul(*v, dt);
}

static inline void
motion_add_force (Motion2D *const this, const vec2 f)
{
    this->a = add(this->a, f);
}

static inline void
motion_update (Motion2D *const this, float dt)
{
    integrate_modified_euler(dt, &this->x, &this->v, &this->a);
    this->a = vec2(0);
}


#define MAX_ENTITY_COUNT 1024
static Entity entities[MAX_ENTITY_COUNT];

int main(void)
{
    glfwInit();

    PRINT_INFO("GLFW Version %s\n", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);

    glfwMakeContextCurrent(pWindow);

    gladLoadGL();

    PRINT_INFO("OpenGL Version %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const char* pVertexStageCode = ReadTextFile("../data/shaders/texturedQuad.vert");
    const char* pPixelStageCode = ReadTextFile("../data/shaders/texturedQuad.frag");

    if (pVertexStageCode == NULL || pPixelStageCode == NULL)
    {
        LOG_ERROR("Could not read shader source files!");
        Delete(pVertexStageCode);
        Delete(pPixelStageCode);
    }

    ShaderStage_t* pVertexShaderStage = New_ShaderStage(pVertexStageCode, SHADER_STAGE_TYPE_VERTEX);
    ShaderStage_t* pPixelShaderStage = New_ShaderStage(pPixelStageCode, SHADER_STAGE_TYPE_PIXEL);

    Shader_t* pTexturedQuadShader = New_Shader_Default(pVertexShaderStage, pPixelShaderStage);

    Vertex_t pQuadVertices[4];
    pQuadVertices[0].position = vec2(-0.5f);
    pQuadVertices[0].texcoord = vec2(0.0f);
    pQuadVertices[1].position = vec2(0.5f, -0.5f);
    pQuadVertices[1].texcoord = vec2(1.0f, 0.0f);
    pQuadVertices[2].position = vec2(-0.5f, 0.5f);
    pQuadVertices[2].texcoord = vec2(0.0f, 1.0f);
    pQuadVertices[3].position = vec2(0.5f);
    pQuadVertices[3].texcoord = vec2(1.0f);

    u32 pQuadIndices[6] = {2,1,0,3,1,2};

    Mesh_t* pQuadMesh = New_Mesh(pTexturedQuadShader, pQuadVertices, 4, pQuadIndices, 6);

    Image policeCarImage = Image_From_File("../data/images/Police.png");

    Sampler defaultSampler;
    defaultSampler.wrapS = CLAMP_TO_EDGE;
    defaultSampler.wrapT = CLAMP_TO_EDGE;
    defaultSampler.filterMin = NEAREST;
    defaultSampler.filterMag = NEAREST;

    Texture2D* pPoliceCarTexture = NewTexture2D(policeCarImage, defaultSampler);

    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        Shader_Bind(pTexturedQuadShader);

        BindTexture2D(pPoliceCarTexture, 0);
        LOG_INFO("Hi!");
        SetSampler2D(pTexturedQuadShader, "iColorTexture", 0);

        Mesh_Draw(pQuadMesh);

        glfwSwapBuffers(pWindow);
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}