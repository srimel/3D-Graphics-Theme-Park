#pragma once
#ifndef _TREE_H_
#define _TREE_H_
#include <Fl/gl.h>
#include "cone.h"
#include <glm/glm.hpp>

class tree
{
public:
	void tree::Draw(float posx, float posy, float posz, GLdouble thickness, GLdouble height);

private:
	cone top;
	cone base;
};
#endif

