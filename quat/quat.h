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
  Quat(Vtx v); //quaternion representation of a vector
  Quat(double w, double x, double y, double z); //raw initialization (w,x,y,z)
  Quat(); //identity (1,0,0,0)
  ~Quat();
  double magnitude();
  Quat normalize();
  Quat operator=(Vtx v);
  Vtx vtx();
  Quat multiply(Quat q);
  Quat operator*(Quat q);
  Quat operator*(Vtx v);
  Quat operator*(double s);
  Vtx rotate(Vtx v);
  Quat getRotation();
};

#endif
