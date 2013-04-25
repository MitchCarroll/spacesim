#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vtx/vtx.h> 
#include <quat/quat.h>
#include <body.h>
using namespace std;


double zoom=2;

//TODO: FIXME: obviously, these are going to be made into classes, and split into different files

Body Earth;
Body Rocket;
Body camera; //massless
double throttle=100; //holds the throttle setting for the rocket

GLfloat sun_pos[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_amb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat sun_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_spe[] = {1.0, 1.0, 1.0, 1.0};
GLfloat earth_dif[] = {0.2, 0.2, 1.0, 1.0};
GLfloat rocket_dif[] = {0.5, 0.5, 0.5, 1.0};

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
  Rocket.pos=Vtx(Earth.radius+35786000,0,0);
  Rocket.rot=Vtx(0,0,0);
  Rocket.vel=Vtx(0,0,110680);
  Rocket.rvel=Vtx(0,0,0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glLightfv(GL_LIGHT0,GL_POSITION,sun_pos);
  glRotatef(-camera.rot.y,0,1,0);
  glRotatef(-camera.rot.x,1,0,0);
  glRotatef(-camera.rot.z,0,0,1);
  glTranslatef(camera.pos.x,camera.pos.y,camera.pos.z);

  glPushMatrix();
  glEnable(GL_DEPTH_TEST);
  glBegin(GL_TRIANGLES);
  glVertex3f(0,0,0);
  glVertex3f(0,-1,0);
  glVertex3f(Rocket.thrust.x,Rocket.thrust.y,Rocket.thrust.z);
  glEnd();

  glRotatef(Rocket.rot.y,0,1,0);
  glRotatef(Rocket.rot.x,1,0,0);
  glRotatef(Rocket.rot.z,0,0,1);

  glBegin(GL_TRIANGLES);
  glVertex3f(0,0,0);
  glVertex3f(0,1.25,-1);
  glVertex3f(0,0,4);
  glEnd();
  
  glColor3f(.7,.7,.7);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, rocket_dif);
  glScalef(1,.25,1);
  glutSolidCone(2.5,5,10,5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-Rocket.pos.x,-Rocket.pos.y,-Rocket.pos.z);
  glRotatef(Earth.rot.y,0,1,0);
  glRotatef(Earth.rot.x,1,0,0);
  glRotatef(Earth.rot.z,0,0,1);
  glColor3f(.25,.3,1);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, earth_dif);
  //  glDisable(GL_DEPTH_TEST);
  glutSolidSphere(Earth.radius,36,18);
  glPopMatrix();

  glutSwapBuffers();
}

void timer(int t)
{
  if(Rocket.altitude(Earth)>0)
    Rocket.vel=Rocket.vel+(Rocket.pos.direction(Earth.pos)*Earth.gravity(Rocket));
  else
    Rocket.vel=Vtx(0,0,0);
  Rocket.pos=Rocket.pos+Rocket.vel/24;
  Rocket.rot=Rocket.rot.vtx()+Rocket.rvel.vtx()/24;
  Earth.rot=Earth.rot.vtx()+Earth.rvel.vtx()/24;
  Rocket.setThrust(throttle);
  glutPostRedisplay();
  glutTimerFunc(1000/24,timer,1000/24);
}

void special(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_LEFT:
    Rocket.rvel.y--;
    break;
  case GLUT_KEY_RIGHT:
    Rocket.rvel.y++;
    break;
  case GLUT_KEY_UP:
    Rocket.rvel.x--;
    break;
  case GLUT_KEY_DOWN:
    Rocket.rvel.x++;
    break;
  default:
    break;
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key){
  case 'a':
    camera.rot.y-=2;
    camera.pos=Vtx(0,0,Rocket.radius*zoom*-1).rotate(-camera.rot.y,Y).rotate(camera.rot.x,X);
    break;
  case 'd':
    camera.rot.y+=2;
    camera.pos=Vtx(0,0,Rocket.radius*zoom*-1).rotate(-camera.rot.y,Y).rotate(camera.rot.x,X);
    break;
  case 'w':
    camera.rot.x-=2;
    camera.pos=Vtx(0,0,Rocket.radius*zoom*-1).rotate(-camera.rot.y,Y).rotate(camera.rot.x,X);
    break;
  case 's':
    camera.rot.x+=2;
    camera.pos=Vtx(0,0,Rocket.radius*zoom*-1).rotate(-camera.rot.y,Y).rotate(camera.rot.x,X);
    
    break;
  case ' ':
    cout << Rocket.altitude(Earth) << " " << Rocket.pos.x << " " << Rocket.pos.y << " " << Rocket.pos.z << endl;
  default:
    break;
  }
}

int main(int argc, char **argv)
{
  initEarth();
  initRocket();
  camera.pos=Vtx(0,0,-Rocket.radius*zoom);
  glutInit(&argc,argv);
  glutInitWindowSize(512,512);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutCreateWindow("Orbit Sim");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90,1,.1,1.7E308);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0,0,0,0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glLightfv(GL_LIGHT0,GL_POSITION,sun_pos);
  glLightfv(GL_LIGHT0,GL_AMBIENT,sun_amb);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,sun_dif);
  glLightfv(GL_LIGHT0,GL_SPECULAR,sun_spe);
  glutDisplayFunc(display);
  glutTimerFunc(1000/24,timer,1000/24);
  glutSpecialFunc(special);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  
  return 0;
}
