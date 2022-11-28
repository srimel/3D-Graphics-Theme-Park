#include "tree.h"

void tree::Draw(float posx, float posy, float posz, GLdouble thickness, GLdouble height)
{
	glm::vec3 posTop(0+posx, 0+posy, 5+posz);
	glm::vec3 topColor(0, 1, 0); //green
	top.Draw(posTop, topColor, 4.0*thickness, 0.0*thickness, height);

	glm::vec3 posBottom(0+posx, 0+posy, 0+posz);
	double r = 139.0 / 255.0;
	double g = 69.0 / 255.0;
	double b = 19.0 / 255.0;
	glm::vec3 bottomColor(r,g,b); //brown
	base.Draw(posBottom, bottomColor, 1.0*thickness, 1.0*thickness, height-4);
}
