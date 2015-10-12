#include <body.h>
using namespace std;

void
Body::initQuadric (void)
{
  quadric = gluNewQuadric ();
  gluQuadricDrawStyle (quadric, GLU_FILL);
  gluQuadricTexture (quadric, GL_TRUE);
  gluQuadricNormals (quadric, GLU_SMOOTH);
}

Body::Body ()
{
  initQuadric ();
  for (int i = 0; i < 4; i++)
    mat_dif[i] = 1.0;
}

Body::~Body ()
{
}

void
Body::setThrust (double v)
{
  thrust = rot.rotate (Vtx (0, 0, 1)) * v;
}

double
Body::gravity (Body m)
{
  return G * mass * m.mass / pow (pos.distance (m.pos), 2);
}

double
Body::altitude (Body m)
{
  return pos.distance (m.pos) - (radius + m.radius);
}

void
Body::display (void)
//sends the appropriate OpenGL commands to display the body
{
  double an = rot.angle ();
  Vtx ax = rot.axis ();
  glPushMatrix ();
  glTranslatef (pos.x, pos.y, pos.z);
  glRotatef (an, ax.x, ax.y, ax.z);
  glBindTexture (GL_TEXTURE_2D, texture);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
  gluSphere (quadric, radius, 36, 18); //TODO: Find a good set of numbers...
  //perhaps a routine which will select numbers based on size
  glPopMatrix ();
}
