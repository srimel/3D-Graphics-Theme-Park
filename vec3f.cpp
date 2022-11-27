#include "vec3f.h"

vec3f::~vec3f()
{
	x = 0;
	y = 0;
	z = 0;
}

vec3f vec3f::add(const vec3f& toadd) const
{
	return vec3f(x+toadd.x, y+toadd.y, z+toadd.z);
}

vec3f vec3f::mult(const float scalar) const
{
	return vec3f(x*scalar, y*scalar, z*scalar);
}
