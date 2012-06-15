#include "vtx.h"

Vtx::Vtx(void){ x=0; y=0; z=0; }
Vtx::Vtx(double a, double b, double c){ x=a; y=b; z=c; }
Vtx::~Vtx(void){}

double Vtx::magnitude(void){ return sqrt( x*x + y*y + z*z ); }

double Vtx::distance(Vtx v) { return direction(v).magnitude(); }

Vtx Vtx::direction(Vtx v){ return Vtx( x-v.x, y-v.y, z-v.z).normalize(); }

Vtx Vtx::translate(Vtx v){ return Vtx(x+v.x, y+v.y, z+v.z); }

Vtx Vtx::scale(double f){ return Vtx(x*f, y*f, z*f); }

Vtx Vtx::scale(Vtx v){ return Vtx(x*v.x, y*v.y, z*v.z); }

Vtx Vtx::normalize(void){ return scale(1.0/magnitude()); }

Vtx Vtx::colorize(void) //convert the given vector into an RGB color
{                       //suitable for OpenGL DOT3 normalmapping
  return normalize().scale(Vtx(.5,-.5,.5)).translate(Vtx(.5,.5,.5));
}

Vtx Vtx::rotate(double angle, AXIS axis)
{
  angle=DEG2RAD(angle);
  switch(axis) {
  case X:
    return  Vtx(x,
		cos(angle)*y-sin(angle)*z,
		sin(angle)*y+cos(angle)*z);
  case Y:
    return  Vtx(cos(angle)*x-sin(angle)*z,
		y,
		sin(angle)*x+cos(angle)*z);
  case Z:
    return  Vtx(cos(angle)*x-sin(angle)*y,
		sin(angle)*x+cos(angle)*y,
		z);
  default:
    return Vtx(x,y,z);
  }
}

Vtx Vtx::operator+(Vtx v){return translate(v);}
Vtx Vtx::operator-(Vtx v){return translate(v.scale(-1));}
Vtx Vtx::operator*(double d){return scale(d);}
Vtx Vtx::operator/(Vtx v){return v;}
