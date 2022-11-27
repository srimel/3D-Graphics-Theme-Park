#ifndef _SWEEP_H_
#define _SWEEP_H_
#include <Fl/gl.h>

class sweep
{
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the grass texture.
    bool    initialized;    // Whether or not we have been initialised.

  public:
	sweep(void) { display_list = 0; initialized = false; };
    ~sweep(void);

    // Initializer. Creates the display list.
    bool Initialize(float posx, float posy, float posz);

    // Does the drawing.
    void    Draw(void);
};

#endif

