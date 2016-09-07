#include "Vector3.h"

#include <math.h>
#include <assert.h>

inline bool isZero(double in_d, double in_dEps = IZ_EPSILON)
{ 
    return (in_d < in_dEps && in_d > -in_dEps) ? true : false; 
}

Vector3::Vector3() : x(0), y(0), z(0)
{
}

Vector3::Vector3(const Vector3& v) : x(v[0]), y(v[1]), z(v[2])
{
}

Vector3::Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{
}

Vector3& Vector3::operator=(const Vector3& a)
{
    x = a[0]; y = a[1]; z = a[2];
    return *this;
}

const double& Vector3::operator[](int n) const
{
	return (&x)[n];
}

double& Vector3::operator[](int n)
{
	return (&x)[n];
}

Vector3& Vector3::operator+=(const Vector3& a)
{
    x += a[0]; y += a[1]; z += a[2];
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& a)
{
    x -= a[0]; y -= a[1]; z -= a[2];
    return *this;
}

Vector3& Vector3::operator*=(double s)
{
    x *= s; y *= s; z *= s;
    return *this;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+() const
{
    return *this;
}

Vector3 Vector3::operator+( const Vector3 &v ) const
{
    return Vector3( x + v.x, y + v.y, z + v.z );
}

Vector3 Vector3::operator-( const Vector3 &v ) const
{
    return Vector3( x - v.x, y - v.y, z - v.z );
}

Vector3 Vector3::operator/( const double s ) const
{
    assert( s > 0.0 );
    return Vector3( x / s, y / s, z / s );
}

Vector3 Vector3::operator*( const double s ) const
{
    return Vector3( x * s, y * s, z * s );
}

// Dot Product
double Vector3::operator*( const Vector3 &v ) const
{
    return x * v.x + y * v.y + z * v.z;
}

// Cross Product
Vector3 Vector3::operator^( const Vector3 &v ) const
{
    return Vector3( y * v.z - z * v.y,
                    z * v.x - x * v.z,
                    x * v.y - y * v.x );
}

double Vector3::length() const
{
    return (double) sqrt(x * x + y * y + z * z);
}

double Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

void Vector3::normalize()
{
    double s = 1.0 / (double) sqrt(x * x + y * y + z * z);
    x *= s; y *= s; z *= s;
}

bool Vector3::operator==( const Vector3 &v ) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=( const Vector3 &v ) const
{
    return x != v.x || y != v.y || z != v.z;
}

bool Vector3::approxEqual( const Vector3 &v, double eps) const
{
    return isZero( x - v.x, eps ) && isZero( y - v.y, eps ) && isZero( z - v.z, eps );
}

void Vector3::print() const
{
    std::cout << x << " " << y << " " << z << "\n";
}
