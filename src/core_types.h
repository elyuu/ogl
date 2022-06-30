#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum ogl_result
{
    ogl_OK,
    ogl_ERR,
} ogl_result;

// static string slice only
typedef struct ogl_str_t
{
    const char* data;
    size_t size;
} ogl_str_t;

// math types
typedef union ogl_v2f_t
{
    float v2f[2];
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
} ogl_v2f_t;

typedef union ogl_v3f_t
{
    float v3f[3];
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
} ogl_v3f_t;

typedef union ogl_v4f_t
{
    float v4f[4];
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
} ogl_v4f_t;

typedef union ogl_mat4f_t
{
    float mat4f[4][4];
    ogl_v4f_t v4f[4];
    struct
    {
        float xx, xy, xz, xw;
        float yx, yy, yz, yw;
        float zx, zy, zz, zw;
        float wx, wy, wz, ww;
    };
} ogl_mat4f_t;
