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

Quat::Quat(Vtx v)
{
  w=0;
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

Quat Quat::normalize()
{
  double m=magnitude();
  return Quat(w/m,x/m,y/m,z/m);
}

Quat Quat::operator=(Vtx v)
{
  return Quat(0,v.x,v.y,v.z);
}

Vtx Quat::vtx()
{
  return Vtx(x,y,z);
}

Quat Quat::multiply(Quat q)
{
  Quat r;
  r.w = w*q.w - x*q.x - y*q.y - z*q.z;
  r.x = w*q.x + x*q.w + y*q.z + z*q.y;
  r.y = w*q.y - x*q.z + y*q.w + z*q.x;
  r.z = w*q.z + x*q.y - y*q.x + z*q.w;
  return r;
}

Quat Quat::operator*(Quat q)
{
  return multiply(q);
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

Vtx Quat::rotate(Vtx v)
{ //rotate a 3D vector by a quaternion
  Quat q(w,-x,-y,-z); //q^(-1)
  Quat r=this->multiply(Quat(v)); //qv
  r=r.multiply(q); //(qv)q^(-1)
  return q.vtx(); //is correct?
}

Quat Quat::getRotation()
{
  Quat q=this->normalize();
  Quat r;
  r.w=Acos(q.w)*2;
  double s=sqrt(1-q.w*q.w);
  if(s<0.001) {
    r.x=q.x;
    r.y=q.y;
    r.z=q.z;
  } else {
    r.x=q.x/s;
    r.y=q.y/s;
    r.z=q.z/s;
  }
  return r;
}
