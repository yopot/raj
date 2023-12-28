#include "vec2.hpp"

vec2 &vec2::set(rajm x, rajm y)
{
 this->x = x;
 this->y = y;
 return *this;
}

rajm vec2::length() const
{
 return sqrt(x * x + y * y);
}

rajm vec2::distance(const vec2 &rh) const
{
 return sqrt((rh.x - x) * (rh.x - x) + (rh.y - y) * (rh.y - y));
}

vec2 &vec2::normalize()
{
 rajm xxyy = x * x + y * y;
 rajm invLen = 1.0 / sqrt(xxyy);
 x *= invLen;
 y *= invLen;
 return *this;
}

rajm vec2::dot(const vec2 &rh) const
{
 return x * rh.x + y * rh.y;
}

bool vec2::equal(const vec2 &rhs, rajm epsilon) const
{
 return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

vec2 vec2::operator-() const
{
 return vec2{-x, -y};
}

vec2 vec2::operator+(const vec2 &rh) const
{
 return vec2{x + rh.x, y + rh.y};
}

vec2 vec2::operator-(const vec2 &rh) const
{
 return vec2{x - rh.x, y - rh.y};
}

vec2 &vec2::operator+=(const vec2 &rh)
{
 x += rh.x;
 y += rh.y;
 return *this;
}

vec2 &vec2::operator-=(const vec2 &rh)
{
 x -= rh.x;
 y -= rh.y;
 return *this;
}

vec2 vec2::operator*(const rajm a) const
{
 return vec2{x * a, y * a};
}

vec2 vec2::operator*(const vec2 &rhs) const
{
 return vec2(x * rhs.x, y * rhs.y);
}

vec2 &vec2::operator*=(const rajm a)
{
 x *= a;
 y *= a;
 return *this;
}

vec2 &vec2::operator*=(const vec2 &rhs)
{
 x *= rhs.x;
 y *= rhs.y;
 return *this;
}

vec2 vec2::operator/(const rajm a) const
{
 return vec2{x / a, y / a};
}

vec2 &vec2::operator/=(const rajm a)
{
 x /= a;
 y /= a;
 return *this;
}

bool vec2::operator==(const vec2 &rh) const
{
 return (x == rh.x) && (y == rh.y);
}

bool vec2::operator!=(const vec2 &rh) const
{
 return (x != rh.x) || (y != rh.y);
}

bool vec2::operator<(const vec2 &rhs) const
{
 if (x < rhs.x)
  return true;
 if (x > rhs.x)
  return false;
 if (y < rhs.y)
  return true;
 if (y > rhs.y)
  return false;
 return false;
}

rajm vec2::operator[](int index) const
{
 return (&x)[index];
}

rajm &vec2::operator[](int index)
{
 return (&x)[index];
}