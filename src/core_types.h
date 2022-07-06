#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum ogl_result
{
    OGL_OK,
    OGL_ERR,
} ogl_result;

// static string slice only
typedef struct ogl_str_t
{
    const char* data;
    size_t size;
} ogl_str_t;

// math types
typedef union ogl_vec2f_t
{
    float vec2f[2];
    struct
    {
        float x, y;
    };
    struct
    {
        float u, v;
    };
    struct
    {
        float r, g;
    };
} ogl_vec2f_t;

typedef union ogl_vec3f_t
{
    float vec3f[3];
    struct
    {
        float x, y, z;
    };
    struct
    {
        float u, v, w;
    };
    struct
    {
        float r, g, b;
    };
} ogl_vec3f_t;

typedef union ogl_vec4f_t
{
    float vec4f[4];
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
} ogl_vec4f_t;

typedef union ogl_mat4f_t
{
    ogl_vec4f_t vec4f[4];
#if 0
    float blob[16];
    float mat4f[4][4];
    struct
    {
        float xx, xy, xz, xw;
        float yx, yy, yz, yw;
        float zx, zy, zz, zw;
        float wx, wy, wz, ww;
    };
#endif
} ogl_mat4f_t;
