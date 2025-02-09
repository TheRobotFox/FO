#ifndef VEC2F_H_
#define VEC2F_H_

#include <cmath>
struct vec2f
{
     static auto pythagoras(float a, float b) -> float{return std::sqrt((a*a)+(b*b));}
public:
    float x, y;

    auto length() const -> float
    {
        return pythagoras(x, y);
    }
    // pythagoras distance to node
    auto normalize() const -> vec2f
    {
        float len = length();
        return {.x=x/len, .y=y/len};
    }
    auto operator+=(const vec2f &other) -> vec2f&
    {
        x+=other.x;
        y+=other.y;
        return *this;
    }
    auto operator+(const vec2f &other) const -> vec2f
    {
        return {.x=x+other.x, .y=y+other.y};
    }
    auto operator-(const vec2f &other) const -> vec2f
    {
        return {.x=x-other.x, .y=y-other.y};
    }
    auto operator*(float scale) const -> vec2f
    {
        return {.x=x*scale, .y=y*scale};
    }
    auto distance(const vec2f &other) const -> float
    {
        return (*this-other).length();
    }
    static auto rand_dir() -> vec2f
    {
        float angle = (2*3.141592653589793*rand())/RAND_MAX;
        return {.x=std::cos(angle), .y=std::sin(angle)};
    }
};



#endif // VEC2F_H_
