//////////////////////////////////////////////////////////////////////////////
//
//  --- vec.h ---
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//  vec2.h - 2D vector
//
#pragma once
#include <iostream>
#include <cmath>
#include "glad.h"
#include <math.h>

#define M_PI 3.14159265358979323846264338327
struct vec2
{

    GLfloat x;
    GLfloat y;

    //
    //  --- Constructors and Destructors ---
    //

    vec2(GLfloat s = GLfloat(0.0)) : x(s), y(s) {}

    vec2(GLfloat x, GLfloat y) : x(x), y(y) {}

    vec2(const vec2 &v)
    {
        x = v.x;
        y = v.y;
    }

    //
    //  --- Indexing Operator ---
    //

    GLfloat &operator[](int i) { return *(&x + i); }
    const GLfloat operator[](int i) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec2 operator-() const // unary minus operator
    {
        return vec2(-x, -y);
    }

    vec2 operator+(const vec2 &v) const
    {
        return vec2(x + v.x, y + v.y);
    } /*there was a BUG here*/

    vec2 operator-(const vec2 &v) const
    {
        return vec2(x - v.x, y - v.y);
    }

    vec2 operator*(const GLfloat s) const
    {
        return vec2(s * x, s * y);
    }

    vec2 operator*(const vec2 &v) const
    {
        return vec2(x * v.x, y * v.y);
    }

    friend vec2 operator*(const GLfloat s, const vec2 &v)
    {
        return v * s;
    }

