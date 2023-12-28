#ifndef RAJ_VECTOR_2F
#define RAJ_VECTOR_2F

#ifdef RAJ_DEBUG
#include <iostream>
using namespace std;
#endif

#ifndef rajm
#define rajm float
#endif

#include <cmath>

struct vec2
{
 rajm x;
 rajm y;

 // ctors
 vec2() : x(0), y(0){};
 vec2(rajm x, rajm y) : x(x), y(y){};

 // utils functions
 vec2& set(rajm x, rajm y);
 rajm length() const;                          //
 rajm distance(const vec2 &vec) const;      // distance between two vectors
 vec2 &normalize();                          //
 rajm dot(const vec2 &vec) const;           // dot product
 bool equal(const vec2 &vec, rajm e) const; // compare with epsilon

 // operators
 vec2 operator-() const;                   // unary operator (negate)
 vec2 operator+(const vec2 &rhs) const; // add rhs
 vec2 operator-(const vec2 &rhs) const; // subtract rhs
 vec2 &operator+=(const vec2 &rhs);     // add rhs and update this object
 vec2 &operator-=(const vec2 &rhs);     // subtract rhs and update this object
 vec2 operator*(const rajm scale) const;  // scale
 vec2 operator*(const vec2 &rhs) const; // multiply each element
 vec2 &operator*=(const rajm scale);      // scale and update this object
 vec2 &operator*=(const vec2 &rhs);     // multiply each element and update this object
 vec2 operator/(const rajm scale) const;  // inverse scale
 vec2 &operator/=(const rajm scale);      // scale and update this object
 bool operator==(const vec2 &rhs) const;   // exact compare, no epsilon
 bool operator!=(const vec2 &rhs) const;   // exact compare, no epsilon
 bool operator<(const vec2 &rhs) const;    // comparison for sort
 rajm operator[](int index) const;           // subscript operator v[0], v[1]
 rajm &operator[](int index);                // subscript operator v[0], v[1]

#ifdef RAJ_DEBUG
 friend std::ostream &operator<<(std::ostream &os, const vec2 &vec);
#endif
};

// functions must be in header file


#ifdef RAJ_DEBUG
ostream &operator<<(ostream &out, const vec2 &v)
{
 out << "(" << v.x << ", " << v.y << ")";
 return out;
}
#endif

#endif