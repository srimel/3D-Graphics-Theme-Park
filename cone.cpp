#include "cone.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>
#include <math.h>
#include <vector>
#define M_PI_DIV180 0.01745329251994329576


vec3f cone::crossProduct(const vec3f& v1, const vec3f& v2)
{
	float new_x = v1.y * v2.z - v1.z * v2.y;
	float new_y = -(v1.x * v2.z - v1.z * v2.x);
	float new_z = v1.x * v2.y - v1.y * v2.x;
	return vec3f(new_x, new_y, new_z);
}

vec3f cone::perpendicular(const vec3f& v)
{
	float min = fabsf(v.x);
	vec3f cardinalAxis(1, 0, 0);

	if (fabsf(v.y) < min)
	{
		min = fabsf(v.y);
		cardinalAxis = vec3f(0, 1, 0);
	}

	if (fabsf(v.z) < min)
	{
		min = fabsf(v.z);
		cardinalAxis = vec3f(0, 0, 1);
	}

	return crossProduct(v, cardinalAxis);
}

cone::~cone(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool cone::Initialize(const vec3f& d, const vec3f& a, const float h, const float rd, const int n)
{
    ubyte   *image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data = (ubyte*)tga_load("brick2.tga", &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Ground::Initialize: Couldn't load grass.tga\n");
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

	vec3f negD = d.mult(-1.0);
	vec3f negDH = negD.mult(h);

	vec3f c = a.add(negDH);
	vec3f e0 = perpendicular(d);
	vec3f e1 = crossProduct(e0, d);
	float angInc = 360.0 / n * M_PI_DIV180;

	//calc points around directrix
	std::vector<vec3f> pts;
	for (int i = 0; i < n; i++)
	{
		float rad = angInc * i;
		float cosRad = cos(rad);
		float sinRad = sin(rad);
		vec3f a = e0.mult(cosRad);
		vec3f b = e1.mult(sinRad);
		vec3f ab = a.add(b);
		vec3f abrd = ab.mult(rd);

		vec3f p = c.add(abrd);
		pts.push_back(p);
	}

    // Now do the geometry. Create the display list.
    display_list_top = glGenLists(1);
    glNewList(display_list_top, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(0.0, 1.0, 0.0);
	// The surface normal is up for the ground.
	glNormal3f(0.0, 0.0, 1.0);
	// Turn on texturing and bind the grass texture.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(a.x, a.y, a.z);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(pts[i].x, pts[i].y, pts[i].z);
	}
	glEnd();
	// Turn texturing off again, because we don't want everything else to
	// be textured.
	glDisable(GL_TEXTURE_2D);
    glEndList();

	/*
    display_list_bottom = glGenLists(1);
    glNewList(display_list_bottom, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	// The surface normal is up for the ground.
	glNormal3f(0.0, 0.0, 1.0);
	// Turn on texturing and bind the grass texture.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(c.x, c.y, c.z);
	for (int i = n-1; i >= 0; --i)
	{
		glVertex3f(pts[i].x, pts[i].y, pts[i].z);
	}
	glEnd();
	// Turn texturing off again, because we don't want everything else to
	// be textured.
	//glDisable(GL_TEXTURE_2D);
    glEndList();
	*/

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

	return true;
}

void cone::Draw(void)
{
	glPushMatrix();
	glCallList(display_list_top);
	glPopMatrix();
}
