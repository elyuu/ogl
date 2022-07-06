/*
===========================================================================
ogl::ogl.c
===========================================================================
*/
#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdbool.h>

#include "core_types.h"
#include "file.inl"
#include "log.h"
#include "math.inl"

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
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // shader stuff
    // --------------
    int success = 0;
    char info_log[512] = { 0 };

    uint32_t vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    ogl_file_src_t vert_shader_src
        = ogl_file_read("D:\\projects\\ogl\\assets\\shaders\\triangle.vert");
    glShaderSource(vertex_shader, 1, &vert_shader_src.data, &vert_shader_src.size);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        log_error("Vertex shader compilation failed: %s", info_log);
    }

    uint32_t fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    ogl_file_src_t frag_shader_src
        = ogl_file_read("D:\\projects\\ogl\\assets\\shaders\\triangle.frag");
    glShaderSource(fragment_shader, 1, &frag_shader_src.data, &frag_shader_src.size);
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
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // glBindVertexArray(0);

    // texture stuff
    //--------------
    uint32_t texture0 = 0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tex_width = 0;
    int tex_height = 0;
    int tex_channels = 0;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* image_bytes = stbi_load(
        "D:\\projects\\ogl\\assets\\textures\\uv_test_1k.png\0",
        &tex_width,
        &tex_height,
        &tex_channels,
        0);

    log_info("%i, %i", tex_width, tex_height);

    if (image_bytes)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            tex_width,
            tex_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image_bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        log_error("Failed to load texture image: %s", stbi_failure_reason());
    }
    stbi_image_free(image_bytes);

    // Render loop
    log_info("STARTING RENDER LOOP");

    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture0"), 1);

    ogl_mat4f_t model = ogl_mat4f_from_rotation_x(-0.872665f);
    //ogl_mat4f_print(&model);

    ogl_mat4f_t tm = ogl_mat4f_identity();
    ogl_vec3f_t tv = { .x = 0.0f, .y = 0.0f, .z = -1.0f };

    ogl_mat4f_t view = { .vec4f = {
                            (ogl_vec4f_t) { 1.0f, 0.0f, 0.0f, 0.0f },
                            (ogl_vec4f_t) { 0.0f, 1.0f, 0.0f, 0.0f },
                            (ogl_vec4f_t) { 0.0f, 0.0f, 1.0f, 0.0f },
                            (ogl_vec4f_t) { 0.0f, 0.0f, -4.0f, 1.0f },
                        } };

    //ogl_mat4f_t view = ogl_mat4f_translate(&tm, &tv);
    // ogl_mat4f_t view = ogl_mat4f_identity();
    //ogl_mat4f_print(&view);

     ogl_mat4f_t proj = ogl_mat4f_perspective(0.785398f, 800.0f / 600.0f, 0.01f, 100.0f);
    //ogl_mat4f_t proj = ogl_mat4f_identity();
    //ogl_mat4f_print(&proj);

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, true, model.vec4f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, true, view.vec4f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, true, proj.vec4f);

    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUseProgram(shader_program);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
