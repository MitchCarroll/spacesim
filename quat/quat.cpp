#include <quat.h>
using namespace std;

Quat::Quat ()
{
  w = 1;
  x = 0;
  y = 0;
  z = 0;
}

Quat::Quat (double angle, Vtx axis)
{
  Quat q;
  q.w = cos (DEG2RAD (angle) / 2);
  double s = sin (DEG2RAD (angle) / 2);
  Vtx n = axis.normalize () * s;
  q.x = n.x;
  q.y = n.y;
  q.z = n.z;
  //  q=q.normalize();
  w = q.w;
  x = q.x;
  y = q.y;
  z = q.z;
}

Quat::Quat (Vtx v)
{
  Vtx n = v.normalize ();
  w = 0;
  x = n.x;
  y = n.y;
  z = n.z;
}

Quat::Quat (double vw, double vx, double vy, double vz)
{
  Quat q (vw, Vtx (vx, vy, vz));
  w = q.w;
  x = q.x;
  y = q.y;
  z = q.z;
}

Quat::~Quat ()
{
}



double
Quat::magnitude ()
{
  return sqrt (w * w + x * x + y * y + z * z);
}

Quat Quat::normalize ()
{
  double
    m = magnitude ();
  Quat
    q;
  q.w = w / m;
  q.x = x / m;
  q.y = y / m;
  q.z = z / m;
  return q;
}

Quat Quat::operator= (Vtx v)
{
  return Quat (v);
}


Quat Quat::multiply (Quat q)
{
  Quat
    r;
  r.w = w * q.w - x * q.x - y * q.y - z * q.z;
  r.x = w * q.x + x * q.w + y * q.z + z * q.y;
  r.y = w * q.y - x * q.z + y * q.w + z * q.x;
  r.z = w * q.z + x * q.y - y * q.x + z * q.w;

  //return r;
  return r.normalize ();	//should normalize?
}

Quat Quat::operator* (Quat q)
{
  return multiply (q);
}

Quat Quat::operator* (Vtx v)
{
  return multiply (Quat (v));
}

Quat Quat::operator* (double s)
{
  Quat
  r (w, x, y, z);
  r.w *= s;
  r.x *= s;
  r.y *= s;
  r.z *= s;
  return r;
  //  return multiply(Quat(s,0,0,0)); //more elegant / accurate?
}

Vtx Quat::rotate (Vtx v)
{				//rotate a 3D vector by a quaternion
  Quat
  q (w, -x, -y, -z);		//q^(-1)
  Quat
    r = this->multiply (Quat (v));	//qv
  r = r.multiply (q);		//(qv)q^(-1)
  return Vtx (r.x, r.y, r.z);	//is correct?
}

double
Quat::angle ()			//in degrees
{
  return Acos (w) * 2.0;
}

Vtx Quat::axis ()
{
  Vtx
    r;
  double
    s = sqrt (1.0 - w * w);
  if (s == 0.0)
    {				//eh?
      r.x = x;
      r.y = y;
      r.z = z;
    }
  else
    {
      r.x = x / s;
      r.y = y / s;
      r.z = z / s;
    }
  return r.normalize ();
}
