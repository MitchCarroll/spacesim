#ifndef QUAT_H
#define QUAT_H 1

#include "config.h"
#include <vtx/vtx.h>
#include <cmath>
using namespace std;

class Quat {
 public:
  double w,x,y,z;
  Quat(double angle, Vtx axis);
  ~Quat(){}
};

#endif
