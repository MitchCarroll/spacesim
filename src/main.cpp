#include <config.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vtx/vtx.h>
#include <quat/quat.h>
#include <body.h>
using namespace std;

long int mission_time=0;

class SpaceShip:public Body
//TODO: create an all inclusive class for 
// spacecraft, and expose its particulars to Guile (e.g.: make-craft, set-mass)
{
public:
  void display (void);
};

void
SpaceShip::display (void)
{
  double an = rot.angle ();
  Vtx ax = rot.axis ();
  glPushMatrix ();
  glRotatef (an, ax.x, ax.y, ax.z);
  glBindTexture (GL_TEXTURE_2D, texture);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
  glScalef (1, .25, 1);
  glRotatef (180, 0, 0, 1);
  gluCylinder (quadric, 2.5, 0, 5, 10, 5);
  glScalef (1, 1, .25);
  gluSphere (quadric, 2.5, 10, 10);
  glScalef (0.01, 1, 3);
  glRotatef (-9, 1, 0, 0);
  glTranslatef (0, -.8, 0);
  gluCylinder (quadric, 4, 0, 5, 10, 5);
  glPopMatrix ();
}

double zoom = 2;

Body Earth;
SpaceShip Rocket;
Quat camera;			//camera angle

double throttle = 100;		//holds the throttle setting for the rocket

