#include <quat.h>
using namespace std;

Quat::Quat(double angle, Vtx axis)
{
  w=cos(DEG2RAD(angle)/2);
  double s=sin(DEG2RAD(angle)/2);
  Vtx v=axis*s;
  x=v.x;
  y=v.y;
  z=v.z;
}
