#pragma once

#include "core_types.h"

#include <math.h>
/*
 * NOTE: source coordinates are right handed y-up, +z out, +x right
 *       dest coordinates are right handed y-down with z(depth) clip from 0.0(near) - 1.0(far)
 */

// ogl_vec2f_t defs

// ogl_vec3f_t defs
float ogl_vec3f_sqr_magnitude(const ogl_vec3f_t* v);
float ogl_vec3f_magnitude(const ogl_vec3f_t* v);
void ogl_vec3f_normalize(ogl_vec3f_t* v);
ogl_vec3f_t ogl_vec3f_normalized(const ogl_vec3f_t* v);
float ogl_vec3f_dot(const ogl_vec3f_t* a, const ogl_vec3f_t* b);
ogl_vec3f_t ogl_vec3f_cross(const ogl_vec3f_t* a, const ogl_vec3f_t* b);
ogl_vec3f_t ogl_vec3f_minus(const ogl_vec3f_t* a, const ogl_vec3f_t* b);
ogl_vec3f_t ogl_vec3f_multiply(const ogl_vec3f_t* a, float b);

// ogl_vec4f_t defs

// ogl_mat4f_t defs
ogl_mat4f_t ogl_mat4f_transpose(const ogl_mat4f_t* m);
ogl_mat4f_t ogl_mat4f_look_at(const ogl_vec3f_t* eye, const ogl_vec3f_t* at, const ogl_vec3f_t* up);
ogl_mat4f_t ogl_mat4f_perspective(
    float vertical_fov,
    float aspect_ratio,
    float z_near,
    float z_far);
ogl_mat4f_t ogl_mat4f_from_rotation_z(float angle);
ogl_mat4f_t ogl_mat4f_rotate(const ogl_mat4f_t* m, float angle, const ogl_vec3f_t* v);
ogl_mat4f_t ogl_mat4f_identity();

// uint32_t defs
static inline uint32_t ogl_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}
static inline uint32_t ogl_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

// ogl_vec2f_t impl

// ogl_vec3f_t impl
float ogl_vec3f_sqr_magnitude(const ogl_vec3f_t* v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

float ogl_vec3f_magnitude(const ogl_vec3f_t* v)
{
    float sqr_mag = ogl_vec3f_sqr_magnitude(v);
    return sqrtf(sqr_mag);
}

void ogl_vec3f_normalize(ogl_vec3f_t* v)
{
    float inv_mag = 1.0f / ogl_vec3f_magnitude(v);
    v->x *= inv_mag;
    v->y *= inv_mag;
    v->z *= inv_mag;
}

ogl_vec3f_t ogl_vec3f_normalized(const ogl_vec3f_t* v)
{
    ogl_vec3f_t r = {
        .x = v->x,
        .y = v->y,
        .z = v->z,
    };
    ogl_vec3f_normalize(&r);
    return r;
}

float ogl_vec3f_dot(const ogl_vec3f_t* a, const ogl_vec3f_t* b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

ogl_vec3f_t ogl_vec3f_cross(const ogl_vec3f_t* a, const ogl_vec3f_t* b)
{
    // clang-format off
    return (ogl_vec3f_t) {
        .x = (a->y * b->z) - (a->z * b->y),
        .y = (a->z * b->x) - (a->x * b->z),
        .z = (a->x * b->y) - (a->y * b->x),
    };
    // clang-format on
}

ogl_vec3f_t ogl_vec3f_minus(const ogl_vec3f_t* a, const ogl_vec3f_t* b)
{
    return (ogl_vec3f_t) { .x = (a->x - b->x), .y = (a->y - b->y), .z = (a->z - b->z) };
}

ogl_vec3f_t ogl_vec3f_multiply(const ogl_vec3f_t* a, float b)
{
    return (ogl_vec3f_t) { .x = (a->x * b), .y = (a->y * b), .z = (a->z * b) };
}

// ogl_vec4f_t impl
ogl_vec4f_t ogl_vec4f_add(const ogl_vec4f_t* a, const ogl_vec4f_t* b)
{
    return (ogl_vec4f_t) { .x = (a->x + b->x), .y = (a->y + b->y), .z = (a->z + b->z), .w = (a->w + b->w) };
}

float ogl_vec4f_dot(const ogl_vec4f_t* a, const ogl_vec4f_t* b)
{
    return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w));
}

