#ifndef Vector3_h
#define Vector3_h

#include <iostream>

#define IZ_EPSILON 1e-16
#define V3AE_EPSILON 1e-12

class Vector3
{
public:
    Vector3();
    Vector3(const Vector3& v);
    Vector3(double _x, double _y, double _z);
  
    Vector3& operator=(const Vector3& a);

    const double &operator[](int n) const;
    double &operator[](int n);

	Vector3& operator+=(const Vector3& a);
    Vector3& operator-=(const Vector3& a);
    Vector3& operator*=(double s);
    Vector3 operator-() const;
    Vector3 operator+() const;
    Vector3 operator+( const Vector3 &v ) const;
    Vector3 operator-( const Vector3 &v ) const;
    Vector3 operator/( const double s ) const;
    Vector3 operator*( const double s ) const;
    // Dot
    double operator*( const Vector3 &v ) const;
    // Cross product
    Vector3 operator^( const Vector3 &v ) const;
    double length() const;
    double lengthSquared() const;
    void normalize();
    bool operator==( const Vector3 &v ) const;
    bool operator!=( const Vector3 &v ) const;
    bool approxEqual( const Vector3 &v, double eps = V3AE_EPSILON ) const;
    void print() const;

private:
    double x, y, z;
};

inline Vector3 operator*( const double s, const Vector3 &v )
{
    return Vector3( v[0] * s, v[1] * s, v[2] * s );
}

inline double dot( const Vector3 &w, const Vector3 &v )
{
    return w * v;
}

inline Vector3 cross( const Vector3 &w, const Vector3 &v )
{
    return w ^ v;
}

inline double length( const Vector3 &v )
{
	return v.length();
}

inline Vector3 unit( const Vector3 &v )
{
	const double len = v.length();
	return v / len;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    return os;
}

#endif /* Vector3_h */
