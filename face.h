#ifndef _FACE_H_
#define _FACE_H_
#include <Fl/gl.h>
#include <glm/glm.hpp>
#include <vector>

class face
{
  private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the grass texture.
    
    float x, y, z;
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    face(void) { display_list = 0; x = 0.f; y = 0.f; z = 0.f; initialized = false; };
    bool loadOBJ(const char* path,
                 std::vector<glm::vec3> & out_verts,
                 std::vector<glm::vec2> & out_uvs,
                 std::vector<glm::vec3> & out_norms);

    // Destructor. Frees the display lists and texture object.
    ~face(void);

    // Initializer. Creates the display list.
    bool Initialize(float posx, float posy, float posz, float scalex, float scaley, float scalez, float r, float g, float b);

    // Does the drawing.
    void    Draw(void);
};

#endif

