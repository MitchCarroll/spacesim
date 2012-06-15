#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vtx/vtx.h> 
using namespace std;

const double G = 6.673E-11;

typedef struct {
  double mass;   //in kg
  double radius; //in m (used for bounding volumes, and rendering celestial bodies)
  Vtx pos;       //in m
  Vtx rot;       //in deg
  Vtx vel;       //in m/s
  Vtx rvel;      //in deg/s
} BODY;

BODY Earth;
BODY Rocket;

void initEarth(void)
{
  Earth.mass=5.9736E24;
  Earth.radius=6378.1E3;
  Earth.pos=Vtx(0,0,0);
  Earth.rot=Vtx(0,0,0);
  Earth.vel=Vtx(0,0,0);
  Earth.rvel=Vtx(0,0,0);
  
}

void initRocket(void)
{
  Rocket.mass=1000;
  Rocket.pos=Vtx(0,0,0);
  Rocket.rot=Vtx(0,0,0);
  Rocket.vel=Vtx(0,0,0);
  Rocket.rvel=Vtx(0,0,0);
}

int main(int argc, char **argv)
{
  initEarth();
  initRocket();
  glutInit(&argc,argv);
  glutInitWindowSize(512,512);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutCreateWindow("Orbit Sim");
  glutMainLoop();
  
  return 0;
}
