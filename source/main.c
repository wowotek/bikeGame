#include "common.h"
#include "image.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "animation.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#define GLM_FORCE_INLINE
#include <glm/glm.h>

static int width = 800, height = 600;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

typedef struct Motion2D { vec2 x, v, a; } Motion2D_t;

typedef
struct Transform2D
{
    vec2 scale;
    vec2 position;
    float angle;
}
Transform2D_t;

typedef
struct Enity
{
    Mesh_t *pMesh;
    Motion2D_t motion;
    Sprite_t *pSprite;
} Entity_t;

typedef
struct Camera2D
{
    vec2 position;
    float angle;
    float aspect;
    float zoom;
    float zoomInterval;
} Camera2D_t;

typedef
struct Vehicle
{
    Entity_t base;
 //   Wheel_t *pWheels;
//    uint8_t wheelCount;
    float speed;
} Vehicle_t;

typedef
struct Material
{
    float friction;
} Material_t;

/*
typedef
struct Wheel
{
    Transform2D_t transform;
} Wheel_t;

Vehicle_t
Vehicle (Wheel_t *pWheels, uint8_t wheelCount, float acceleration)
{
    Vehicle_t vehicle;
    vehicle.pWheels = malloc(sizeof(Wheel_t) * )
}
*/

#define MOTION_DEFAULT (Motion2D_t){vec2(0), vec2(0), vec2(0)}

static void
IntegrateModifiedEuler (float dt, vec2* x, vec2* v, vec2* a)
{
    *v = add(*v, mul(*a, dt)); 
    *x = add(*x, mul(*v, dt));
}

static void
Motion2D_AddForce (Motion2D_t *const this, const vec2 f)
{
    this->a = add(this->a, f);
}

static void
UpdateMotion2D (Motion2D_t *const this, float dt)
{
    IntegrateModifiedEuler(dt, &this->x, &this->v, &this->a);
    this->a = vec2(0);
}

Camera2D_t camera;

typedef struct World
{
    Camera2D_t mainCamera;
} World_t;


static Mesh_t *pQuadMesh;
static Shader_t *pWireframeShader, *pSpriteShader;
static Sprite_t policeCarSprite;
static Vehicle_t policeCar;
static bool enabledWireframe;

static Material_t dirtMaterial;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.zoom += yoffset * camera.zoomInterval;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
        enabledWireframe = !enabledWireframe;
}

void Initialize (void)
{

    {
        // Creating sprite shader   
        const char* pVertexStageCode = ReadTextFile("../data/shaders/sprite.vert");
        const char* pPixelStageCode = ReadTextFile("../data/shaders/sprite.frag");

        if (pVertexStageCode == NULL || pPixelStageCode == NULL)
        {
            LOG_ERROR("Could not read shader source files!");
            Delete(pVertexStageCode);
            Delete(pPixelStageCode);
        }

        ShaderStage_t* pVertexShaderStage = New_ShaderStage(pVertexStageCode, SHADER_STAGE_TYPE_VERTEX);
        ShaderStage_t* pPixelShaderStage = New_ShaderStage(pPixelStageCode, SHADER_STAGE_TYPE_PIXEL);
        pSpriteShader = New_Shader_Default(pVertexShaderStage, pPixelShaderStage);
    }

    {
        // Creating wireframe shader
        const char* pVertexStageCode = ReadTextFile("../data/shaders/wireframe.vert");
        const char* pPixelStageCode = ReadTextFile("../data/shaders/wireframe.frag");

        if (pVertexStageCode == NULL || pPixelStageCode == NULL)
        {
            LOG_ERROR("Could not read shader source files!");
            Delete(pVertexStageCode);
            Delete(pPixelStageCode);
        }

        ShaderStage_t* pVertexShaderStage = New_ShaderStage(pVertexStageCode, SHADER_STAGE_TYPE_VERTEX);
        ShaderStage_t* pPixelShaderStage = New_ShaderStage(pPixelStageCode, SHADER_STAGE_TYPE_PIXEL);
        pWireframeShader = New_Shader_Default(pVertexShaderStage, pPixelShaderStage);
    }

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

    pQuadMesh = New_Mesh(pQuadVertices, 4, pQuadIndices, 6);

    Sampler2D defaultSampler;
    defaultSampler.wrapS = CLAMP_TO_EDGE;
    defaultSampler.wrapT = CLAMP_TO_EDGE;
    defaultSampler.filterMin = LINEAR;
    defaultSampler.filterMag = LINEAR;

    const char *ppPoliceAnimationFrameFilenames[3] = {
        "../data/images/Police_animation/1.png",
        "../data/images/Police_animation/2.png",
        "../data/images/Police_animation/3.png"      
        };

    uint8_t policeFrameCount = 3;
    
    Animation_t policeAnimation = AnimationFromFiles("Police Car Animation",
    0.25f, policeFrameCount, ppPoliceAnimationFrameFilenames, defaultSampler);

    policeCarSprite.anim = policeAnimation;
    policeCarSprite.enableAnimation = true;

    policeCar.base.pMesh = pQuadMesh;
    policeCar.base.pSprite = &policeCarSprite;
    policeCar.base.motion = MOTION_DEFAULT;
    policeCar.speed = 1.0f;

    enabledWireframe = false;

    dirtMaterial.friction = .03f;
}

