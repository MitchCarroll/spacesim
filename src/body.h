#ifndef BODY_H
#define BODY_H 1

#include <vtx/vtx.h>
#include <quat/quat.h>
#include <GL/glut.h>
using namespace std;

const double G = 6.673E-11;	//universal gravitational constant

class Body
{
public:
  Body ();
  ~Body ();
  void setThrust (double v);
  double gravity (Body m);
  double altitude (Body m);
  void display (void);
  void initQuadric (void);

  GLUquadricObj *quadric;	//for drawing spheres and stuff
  GLuint texture;
  GLfloat mat_dif[4];

  double mass;			//in kg
  double radius;		//in m (used for bounding volumes, and rendering celestial bodies)
  Vtx pos;			//in m
  Quat rot;			//in deg
  Vtx vel;			//in m/s
  Vtx axis;
  double rvel;			//in deg/s
  Vtx thrust;			//thrust vector in m/s
};
#endif
