#ifndef _CONE_H_
#define _CONE_H_
#include <Fl/gl.h>
#include "vec3f.h"

class cone
{
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLubyte display_list_top;   // The display list that does all the work.
    GLubyte display_list_bottom;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the grass texture.
    bool    initialized;    // Whether or not we have been initialised.

    vec3f crossProduct(const vec3f& v1, const vec3f& v2);
    vec3f perpendicular(const vec3f& v);

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    cone(void) { display_list = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~cone(void);

    // Initializer. Creates the display list.
    bool Initialize(const vec3f& d, const vec3f& a, const float h, const float rd, const int n);

    // Does the drawing.
    void    Draw(void);
};

#endif

