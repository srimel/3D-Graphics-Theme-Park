/*
 * CS559 Maze Project
 *
 * Class file for the WorldWindow class.
 *
 * (c) Stephen Chenney, University of Wisconsin at Madison, 2001-2002
 *
 */

#include "WorldWindow.h"
#include <Fl/math.h>
#include <Fl/gl.h>
#include <GL/glu.h>
#include <stdio.h>

const double WorldWindow::FOV_X = 45.0;

WorldWindow::WorldWindow(int x, int y, int width, int height, char *label)
	: Fl_Gl_Window(x, y, width, height, label)
{
    button = -1;
	button2 = -1;
	button3 = -1;
	controlIndex = 0;

    // Initial viewing parameters.
    phi = 45.0f;
    theta = 0.0f;
    dist = 100.0f;
    x_at = 0.0f;
    y_at = 0.0f;
	trainEyeX = 0.0f;
	trainEyeY = 0.0f;
	trainEyeZ = 0.0f;
	trainDerX = 0.0f;
	trainDerY = 0.0f;
	trainDerZ = 0.0f;


	// hardcoded close-up views for the objects I created
	eyeView[0].x = 13.630882;
	eyeView[0].y = 3.859577;
	eyeView[0].z = 4.879395;
	gazeView[0].x = 16.770647;
	gazeView[0].y = -96.049637;
	gazeView[0].z = 2.000000;

	eyeView[1].x = 30.211098;
	eyeView[1].y = 3.406841;
	eyeView[1].z = 15.950928;
	gazeView[1].x = -69.895599;
	gazeView[1].y = -48.594421;
	gazeView[1].z = 2.000000;

	eyeView[2].x = 4.874422;
	eyeView[2].y = 38.411864;
	eyeView[2].z = 3.832493;
	gazeView[2].x = -94.218910;
	gazeView[2].y = 25.101965;
	gazeView[2].z = 2.000000;

	eyeView[3].x = 48.198542;
	eyeView[3].y = -25.009768;
	eyeView[3].z = 5.925979;
	gazeView[3].x = -40.474377;
	gazeView[3].y = 21.052149;
	gazeView[3].z = 2.000000;

	eyeView[4].x = 71.937927;
	eyeView[4].y = 19.440718;
	eyeView[4].z = 8.540310;
	gazeView[4].x = -27.771034;
	gazeView[4].y = 23.358294;
	gazeView[4].z = 2.000000;
}


