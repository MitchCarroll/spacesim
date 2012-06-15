#ifndef VTX_H
#define VTX_H 1

#include <cmath> //for trig

//some trig functions using 30 degrees in lieu of radians
#define DEG2RAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RAD2DEG(radian) ((radian) * (180.0f / 3.141592654f))
#define Sin(angle) (sin(DEG2RAD(angle))) //SOH CAH TOA :)
#define Cos(angle) (cos(DEG2RAD(angle)))
#define Tan(val) (tan(DEG2RAD(angle)))
#define Asin(val) (RAD2DEG(asin(val)))
#define Acos(val) (RAD2DEG(acos(val)))
#define Atan(val) (RAD2DEG(atan(val)))

typedef enum { X, Y, Z } AXIS; 

//PROTOTYPE to make the compiler happy
class Vtx;

class Vtx {
 public:
  double x,y,z;
  Vtx(void);
  Vtx(double a, double b, double c);
  ~Vtx(void);
  double magnitude(void);
  double distance(Vtx v);
  Vtx direction(Vtx v);
  Vtx translate(Vtx v);
  Vtx rotate(double angle, AXIS axis);
  Vtx scale(double f);
  Vtx scale(Vtx v);
  Vtx normalize(void);
  Vtx colorize(void);
};

#endif
