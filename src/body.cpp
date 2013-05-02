#include <body.h>
using namespace std;

Body::Body(){}
Body::~Body(){}
void Body::setThrust(double v)
{
  thrust=Vtx(0,0,1).rotate(rot.vtx().y,Y).rotate(rot.vtx().x,X).rotate(rot.vtx().z,Z)*v;
}

double Body::gravity(Body m)
{
  return G*mass*m.mass/pow(pos.distance(m.pos),2);
}

double Body::altitude(Body m)
{
  return pos.distance(m.pos)-(radius+m.radius);
}


