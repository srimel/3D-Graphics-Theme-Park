#include "cube.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

cube::~cube(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool cube::InitializeConcession(float pos_x, float pos_y, float pos_z, float scale)
{
	x = pos_x; y = pos_y; z = pos_z;
    ubyte   *image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data = (ubyte*)tga_load("concession2.tga", &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "cube::Initialize: Couldn't load concession.tga\n");
	return false;
    }

	// This creates a texture object and binds it, so the next few operations
	// apply to this texture.
	glGenTextures(1, &texture_obj);
	glBindTexture(GL_TEXTURE_2D, texture_obj);

	// This sets a parameter for how the texture is loaded and interpreted.
	// basically, it says that the data is packed tightly in the image array.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// This sets up the texture with high quality filtering. First it builds
	// mipmaps from the image data, then it sets the filtering parameters
	// and the wrapping parameters. We want the grass to be repeated over the
	// ground.
	gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width, image_height, 
			  GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST_MIPMAP_LINEAR);

	// This says what to do with the texture. Modulate will multiply the
	// texture by the underlying color.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

	// Now do the geometry. Create the display list.
	display_list = glGenLists(1);
	glNewList(display_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// The surface normal is up for the ground.
	//glNormal3f(0.0, 0.0, 1.0);

	// Turn on texturing and bind the grass texture.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);

	// Draw the ground as a quadrilateral, specifying texture coordinates.
	glBegin(GL_QUADS);
    /* //Old Code
	    glTexCoord2f(100.0, 100.0);
	    glVertex3f(50.0, 50.0, 0.0);
	    glTexCoord2f(-100.0, 100.0);
	    glVertex3f(-50.0, 50.0, 0.0);
	    glTexCoord2f(-100.0, -100.0);
	    glVertex3f(-50.0, -50.0, 0.0);
	    glTexCoord2f(100.0, -100.0);
	    glVertex3f(50.0, -50.0, 0.0);
	*/
	float as = 0.f;
	float at = 0.f;
	float bs = 0.f;
	float bt = 1.0;
	float cs = 1.0;
	float ct = 1.0;
	float ds = 1.0;
	float dt = 0.f;
	float offset = 0.5;

		// front)
		//glColor3f(1, 0, 0);
	glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(as+offset, at);
		glVertex3f((-1*scale)+x, (1*scale)+y, (1*scale)+z);
	    glTexCoord2f(bs+offset, bt);
		glVertex3f((-1*scale)+x, (-1*scale)+y, (1*scale)+z);
	    glTexCoord2f(cs, ct);
		glVertex3f((1*scale)+x, (-1*scale)+y, (1*scale)+z);
	    glTexCoord2f(ds, dt);
		glVertex3f((1*scale)+x, (1*scale)+y, (1*scale)+z);

		// back
		//glColor3f(0, 1, 0);
	glNormal3f(0.0, 0.0, -1.0);
	    glTexCoord2f(ds, dt); // 1,0
		glVertex3f((-1*scale)+x, (1*scale)+y, (-1*scale)+z);
		glTexCoord2f(as+offset, at); // 0,0
		glVertex3f((1*scale)+x, (1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(bs+offset, bt); // 0,1
		glVertex3f((1*scale)+x, (-1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(cs, ct); // 1,1
		glVertex3f((-1*scale)+x, (-1*scale)+y, (-1*scale)+z);

		// top
		//glColor3f(0, 0, 1);
	glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(as, at);
		glVertex3f((-1*scale)+x, (1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(bs, bt);
		glVertex3f((-1*scale)+x, (1*scale)+y, (1*scale)+z);
	    glTexCoord2f(cs-offset, ct);
		glVertex3f((1*scale)+x, (1*scale)+y, (1*scale)+z);
	    glTexCoord2f(ds-offset, dt);
		glVertex3f((1*scale)+x, (1*scale)+y, (-1*scale)+z);

		// bottom
		//glColor3f(1, 1, 0);
	glNormal3f(0.0, -1.0, 0.0);
	    glTexCoord2f(ds, dt); // 1,0
		glVertex3f((-1*scale)+x, (-1*scale)+y, (-1*scale)+z);
		glTexCoord2f(as+offset, at); // 0,0
		glVertex3f((1*scale)+x, (-1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(bs+offset, bt); // 0,1
		glVertex3f((1*scale)+x, (-1*scale)+y, (1*scale)+z);
	    glTexCoord2f(cs, ct); // 1,1
		glVertex3f((-1*scale)+x, (-1*scale)+y, (1*scale)+z);

		// left
		//glColor3f(0, 1, 1);
	glNormal3f(-1.0, 0.0, 0.0);
	    glTexCoord2f(ds, dt); // 1,0
		glVertex3f((-1*scale)+x, (1*scale)+y, (-1*scale)+z);
		glTexCoord2f(as+offset, at); // 0,0
		glVertex3f((-1*scale)+x, (-1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(bs+offset, bt); // 0,1
		glVertex3f((-1*scale)+x, (-1*scale)+y, (1*scale)+z);
	    glTexCoord2f(cs, ct); // 1,1
		glVertex3f((-1*scale)+x, (1*scale)+y, (1*scale)+z);

		// right
		//glColor3f(1, 0, 1);
	glNormal3f(1.0, 0.0, 0.0);
	    glTexCoord2f(ds, dt); // 1,0
		glVertex3f((1*scale)+x, (1*scale)+y, (1*scale)+z);
		glTexCoord2f(as+offset, at); // 0,0
		glVertex3f((1*scale)+x, (-1*scale)+y, (1*scale)+z);
	    glTexCoord2f(bs+offset, bt); // 0,1
		glVertex3f((1*scale)+x, (-1*scale)+y, (-1*scale)+z);
	    glTexCoord2f(cs, ct); // 1,1
		glVertex3f((1*scale)+x, (1*scale)+y, (-1*scale)+z);
	glEnd();

	// Turn texturing off again, because we don't want everything else to
	// be textured.
	glDisable(GL_TEXTURE_2D);
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void cube::Draw(void)
{
	glPushMatrix();
	glCallList(display_list);
	glPopMatrix();
}