void
WorldWindow::draw(void)
{
    double  eye[3];
    float   color[4], dir[4];

    if ( ! valid() )
    {
		// Stuff in here doesn't change from frame to frame, and does not
		// rely on any coordinate system. It only has to be done if the
		// GL context is damaged.

		double	fov_y;

		// Sets the clear color to sky blue.
		glClearColor(0.53f, 0.81f, 0.92f, 1.0);

		// Turn on depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Turn on back face culling. Faces with normals away from the viewer
		// will not be drawn.
		glEnable(GL_CULL_FACE);

		// Enable lighting with one light.
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

		// Ambient and diffuse lighting track the current color.
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		// Turn on normal vector normalization. You don't have to give unit
		// normal vector, and you can scale objects.
		glEnable(GL_NORMALIZE);

		// Set up the viewport.
		glViewport(0, 0, w(), h());

		// Set up the persepctive transformation.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		fov_y = 360.0f / M_PI * atan(h() * tan(FOV_X * M_PI / 360.0) / w());
		gluPerspective(fov_y, w() / (float)h(), 1.0, 1000.0);

		// Do some light stuff. Diffuse color, and zero specular color
		// turns off specular lighting.
		color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; color[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);

		// Initialize all the objects.
		ground.Initialize();
		traintrack.Initialize();
		for (int i = 0, j=0; i < 5; i++, j += 5)
		{
			concessions[i].InitializeConcession(5 + j, -35, 2, 2);
		}
		a_road.Initialize(0, -30, 0, 1);

		float x = -30.0, y = 35.0;
		float scalex = 5.0;
		float scaley = 5.0;
		float scalez = 0.25;
		float j = 0.25;
		for (int i = 0; i < 5; i++, j+=0.50, scalex--, scaley--)
		{
			pyramid[i].Initialize(x, y, j, scalex, scaley, scalez, "brick2.tga");
		}
		//vec3f d(4, 8, 9);
		//vec3f a(12, 15, 15);
		//a_cone.Initialize(d, a, 25, 10, 30);
		// posx -> right 
		// posy -> up
		// posz -> forward
		a_face.Initialize(15, 3.5, -43.5, 1, 1, 1, 1, 0, 0);
		a_dino.Initialize(0, 3.3, 0, 1, 1, 1);

		faces[0].Initialize(35, 5, 20, 2, 3, 2, (float) 255/255, (float) 160/255, (float) 122/255);
		faces[1].Initialize(37, 6.2, 21.7, 0.5, 0.5, 0.5, (float) 255/255, (float) 255/255, (float) 255/255);
		faces[2].Initialize(35.3, 6.2, 22.2, 0.5, 0.5, 0.5, (float) 255/255, (float) 255/255, (float) 255/255);
    }

    // Stuff out here relies on a coordinate system or must be done on every
    // frame.

    // Clear the screen. Color and depth.
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_ay, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    eye[0] = x_at + dist * cos(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[1] = y_at + dist * sin(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[2] = 2.0 + dist * sin(phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	if (button2 != -1)
	{
		gluLookAt(trainEyeX, trainEyeY, trainEyeZ, trainDerX, trainDerY, trainDerZ, 0, 0, 1);
	}
	else if (button3 != -1)
	{
		gluLookAt(eyeView[controlIndex-1].x, eyeView[controlIndex-1].y, eyeView[controlIndex-1].z, 
				  gazeView[controlIndex-1].x, gazeView[controlIndex-1].y, gazeView[controlIndex-1].z, 0, 0, 1);
	}
	else {
		gluLookAt(eye[0], eye[1], eye[2], x_at, y_at, 2.0, 0.0, 0.0, 1.0);
		//printf("eye: %f, %f, %f\ngaze: %f, %f, %f\n", eye[0], eye[1], eye[2], x_at, y_at, 2.0);
	}

    // Position the light source. This has to happen after the viewing
    // transformation is set up, so that the light stays fixed in world
    // space. This is a directional light - note the 0 in the w component.
    dir[0] = 1.0; dir[1] = 1.0; dir[2] = 1.0; dir[3] = 0.0;
    glLightfv(GL_LIGHT0, GL_POSITION, dir);

    // Draw stuff. Everything.
    ground.Draw();
    traintrack.Draw();
	for (int i = 0; i < 5; i++)
	{
		concessions[i].Draw();
	}
	a_road.Draw();
	for (int i = 0; i < 5; i++)
	{
		pyramid[i].Draw();
	}
	glm::vec3 pos(5, 5, 10);
	glm::vec3 col(1, 0, 1);
	a_face.Draw();
	a_dino.Draw();
	//a_tree.Draw(5,5,5, 1, 20);
	//posx -> forwards
	//posy ->  right
	trees[0].Draw(-30, -25, 0, 1, 10);
	trees[1].Draw(-35, -35, 0, 0.5, 15);
	trees[2].Draw(-35, -10, 0, 2, 15);
	trees[3].Draw(-15, -40, 0, 1.5, 10);
	faces[0].Draw();
	faces[1].Draw();
	faces[2].Draw();
}


void
WorldWindow::Drag(float dt)
{
    int	    dx = x_down - x_last;
    int     dy = y_down - y_last;

    switch ( button )
    {
      case FL_LEFT_MOUSE:
		// Left button changes the direction the viewer is looking from.
		theta = theta_down + 360.0f * dx / (float)w();
		while ( theta >= 360.0f )
			theta -= 360.0f;
		while ( theta < 0.0f )
			theta += 360.0f;
		phi = phi_down + 90.0f * dy / (float)h();
		if ( phi > 89.0f )
			phi = 89.0f;
		if ( phi < -5.0f )
			phi = -5.0f;
		break;
      case FL_MIDDLE_MOUSE:
		// Middle button moves the viewer in or out.
		dist = dist_down - ( 0.5f * dist_down * dy / (float)h() );
		if ( dist < 1.0f )
			dist = 1.0f;
		break;
      case FL_RIGHT_MOUSE: {
		// Right mouse button moves the look-at point around, so the world
		// appears to move under the viewer.
		float	x_axis[2];
		float	y_axis[2];

		x_axis[0] = -(float)sin(theta * M_PI / 180.0);
		x_axis[1] = (float)cos(theta * M_PI / 180.0);
		y_axis[0] = x_axis[1];
		y_axis[1] = -x_axis[0];

		x_at = x_at_down + 100.0f * ( x_axis[0] * dx / (float)w()
						+ y_axis[0] * dy / (float)h() );
		y_at = y_at_down + 100.0f * ( x_axis[1] * dx / (float)w()
						+ y_axis[1] * dy / (float)h() );
		} break;
      default:;
    }
}

void WorldWindow::norm(float v[3])
{
    double  l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    if ( l == 0.0 )
	return;

    v[0] /= (float)l;
    v[1] /= (float)l;
    v[2] /= (float)l;
}


bool
WorldWindow::Update(float dt)
{
    // Update the view. This gets called once per frame before doing the
    // drawing.

    // Animate the train.
	float current = traintrack.Update(dt);

	//button controls
    if ( button != -1 ) // Only do anything if the mouse button is down.
		Drag(dt);


	if (button2 != -1)
	{
		float posn[3];
		float tangent[3];
		traintrack.track->Evaluate_Point(traintrack.posn_on_track, posn);
		trainEyeX = posn[0];
		trainEyeY = posn[1];
		trainEyeZ = posn[1];

		float posn_at[3];
		float dist = traintrack.posn_on_track + 0.1;
		traintrack.track->Evaluate_Point(dist, posn_at);
		trainDerX = posn_at[0];
		trainDerY = posn_at[1];
		trainDerZ = posn_at[2] + 0.5;


		/*
		traintrack.track->Evaluate_Derivative(traintrack.posn_on_track, tangent);
		norm(tangent);
		float d = 1;
		trainDerX = (tangent[0]*d);
		trainDerY = (tangent[1]*d);
		trainDerZ = (tangent[2]*d);

		float posn_at[3];
		float dist = traintrack.posn_on_track + 0.1;
		traintrack.track->Evaluate_Point(dist, posn_at);
		trainDerX = posn_at[0];
		trainDerY = posn_at[1];
		trainDerZ = posn_at[2];

		track->Evaluate_Derivative(posn_on_track, tangent);
		Normalize_3(tangent);

		// Rotate it to poitn along the track, but stay horizontal
		angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
		glRotatef((float)angle, 0.0f, 0.0f, 1.0f);

		// Another rotation to get the tilt right.
		angle = asin(-tangent[2]) * 180.0 / M_PI;
		glRotatef((float)angle, 0.0f, 1.0f, 0.0f);
		*/
	}

    return true;
}


int
WorldWindow::handle(int event)
{
    // Event handling routine. Only looks at mouse events.
    // Stores a bunch of values when the mouse goes down and keeps track
    // of where the mouse is and what mouse button is down, if any.
    switch ( event )
    {
      case FL_PUSH:
        button = Fl::event_button();
		x_last = x_down = Fl::event_x();
		y_last = y_down = Fl::event_y();
		phi_down = phi;
		theta_down = theta;
		dist_down = dist;
		x_at_down = x_at;
		y_at_down = y_at;
		return 1;
      case FL_DRAG:
		x_last = Fl::event_x();
		y_last = Fl::event_y();
		return 1;
      case FL_RELEASE:
        button = -1;
		return 1;
	  case FL_KEYBOARD:
		  int key = Fl::event_key();
		  switch (key)
		  {
		  case FL_Left:
			  y_at -= 5;
			  return 1;

		  case FL_Right:
			  y_at += 5;
			  return 1;
		  case FL_Up:
			  x_at -= 5;
			  return 1;
		  case FL_Down:
			  x_at += 5;
			  return 1;
		  case FL_Enter:
			  button2 = Fl::event_button();
			  return 1;
		  case FL_BackSpace:
			  button2 = -1;
			  button3 = -1;
			  return 1;
		  case FL_Tab:
			  button3 = Fl::event_button();
			  if (controlIndex < 5)
				  ++controlIndex;
			  else
				  controlIndex = 1;
			  return 1;
		  }

    }
    // Pass any other event types on the superclass.
    return Fl_Gl_Window::handle(event);
}


