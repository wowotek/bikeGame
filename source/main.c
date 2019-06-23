#include <common.h>
#include <image.h>
#include <texture.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#define GLM_FORCE_INLINE
#include <glm/glm.h>


#define WIDTH 480
#define HEIGHT 760

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

struct Motion
{
	vec2 x, v, a;
};

#define MOTION_DEFAULT (struct Motion){(vec2){0.0f}, (vec2){0.0f}, (vec2){0.0f}}

static inline void
integrate_modified_euler (float dt, vec2* x, vec2* v, vec2* a)
{
    *v = mul(*a, dt); 
    *x = mul(*v, dt);
}

static inline void
motion_add_force (struct Motion *const this, const vec2 f)
{
    this->a = add(this->a, f);
}

static inline void
motion_update (struct Motion *const this, float dt)
{
    integrate_modified_euler(dt, &this->x, &this->v, &this->a);
    this->a = vec2(0.0f, 0.0f);
}

struct Camera
{
	struct Motion motion;
	float aspect, zoom;
};

struct Sprite
{
};

struct Entity
{
    struct Motion motion;
    uint8_t spriteID;
};


#define MAX_ENTITY_COUNT 1024
static struct Entity entities[MAX_ENTITY_COUNT];

static void
update (float dt)
{

}

#include <stdio.h>

int main(void)
{
    glfwInit();

    printf("%s\n", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);

    glfwMakeContextCurrent(pWindow);

    gladLoadGL();

    printf("%s\n", glGetString(GL_VERSION));

    Image wheelImage = image_from_file("../data/images/wheel.png");

    printf("path: %s\n", wheelImage.path);
    printf("resolution: %ux%u\n", wheelImage.width, wheelImage.height);
    printf("format: %u\n", wheelImage.format);
    printf("pixels: %p\n", wheelImage.pixels);

    Sampler defaultSampler;

    defaultSampler.wrapS = CLAMP_TO_EDGE;
    defaultSampler.wrapT = CLAMP_TO_EDGE;
    defaultSampler.filterMin = NEAREST;
    defaultSampler.filterMag = NEAREST;

    Texture* wheelTexture = make_texture(wheelImage, defaultSampler);

    printf("%p\n", wheelTexture);

    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bind_texture(wheelTexture, 1);

        glfwSwapBuffers(pWindow);
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}