ogl_vec4f_t ogl_vec4f_multiply(const ogl_vec4f_t* a, float b)
{
    return (ogl_vec4f_t) { .x = (a->x * b), .y = (a->y * b), .z = (a->z * b), .w = (a->w * b) };
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

ogl_mat4f_t ogl_mat4f_transpose(const ogl_mat4f_t* m)
{
    return (ogl_mat4f_t) { .mat4f = {
                               { m->xx, m->yx, m->zx, m->wx },
                               { m->xy, m->yy, m->zy, m->wy },
                               { m->xz, m->yz, m->zz, m->wz },
                               { m->xw, m->yw, m->zw, m->ww },
                           } };
}
#endif

ogl_mat4f_t ogl_mat4f_look_at(const ogl_vec3f_t* eye, const ogl_vec3f_t* at, const ogl_vec3f_t* up)
{
    ogl_vec3f_t f = ogl_vec3f_minus(at, eye);
    ogl_vec3f_normalize(&f);
    ogl_vec3f_t r = ogl_vec3f_cross(&f, up);
    ogl_vec3f_normalize(&r);
    ogl_vec3f_t u = ogl_vec3f_cross(&r, &f);

    ogl_vec3f_t w = { -ogl_vec3f_dot(&r, eye), -ogl_vec3f_dot(&u, eye), ogl_vec3f_dot(&f, eye) };

    // clang-format off
    return (ogl_mat4f_t) {
        .vec4f = {
            (ogl_vec4f_t) {r.x, u.x, -f.x, 0.0f},
            (ogl_vec4f_t) {r.y, u.y, -f.y, 0.0f},
            (ogl_vec4f_t) {r.z, u.z, -f.z, 0.0f},
            (ogl_vec4f_t) {w.x, w.y, w.z, 1.0f},
        }
    };
    // clang-format on
}

ogl_mat4f_t ogl_mat4f_perspective(float vertical_fov, float aspect_ratio, float z_near, float z_far)
{
    float inv_length = 1.0f / (z_near - z_far);
    float f = 1.0f / tanf(0.5f * vertical_fov);
    float a = f / aspect_ratio;
    float b = (z_near + z_far) * inv_length;
    float c = 2.0f * z_near * z_far * inv_length;

    // clang-format off
    return (ogl_mat4f_t) {
        .vec4f = {
            (ogl_vec4f_t) {a,    0.0f, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, f,    0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, b,    -1.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, c,    0.0f},
        }
    };
    // clang-format on
#if 0
    float t = tanf(vertical_fov / 2.0f);
    float sy = 1.0f / t;
    float sx = sy / aspect_ratio;
    float nmf = z_near - z_far;

    // clang-format off
    return (ogl_mat4f_t) {
        .vec4f = {
            (ogl_vec4f_t) {sx, 0.0f, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, sy, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, (z_far + z_near) / nmf, -1.0},
            (ogl_vec4f_t) {0.0f, 0.0f, 2.0f * z_near * z_far / nmf, 0.0f},
        }
    };
    // clang-format on
#endif
}

ogl_mat4f_t ogl_mat4f_from_rotation_x(float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    // clang-format off
    return (ogl_mat4f_t) {
        .vec4f = {
            (ogl_vec4f_t) {1.0f, 0.0f, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, c,    s,    0.0f},
            (ogl_vec4f_t) {0.0f, -s,   c,    0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, 0.0f, 1.0f},
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
        .vec4f = {
            (ogl_vec4f_t) {c,    s,    0.0f, 0.0f},
            (ogl_vec4f_t) {-s,   c,    0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, 1.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    // clang-format on
}

ogl_mat4f_t ogl_mat4f_translate(const ogl_mat4f_t* m, const ogl_vec3f_t* v)
{
    ogl_mat4f_t result = { 0 };
    memcpy(result.vec4f, m->vec4f, 16 * sizeof(float));
    ogl_vec4f_t t1 = ogl_vec4f_multiply(&m->vec4f[0], v->x);
    ogl_vec4f_t t2 = ogl_vec4f_multiply(&m->vec4f[1], v->y);
    ogl_vec4f_t t3 = ogl_vec4f_multiply(&m->vec4f[2], v->z);
    ogl_vec4f_t r1 = ogl_vec4f_add(&t1, &t2);
    r1 = ogl_vec4f_add(&r1, &t3);
    result.vec4f[3] = ogl_vec4f_add(&m->vec4f[3], &r1);
    return result;
}

#if 0
ogl_mat4f_t ogl_mat4f_rotate(const ogl_mat4f_t* m, float angle, const ogl_vec3f_t* v)
{
    float a = angle;
    float c = cosf(a);
    float s = sinf(a);

    ogl_vec3f_t axis = ogl_vec3f_normalized(v);
    ogl_vec3f_t temp = ogl_vec3f_multiply(&axis, (1.0f - angle));

    ogl_mat4f_t rotate = ogl_mat4f_identity();
    rotate.mat4f[0][0] = c + temp.x * axis.x;
    rotate.mat4f[0][1] = temp.x * axis.y + s * axis.z;
    rotate.mat4f[0][2] = temp.x * axis.z - s * axis.y;

    rotate.mat4f[1][0] = temp.y * axis.x - s * axis.z;
    rotate.mat4f[1][1] = c + temp.y * axis.y;
    rotate.mat4f[1][2] = temp.y * axis.z + s * axis.x;

    rotate.mat4f[2][0] = temp.z * axis.x + s * axis.y;
    rotate.mat4f[2][1] = temp.z * axis.y - s * axis.x;
    rotate.mat4f[2][2] = c + temp.z * axis.z;

    ogl_mat4f_t result = ogl_mat4f_identity();
    ogl_vec4f_t t1 = ogl_vec4f_multiply(&m->vec4f[0], rotate.mat4f[0][0]);
    ogl_vec4f_t t2 = ogl_vec4f_multiply(&m->vec4f[1], rotate.mat4f[0][1]);
    ogl_vec4f_t t3 = ogl_vec4f_multiply(&m->vec4f[2], rotate.mat4f[0][2]);
    ogl_vec4f_t r1 = ogl_vec4f_add(&t1, &t2);
    result.vec4f[0] = ogl_vec4f_add(&r1, &t3);

    t1 = ogl_vec4f_multiply(&m->vec4f[0], rotate.mat4f[1][0]);
    t2 = ogl_vec4f_multiply(&m->vec4f[1], rotate.mat4f[1][1]);
    t3 = ogl_vec4f_multiply(&m->vec4f[2], rotate.mat4f[1][2]);
    r1 = ogl_vec4f_add(&t1, &t2);
    result.vec4f[1] = ogl_vec4f_add(&r1, &t3);

    t1 = ogl_vec4f_multiply(&m->vec4f[0], rotate.mat4f[2][0]);
    t2 = ogl_vec4f_multiply(&m->vec4f[1], rotate.mat4f[2][1]);
    t3 = ogl_vec4f_multiply(&m->vec4f[2], rotate.mat4f[2][2]);
    r1 = ogl_vec4f_add(&t1, &t2);
    result.vec4f[2] = ogl_vec4f_add(&r1, &t3);

    result.vec4f[3] = m->vec4f[3];

    return result;
}
#endif
ogl_mat4f_t ogl_mat4f_identity()
{
    // clang-format off
    return (ogl_mat4f_t) {
        .vec4f = {
            (ogl_vec4f_t) {1.0f, 0.0f, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 1.0f, 0.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, 1.0f, 0.0f},
            (ogl_vec4f_t) {0.0f, 0.0f, 0.0f, 1.0f},
        }
    };
    // clang-format on
}
