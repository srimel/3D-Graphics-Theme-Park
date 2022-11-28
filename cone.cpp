#include "cone.h"
#include <GL/glu.h>

void cone::Draw(glm::vec3 & pos, glm::vec3 & color, GLdouble br, GLdouble tr, GLdouble h)
{
	glPushMatrix();
		glColor3d(color.r, color.g, color.b);
		GLUquadricObj* quadratic;
		quadratic = gluNewQuadric();
		glTranslatef(pos.x, pos.y, pos.z);
		gluCylinder(quadratic, br, tr, h, 32, 32);
	glPopMatrix();
}
