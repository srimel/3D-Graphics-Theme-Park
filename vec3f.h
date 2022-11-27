#ifndef _VEC3F_H_
#define _VEC3F_H_

class vec3f
{
	public:
		vec3f(float x, float y, float z) { x = 0.0; y = 0.0; z = 0.0; };
		vec3f() { x = 0.0; y = 0.0; z = 0.0; };
		~vec3f();
		vec3f add(const vec3f& toadd) const;
		vec3f mult(const float scalar) const;
		float x;
		float y;
		float z;
};
#endif
