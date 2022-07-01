/*
===========================================================================
ogl::ogl.c
===========================================================================
*/
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdbool.h>

#include "core_types.h"
#include "file.inl"
#include "log.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#if 0
const char* vert_shader_src = "#version 330 core\n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "void main()\n"
                              "{\n"
                              "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                              "}\0";
const char* frag_shader_src = "#version 330 core\n"
                              "out vec4 FragColor;\n"
                              "void main()\n"
                              "{\n"
                              "   FragColor = ourColor;\n"
                              "}\n\0";
#endif

int main(int argc, char** argv)
{
    // SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow(
        "ogl",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window)
    {
        log_fatal("Error creating window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // GL
    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        log_fatal("Failed to init OpenGL context");
        return 1;
    };

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    // triangle stuff
    // --------------
    int success = 0;
    char info_log[512] = { 0 };

    uint32_t vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    ogl_file_src_t vert_shader_src = ogl_file_read("D:\\projects\\ogl\\assets\\shaders\\triangle.vert");
    glShaderSource(vertex_shader, 1, &vert_shader_src.data, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        log_error("Vertex shader compilation failed: %s", info_log);
    }

    uint32_t fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    ogl_file_src_t frag_shader_src = ogl_file_read("D:\\projects\\ogl\\assets\\shaders\\triangle.frag");
    glShaderSource(fragment_shader, 1, &frag_shader_src.data, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        log_error("Fragment shader compilation failed: %s", info_log);
    }

    uint32_t shader_program = 0;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        log_error("Shader program linking failed: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    ogl_file_src_destroy(&vert_shader_src);
    ogl_file_src_destroy(&frag_shader_src);

    float vertices[] = {
        // positions         // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f // top
    };

    uint32_t vao = 0;
    uint32_t vbo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Render loop
    log_info("STARTING RENDER LOOP");

    SDL_Event event;
    bool quit = false;
    glUseProgram(shader_program);
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                log_info("KEY PRESSED");
                break;
            case SDL_QUIT:
                log_info("QUIT");
                quit = true;
                break;
            default:
                break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}