    vec2 operator/(const GLfloat s) const
    {

        GLfloat r = GLfloat(1.0) / s;
        return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec2 &operator+=(const vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    } /*there was a BUG here*/

    vec2 &operator-=(const vec2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2 &operator*=(const GLfloat s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    vec2 &operator*=(const vec2 &v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    vec2 &operator/=(const GLfloat s)
    {

        GLfloat r = GLfloat(1.0) / s;
        *this *= r;

        return *this;
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream &operator<<(std::ostream &os, const vec2 &v)
    {
        return os << "( " << v.x << ", " << v.y << " )";
    }

    friend std::istream &operator>>(std::istream &is, vec2 &v)
    {
        return is >> v.x >> v.y;
    }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat *() const
    {
        return static_cast<const GLfloat *>(&x);
    }

    operator GLfloat *()
    {
        return static_cast<GLfloat *>(&x);
    }
};

//----------------------------------------------------------------------------
//
//  Non-class vec2 Methods
//

inline GLfloat dot(const vec2 &u, const vec2 &v)
{
    return u.x * v.x + u.y * v.y; /*there was a BUG here*/
}

inline GLfloat length(const vec2 &v)
{
    return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2)); /*there was a BUG here*/
}

inline vec2 normalize(const vec2 &v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y);

    // Avoid division by zero
    if (magnitude > 0.0f)
    {
        float invMagnitude = 1.0f / magnitude;
        return vec2(v.x * invMagnitude, v.y * invMagnitude);
    }
    else
    {
        // Handle the case where the vector is already close to zero or is zero
        return vec2(0.0f, 0.0f);
    }
}

inline GLfloat cross(const vec2 &u, const vec2 &v)
{
    return u.x * v.y - u.y * v.x;
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////

struct vec3
{

    GLfloat x;
    GLfloat y;
    GLfloat z;

    //
    //  --- Constructors and Destructors ---
    //

    vec3(GLfloat s = GLfloat(0.0)) : x(s), y(s), z(s) {}

    vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

    vec3(const vec3 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    vec3(const vec2 &v, const float f)
    {
        x = v.x;
        y = v.y;
        z = f;
    }

    //
    //  --- Indexing Operator ---
    //

    GLfloat &operator[](int i) { return *(&x + i); }
    const GLfloat operator[](int i) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec3 operator-() const // unary minus operator
    {
        return vec3(-x, -y, -z);
    }

    vec3 operator+(const vec3 &v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }

    vec3 operator-(const vec3 &v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }

    vec3 operator*(const GLfloat s) const
    {
        return vec3(s * x, s * y, s * z);
    }

    vec3 operator*(const vec3 &v) const
    {
        return vec3(x * v.x, y * v.y, z * v.z);
    }

    friend vec3 operator*(const GLfloat s, const vec3 &v)
    {
        return v * s;
    }

    vec3 operator/(const GLfloat s) const
    {

        GLfloat r = GLfloat(1.0) / s;
        return *this * r;
    }

    vec3 clamp(float min_val, float max_val)
    {
        int test = std::max(3,4);
        return vec3(std::max(min_val, std::min(x, max_val)),
                    std::max(min_val, std::min(y, max_val)),
                    std::max(min_val, std::min(z, max_val)));
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec3 &operator+=(const vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3 &operator-=(const vec3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vec3 &operator*=(const GLfloat s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    } /*there was a BUG here*/

    vec3 &operator*=(const vec3 &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    vec3 &operator/=(const GLfloat s)
    {

        GLfloat r = GLfloat(1.0) / s;
        *this *= r;

        return *this;
    }

    inline vec3 toVec3(const vec2 &v)
    {
        return vec3(v.x, v.y, 0);
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
    {
        return os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
    }

    friend std::istream &operator>>(std::istream &is, vec3 &v)
    {
        return is >> v.x >> v.y >> v.z;
    }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat *() const
    {
        return static_cast<const GLfloat *>(&x);
    }

    operator GLfloat *()
    {
        return static_cast<GLfloat *>(&x);
    }
};

//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//

inline GLfloat dot(const vec3 &u, const vec3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline GLfloat length(const vec3 &v)
{
    return std::sqrt(dot(v, v));
}

inline vec3 normalize(const vec3 &v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    // Avoid division by zero
    if (magnitude > 0.0f)
    {
        float invMagnitude = 1.0f / magnitude;
        return vec3(v.x * invMagnitude, v.y * invMagnitude, v.z * invMagnitude);
    }
    else
    {
        // Handle the case where the vector is already close to zero or is zero
        return vec3(0.0f, 0.0f, 0.0f);
    }
}

inline vec3 cross(const vec3 &a, const vec3 &b)
{
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    // here there was a bug
}

inline vec3 calculateNormal(vec3 first_point, vec3 second_point, vec3 third_point)
{
    vec3 a = third_point - first_point;
    vec3 b = second_point - first_point;

    vec3 c = cross(a, b);

    return -normalize(c);
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////

struct vec4
{

    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;

    //
    //  --- Constructors and Destructors ---
    //

    vec4(GLfloat s = GLfloat(0.0)) : x(s), y(s), z(s), w(s) {}

    vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x(x), y(y), z(z), w(w) {}

    vec4(const vec4 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    vec4(const vec3 &v, const float w = 1.0) : w(w)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    vec4(const vec2 &v, const float z, const float w) : z(z), w(w)
    {
        x = v.x;
        y = v.y;
    }

    //
    //  --- Indexing Operator ---
    //

    GLfloat &operator[](int i) { return *(&x + i); }
    const GLfloat operator[](int i) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec4 operator-() const // unary minus operator
    {
        return vec4(-x, -y, -z, -w);
    }

    vec4 operator+(const vec4 &v) const
    {
        return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    vec4 operator-(const vec4 &v) const
    {
        return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    vec4 operator*(const GLfloat s) const
    {
        return vec4(s * x, s * y, s * z, s * w);
    }

    vec4 operator*(const vec4 &v) const
    {
        return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    friend vec4 operator*(const GLfloat s, const vec4 &v)
    {
        return v * s;
    }

    vec4 operator/(const GLfloat s) const
    {

        GLfloat r = GLfloat(1.0) / s;
        return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec4 &operator+=(const vec4 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    vec4 &operator-=(const vec4 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    vec4 &operator*=(const GLfloat s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    vec4 &operator*=(const vec4 &v)
    {
        x *= v.x, y *= v.y, z *= v.z, w *= v.w;
        return *this;
    }

    vec4 &operator/=(const GLfloat s)
    {

        GLfloat r = GLfloat(1.0) / s;
        *this *= r;

        return *this;
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream &operator<<(std::ostream &os, const vec4 &v)
    {
        return os << "( " << v.x << ", " << v.y
                  << ", " << v.z << ", " << v.w << " )";
    }

    friend std::istream &operator>>(std::istream &is, vec4 &v)
    {
        return is >> v.x >> v.y >> v.z >> v.w;
    }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat *() const
    {
        return static_cast<const GLfloat *>(&x);
    }

    operator GLfloat *()
    {
        return static_cast<GLfloat *>(&x);
    }
};

//----------------------------------------------------------------------------
//
//  Non-class vec4 Methods
//

inline GLfloat dot(const vec4 &u, const vec4 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

inline GLfloat length(const vec4 &v)
{
    return std::sqrt(dot(v, v));
}

inline vec4 normalize(const vec4 &v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

    // Avoid division by zero
    if (magnitude > 0.0f)
    {
        float invMagnitude = 1.0f / magnitude;
        return vec4(v.x * invMagnitude, v.y * invMagnitude, v.z * invMagnitude, v.w * invMagnitude);
    }
    else
    {
        // Handle the case where the vector is already close to zero or is zero
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

inline vec3 cross(const vec4 &a, const vec4 &b)
{
    return vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

inline vec3 toVec3(const vec4 &v)
{
    return vec3(v.x, v.y, v.z);
}

inline vec3 toEuclidian(const vec4 &v)
{
    return vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

inline vec3 reflect(const vec3 &vector, const vec3 &normal)
{
    return vector - normal * 2 * dot(vector, normal);
}
//----------------------------------------------------------------------------