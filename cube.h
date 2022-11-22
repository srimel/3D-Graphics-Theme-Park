#pragma once
#ifndef _CUBE_H_
#define _CUBE_H_
#include <Fl/gl.h>

class cube
{
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the grass texture.
    float x, y, z;
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
      cube(void) { display_list = 0; x = 0.f; y = 0.f; z = 0.f; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~cube(void);

    // Initializer. Creates the display list.
    bool    Initialize(float pos_x, float pos_y, float pos_z, float scale);

    // Does the drawing.
    void    Draw(void);
};

#endif

