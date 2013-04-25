#ifndef QUAT_H
#define QUAT_H 1

#include "config.h"
#include <vtx/vtx.h>
#include <cmath>
using namespace std;

class Quat {
 public:
  double w,x,y,z;
  Quat(double angle, Vtx axis); //axis/angle initialization
  Quat(double w, double x, double y, double z); //raw initialization (w,x,y,z)
  Quat(); //identity (1,0,0,0)
  ~Quat();
  double magnitude();
  void normalize();
  Quat operator=(Vtx v);
  Vtx vtx();
  Quat operator*(Quat q);
  Quat operator*(Vtx v);
  Quat operator*(double s);

};

#endif
