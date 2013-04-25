#ifndef BODY_H
#define BODY_H 1

#include <vtx/vtx.h>
#include <quat/quat.h>
using namespace std;

const double G = 6.673E-11; //universal gravitational constant

class Body {
 public:
  Body();
  ~Body();
  void setThrust(double v);
  double gravity(Body m);
  double altitude(Body m);
  double mass;   //in kg
  double radius; //in m (used for bounding volumes, and rendering celestial bodies)
  Vtx pos;       //in m
  Quat rot;       //in deg
  Vtx vel;       //in m/s
  Quat rvel;      //in deg/s
  Vtx thrust;    //thrust vector in m/s
};
#endif