GLfloat sun_pos[] = { 1.0, 0.0, 0.0, 0.0 };
GLfloat sun_amb[] = { 0.01, 0.01, 0.01, 1.0 };
GLfloat sun_dif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat sun_spe[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat rocket_dif[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat ball_dif[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat ball_amb[] = { 0.75, 0.75, 0.75, 1.0 };

GLuint ballTexture = 0;

void
quit ()
{
  //at some point, some "de-initialization" needs to be done. deleting textures and such.
  exit (0);
}

void
loadTexture (const char *filename, GLuint & tex)
{
  int w = 0, h = 0;
  string comment;
  GLubyte *tD;
  ifstream infile;
  infile.open (filename);
  getline (infile, comment);	//Magic Number (should always be P6
  getline (infile, comment);	//comment (doesn't matter)
  infile >> w >> h;
  getline (infile, comment);	//color depth (should always be 255)
  GLubyte *temp = new GLubyte[h * h * 3];
  tD = new GLubyte[w * h * 3];
  for (int line = 0; line < h; line++)
    {
      for (int byte = w * 3; byte > 0; byte -= 3)
	{
	  tD[line * w * 3 + byte + 2] = infile.get ();
	  tD[line * w * 3 + byte] = infile.get ();
	  tD[line * w * 3 + byte + 1] = infile.get ();
	}
    }
  infile.close ();
  glGenTextures (1, &tex);
  glBindTexture (GL_TEXTURE_2D, tex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB,
		     w, h, GL_RGB, GL_UNSIGNED_BYTE, tD);
  delete[]tD, temp;
}

void
initEarth (void)
{
  loadTexture (DATADIR "earth.pnm", Earth.texture);
  Earth.mass = 5.9736E15;
  Earth.radius = 6371E3;
  Earth.pos = Vtx (0, 0, 0);
  Earth.rot = Quat (23.5, Vtx (0, 1, 0)) * Quat (90, Vtx (1, 0, 0));	//give the Earth it's tilt
  Earth.vel = Vtx (0, 0, 0);
  Earth.axis = Vtx (0, 0, 1);
  Earth.rvel = 360 / 24 / 60 / 60 / 24;	//start the Earth spinning
}

void
initRocket (void)
{
  loadTexture (DATADIR "rocket.pnm", Rocket.texture);
  loadTexture (DATADIR "8ball.pnm", ballTexture);
  Rocket.mass = 1000;
  Rocket.radius = 5;
  Rocket.pos = Earth.pos + Vtx (Earth.radius+420000, 0, 0);	//TODO: Find a better value
  Rocket.vel = Vtx (0, 0, 7658.50387).rotate(51.65, Z);	//TODO: find a good value 
  //FIXME: use a config 
  //TODO: make a function to determine circular speed or something
  Rocket.rvel = 0.000001;
  Rocket.axis = Vtx (0, 0, 1);
}

void
display (void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  Vtx r = Rocket.rot.axis ();
  Quat e = Earth.rot.axis ();
  Vtx c = camera.rotate (Vtx (0, 0, -1)) * Rocket.radius * zoom;
  Vtx u = camera.rotate (Vtx (0, 1, 0));
  
  //draw 8-ball
  //TODO: expose an API, and leave this to the interface and extension code
  glPushMatrix ();
  glBindTexture (GL_TEXTURE_2D, ballTexture);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, ball_dif);
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, ball_amb);
  glTranslatef (0, -.6, -.75);
  glRotatef (50, 1, 0, 0);
  glRotatef (-Rocket.rot.angle (), r.x, r.z, r.y);
  gluSphere (Rocket.quadric, .125, 36, 18);
  glPopMatrix ();

  //apply camera transformation
  gluLookAt (c.x, c.y, c.z,
	     0, 0, 0,
	     u.x, u.y, u.z);
  glLightfv (GL_LIGHT0, GL_POSITION, sun_pos);

  //draw rocket
  Rocket.display ();

  //draw earth
  glPushMatrix ();
  glTranslatef (-Rocket.pos.x, -Rocket.pos.y, -Rocket.pos.z);
  Earth.display ();
  glPopMatrix ();

  glutSwapBuffers ();
}

void
timer (int t)
{
  if (Rocket.altitude (Earth) > 0)
    Rocket.vel =
      Rocket.vel +
      (Rocket.pos.direction (Earth.pos) * Earth.gravity (Rocket));
  else
    {
      Rocket.vel = Vtx (0, 0, 0);
      Rocket.rvel = 0;
    }
  Rocket.pos = Rocket.pos + Rocket.vel / 24;
  Rocket.rot = Rocket.rot * Quat (Rocket.rvel, Rocket.axis);
  Earth.rot = Earth.rot * Quat (Earth.rvel, Earth.axis);
  Rocket.setThrust (throttle);
  glutPostRedisplay ();
  glutTimerFunc (t, timer, t);
}

void
special (int key, int x, int y)
{
  Quat rv (Rocket.rvel, Rocket.axis);
  switch (key)
    {
    case GLUT_KEY_LEFT:	//roll left
      rv = rv * Quat (-.1, 0, 0, 1);
      break;
    case GLUT_KEY_RIGHT:	//roll right
      rv = rv * Quat (.1, 0, 0, 1);
      break;
    case GLUT_KEY_UP:		//pitch down
      rv = rv * Quat (.1, 1, 0, 0);
      break;
    case GLUT_KEY_DOWN:	//pitch up 
      ///friends don't let friends use inverted controls
      rv = rv * Quat (-.1, 1, 0, 0);
      break;
    default:
      break;
    }
  Rocket.axis = rv.axis ();
  Rocket.rvel = rv.angle ();
}

void
keyboard (unsigned char key, int x, int y)
{
  Quat rv (Rocket.rvel, Rocket.axis);
  switch (key)
    {
    case 27:			// quit
      quit ();
      break;
    case ',':			//yaw left
      rv = rv * Quat (.1, 0, 1, 0);
      break;
    case '.':			//yaw right
      rv = rv * Quat (-.1, 0, 1, 0);
      break;
      //////camera rotation
    case 'a':
      camera=Quat(-2,0,1,0)*camera;
      break;
    case 'd':
      camera=Quat(2,0,1,0)*camera;
      break;
    case 'w':
      camera=Quat(-2,1,0,0)*camera;
      break;
    case 's':
      camera=Quat(2,1,0,0)*camera;
      break;
    case ' ':
      cout << time(NULL)-mission_time << " " 
	   << Rocket.altitude (Earth) << " " 
	   << Rocket.vel.magnitude () << " " 
	   << Rocket.pos.x << " " 
	   << Rocket.pos.y << " " 
	   << Rocket.pos.z << endl;
    default:
      break;
    }
}

int
main (int argc, char **argv)
{
  glutInit (&argc, argv);
  glutInitWindowSize (1024, 600);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow (PACKAGE_STRING);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (90, 1024.0 / 600, .1, 1.7E308);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glClearColor (0, 0, 0, 0);
  glEnable (GL_TEXTURE_2D);
  glEnable (GL_LIGHTING);
  glShadeModel (GL_SMOOTH);
  glEnable (GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glDepthFunc (GL_LEQUAL);
  glLightfv (GL_LIGHT0, GL_POSITION, sun_pos);
  glLightfv (GL_LIGHT0, GL_AMBIENT, sun_amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, sun_dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, sun_spe);
  glutDisplayFunc (display);
  glutTimerFunc (1000 / 24, timer, 1000 / 24);
  glutSpecialFunc (special);
  initEarth ();
  initRocket ();
  camera = Quat (.1, 0, 0, 1);
  glutKeyboardFunc (keyboard);
  mission_time=time(NULL);
  glutMainLoop ();
  return 0;
}
