#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vtx/vtx.h> 
using namespace std;

const double G = 6.673E-11;

double zoom=2;

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

double gravity(BODY M, BODY m)
{
  return G*M.mass*m.mass/pow(M.pos.distance(m.pos),2);
}

void initEarth(void)
{
  Earth.mass=5.9736E24;
  Earth.radius=6371E3;
  Earth.pos=Vtx(0,0,0);
  Earth.rot=Vtx(0,0,0);
  Earth.vel=Vtx(0,0,0);
  Earth.rvel=Vtx(0,360.0/24/60/60,0);
  
}

void initRocket(void)
{
  Rocket.mass=1000;
  Rocket.radius=5;
  Rocket.pos=Vtx(Earth.radius+2.5,0,0);
  Rocket.rot=Vtx(0,90,0);
  Rocket.vel=Vtx(0,0,0);
  Rocket.rvel=Vtx(0,0,0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glTranslatef(-Rocket.pos.x,-Rocket.pos.y,-Rocket.pos.z-Rocket.radius*zoom);
  glColor3f(.25,.3,1);
  glutWireSphere(Earth.radius,360,180);
  glLoadIdentity();
  glTranslatef(0,0,-Rocket.radius*zoom);
  glRotatef(Rocket.rot.x,1,0,0);
  glRotatef(Rocket.rot.y,0,1,0);
  glRotatef(Rocket.rot.z,0,0,1);
  glColor3f(.7,.7,.7);
  glutSolidCone(2.5,5,10,5);
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  initEarth();
  initRocket();
  glutInit(&argc,argv);
  glutInitWindowSize(512,512);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutCreateWindow("Orbit Sim");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90,1,0.001,1.7E308);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0,0,0,0);
  glutDisplayFunc(display);
  glutMainLoop();
  
  return 0;
}
