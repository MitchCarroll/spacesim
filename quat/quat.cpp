#include <quat.h>
using namespace std;

Quat::Quat()
{
  w=1;
  x=0;
  y=0;
  z=0;
}

Quat::Quat(double angle, Vtx axis)
{
  w=cos(DEG2RAD(angle)/2);
  double s=sin(DEG2RAD(angle)/2);
  Vtx v=axis.normalize()*s;
  x=v.x;
  y=v.y;
  z=v.z;
}

Quat::Quat(double vw, double vx, double vy, double vz)
{
  w=vw;
  x=vx;
  y=vy;
  z=vz;
}

Quat::~Quat(){}

double Quat::magnitude()
{
  return sqrt(w*w+x*x+y*y+z*z);
}

void Quat::normalize()
{
  double m=magnitude();
  w=w/m;
  x=x/m;
  y=y/m;
  z=z/m;
}

Quat Quat::operator=(Vtx v)
{
  return Quat(0,v.x,v.y,v.z);
}

Vtx Quat::vtx()
{
  return Vtx(x,y,z);
}

Quat Quat::operator*(Quat q)
{
  Quat r;
  r.w = w*q.w - x*q.x - y*q.y - z*q.z;
  r.x = w*q.x + x*q.w + y*q.z + z*q.y;
  r.y = w*q.y - x*q.z + y*q.w + z*q.x;
  r.z = w*q.z + x*q.y - y*q.x + z*q.w;
  return r;
}

Quat Quat::operator*(Vtx v)
{
  return Quat(0,x*v.x,y*v.y,z*v.z);
}

Quat Quat::operator*(double s)
{
  Quat r(w,x,y,z);
  r.w*=s;
  r.x*=s;
  r.y*=s;
  r.z*=s;
  return r; 
}