#define min(x, a) ((x) < (a) ? (x) : (a))
#define max(x, a) ((x) > (a) ? (x) : (a))


void DrawEntity (Entity_t *pEntity, Camera2D_t *pCamera, float time)
{
    Shader_Bind(pSpriteShader);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    SetUniformVec2(pSpriteShader, "iModelPosition", pEntity->motion.x);

    SetUniformVec2(pSpriteShader, "iCameraPosition", pCamera->position);
    SetUniformFloat(pSpriteShader, "iCameraAspect", pCamera->aspect);
    SetUniformFloat(pSpriteShader, "iCameraZoom", pCamera->zoom);
    SetUniformFloat(pSpriteShader, "iCameraAngle", pCamera->angle);

    BindTexture2D( GetCurrentAnimationFrame(pEntity->pSprite->anim, time), 0 );
    SetSampler2D(pSpriteShader, "iColorTexture", 0);
    
    Mesh_Draw(pEntity->pMesh);
}

void DrawEntityWireframe (Entity_t *pEntity, Camera2D_t *pCamera, vec4 color)
{
    Shader_Bind(pWireframeShader);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);

    SetUniformVec2(pSpriteShader, "iModelPosition", pEntity->motion.x);

    SetUniformVec2(pWireframeShader, "iCameraPosition", pCamera->position);
    SetUniformFloat(pWireframeShader, "iCameraAspect", pCamera->aspect);
    SetUniformFloat(pWireframeShader, "iCameraZoom", pCamera->zoom);
    SetUniformFloat(pWireframeShader, "iCameraAngle", pCamera->angle);
    SetUniformVec4(pWireframeShader, "iColor", color);
    Mesh_Draw(pEntity->pMesh);
}

void UpdateVehicle (Vehicle_t *pVehicle, Material_t *pGroundMaterial, float dt)
{
    pVehicle->base.motion.v.x *= max(1.0f - pGroundMaterial->friction * dt, 0.0f);
    pVehicle->base.motion.v.y *= max(1.0f - pGroundMaterial->friction * dt, 0.0f);
    UpdateMotion2D(&pVehicle->base.motion, dt);
}

#define CLAMP(x, a, b) ( ((x) < (a)) ? (a) : ( ((x) > (b)) ? (b) : (x)))

void AccelerateVehicle (Vehicle_t *pVehicle, float a)
{
    a = CLAMP(a, 0.0f, 1.0f);
    Motion2D_AddForce(&pVehicle->base.motion, vec2(0, a * pVehicle->speed));
}

int main (void)
{
    glfwInit();

    PRINT_INFO("GLFW Version %s\n", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(width, height, "Game", NULL, NULL);

    glfwMakeContextCurrent(pWindow);
    glfwSetScrollCallback(pWindow, &scroll_callback);
    glfwSetKeyCallback(pWindow, &key_callback);

    gladLoadGL();

    PRINT_INFO("OpenGL Version %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Initialize();

    camera.angle = 0.0f;
    camera.zoom = 0.0f;
    camera.position = vec2(0);
    camera.aspect = (float)height / (float)width;
    camera.zoomInterval = 0.05f;

/*
    Image policeCarImage = ImageFromFile("../data/images/Mini_truck.png");

    Sampler2D defaultSampler;
    defaultSampler.wrapS = CLAMP_TO_EDGE;
    defaultSampler.wrapT = CLAMP_TO_EDGE;
    defaultSampler.filterMin = LINEAR;
    defaultSampler.filterMag = LINEAR;

    Texture2D_t* pPoliceCarTexture = NewTexture2D(policeCarImage, defaultSampler);
*/

    float time = (float) glfwGetTime(), lastTime, deltaTime;

    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();
        glfwGetWindowSize(pWindow, &width, &height);
        camera.aspect = (float)height / (float)width;

        lastTime = time;
        time = (float) glfwGetTime();
        deltaTime = time - lastTime;

        
        float cameraSpeed = 1.0f;

        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            camera.position.x += deltaTime * cameraSpeed;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            camera.position.x -= deltaTime * cameraSpeed;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            camera.position.y -= deltaTime * cameraSpeed;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
        {
            camera.position.y += deltaTime * cameraSpeed;
        }

        float a = 0.0f;
        
        if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            a = 1.0f;
        }

        AccelerateVehicle(&policeCar, a);
        UpdateVehicle(&policeCar, &dirtMaterial, deltaTime);
        
        PRINT_INFO("%f\n", policeCar.base.motion.x.y);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawEntity((Entity_t*)&policeCar, &camera, time);

        if (enabledWireframe)
        {
            DrawEntityWireframe((Entity_t*)&policeCar, &camera, vec4(1,0,1,1));
        }

        glfwSwapBuffers(pWindow);
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}