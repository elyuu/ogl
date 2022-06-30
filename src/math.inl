#pragma once

#include "core_types.h"

#include <math.h>
/*
 * NOTE: source coordinates are right handed y-up, +z out, +x right
 *       dest coordinates are right handed y-doogl with z(depth) clip from 0.0(near) - 1.0(far)
 */

// ogl_v2f_t defs

// ogl_v3f_t defs
float ogl_v3f_sqr_magnitude(const ogl_v3f_t* v);
float ogl_v3f_magnitude(const ogl_v3f_t* v);
void ogl_v3f_normalize(ogl_v3f_t* v);
ogl_v3f_t ogl_v3f_normalized(const ogl_v3f_t* v);
float ogl_v3f_dot(const ogl_v3f_t* a, const ogl_v3f_t* b);
ogl_v3f_t ogl_v3f_cross(const ogl_v3f_t* a, const ogl_v3f_t* b);
ogl_v3f_t ogl_v3f_minus(const ogl_v3f_t* a, const ogl_v3f_t* b);

// ogl_v4f_t defs

// ogl_mat4f_t defs
ogl_mat4f_t ogl_mat4f_transpose(const ogl_mat4f_t* m);
ogl_mat4f_t ogl_mat4f_look_at(const ogl_v3f_t* eye, const ogl_v3f_t* at, const ogl_v3f_t* up);
ogl_mat4f_t ogl_mat4f_perspective(float vertical_fov, float aspect_ratio, float z_near, float z_far);
ogl_mat4f_t ogl_mat4f_from_rotation_z(float angle);
ogl_mat4f_t ogl_mat4f_indentity();

// uint32_t defs
static inline uint32_t ogl_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}
static inline uint32_t ogl_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

// ogl_v2f_t impl

// ogl_v3f_t impl
float ogl_v3f_sqr_magnitude(const ogl_v3f_t* v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

float ogl_v3f_magnitude(const ogl_v3f_t* v)
{
    float sqr_mag = ogl_v3f_sqr_magnitude(v);
    return sqrtf(sqr_mag);
}

void ogl_v3f_normalize(ogl_v3f_t* v)
{
    float inv_mag = 1.0f / ogl_v3f_magnitude(v);
    v->x *= inv_mag;
    v->y *= inv_mag;
    v->z *= inv_mag;
}

ogl_v3f_t ogl_v3f_normalized(const ogl_v3f_t* v)
{
    ogl_v3f_t r = {
        .x = v->x,
        .y = v->y,
        .z = v->z,
    };
    ogl_v3f_normalize(&r);
    return r;
}

float ogl_v3f_dot(const ogl_v3f_t* a, const ogl_v3f_t* b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

ogl_v3f_t ogl_v3f_cross(const ogl_v3f_t* a, const ogl_v3f_t* b)
{
    // clang-format off
    return (ogl_v3f_t) {
        .x = (a->y * b->z) - (a->z * b->y),
        .y = (a->z * b->x) - (a->x * b->z),
        .z = (a->x * b->y) - (a->y * b->x),
    };
    // clang-format on
}

ogl_v3f_t ogl_v3f_minus(const ogl_v3f_t* a, const ogl_v3f_t* b)
{
    return (ogl_v3f_t) { .x = (a->x - b->x), .y = (a->y - b->y), .z = (a->z - b->z) };
}

// ogl_mat4f_t impl

// FIXME: This file shouldn't have anything to do with stdio
#if 0
void ogl_mat4f_print(const ogl_mat4f_t* m)
{
    // clang-format off
    printf(
        "%f, %f, %f, %f,\n"
        "%f, %f, %f, %f,\n"
        "%f, %f, %f, %f,\n"
        "%f, %f, %f, %f,\n\n",
        m->xx, m->xy, m->xz, m->xw,
        m->yx, m->yy, m->yz, m->yw,
        m->zx, m->zy, m->zz, m->zw,
        m->wx, m->wy, m->wz, m->ww);
    // clang-format on
}
#endif

ogl_mat4f_t ogl_mat4f_transpose(const ogl_mat4f_t* m)
{
    return (ogl_mat4f_t) { .mat4f = {
                              { m->xx, m->yx, m->zx, m->wx },
                              { m->xy, m->yy, m->zy, m->wy },
                              { m->xz, m->yz, m->zz, m->wz },
                              { m->xw, m->yw, m->zw, m->ww },
                          } };
}

ogl_mat4f_t ogl_mat4f_look_at(const ogl_v3f_t* eye, const ogl_v3f_t* at, const ogl_v3f_t* up)
{
    ogl_v3f_t f = ogl_v3f_minus(at, eye);
    ogl_v3f_normalize(&f);
    ogl_v3f_t r = ogl_v3f_cross(&f, up);
    ogl_v3f_normalize(&r);
    ogl_v3f_t u = ogl_v3f_cross(&r, &f);

    ogl_v3f_t w = { -ogl_v3f_dot(&r, eye), -ogl_v3f_dot(&u, eye), ogl_v3f_dot(&f, eye) };

    // clang-format off
    return (ogl_mat4f_t) {
        .v4f = {
            (ogl_v4f_t) {r.x, u.x, -f.x, 0.0f},
            (ogl_v4f_t) {r.y, u.y, -f.y, 0.0f},
            (ogl_v4f_t) {r.z, u.z, -f.z, 0.0f},
            (ogl_v4f_t) {w.x, w.y, w.z, 1.0f},
        }
    };
    // clang-format on
}

ogl_mat4f_t ogl_mat4f_perspective(float vertical_fov, float aspect_ratio, float z_near, float z_far)
{
    float t = tanf(vertical_fov / 2.0f);
    float sy = 1.0f / t;
    float sx = sy / aspect_ratio;
    float nmf = z_near - z_far;

    // clang-format off
    return (ogl_mat4f_t) {
        .v4f = {
            (ogl_v4f_t) {sx, 0.0f, 0.0f, 0.0f},
            (ogl_v4f_t) {0.0f, -sy, 0.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 0.0f, z_far / nmf, -1.0},
            (ogl_v4f_t) {0.0f, 0.0f, z_near * z_far / nmf, 0.0f},
        }
    };
    // clang-format on
}

ogl_mat4f_t ogl_mat4f_from_rotation_z(float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    // clang-format off
    return (ogl_mat4f_t) {
        .v4f = {
            (ogl_v4f_t) {c, s, 0.0f, 0.0f},
            (ogl_v4f_t) {-s, c, 0.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 0.0f, 1.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    // clang-format on
}

ogl_mat4f_t ogl_mat4f_indentity()
{
    // clang-format off
    return (ogl_mat4f_t) {
        .v4f = {
            (ogl_v4f_t) {1.0f, 0.0f, 0.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 1.0f, 0.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 0.0f, 1.0f, 0.0f},
            (ogl_v4f_t) {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    // clang-format on
}
