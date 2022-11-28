#ifndef _CONE_H_
#define _CONE_H_
#include <Fl/gl.h>
#include "vec3f.h"
#include <glm/glm.hpp>

class cone
{
  public:
    // Does the drawing.
    void    Draw(glm::vec3 & pos, glm::vec3 & color, GLdouble br, GLdouble tr, GLdouble h);
};

#endif

