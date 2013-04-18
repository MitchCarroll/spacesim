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

Vtx camera;      //view angles
GLfloat sun_pos[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_amb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat sun_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_spe[] = {1.0, 1.0, 1.0, 1.0};
GLfloat earth_dif[] = {0.2, 0.2, 1.0, 1.0};
GLfloat rocket_dif[] = {0.5, 0.5, 0.5, 1.0};

double gravity(BODY M, BODY m)
{
  return G*M.mass*m.mass/pow(M.pos.distance(m.pos),2);
}

double altitude(BODY M, BODY m)
{
  return M.pos.distance(m.pos)-(M.radius+m.radius);
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
  Rocket.pos=Vtx(Earth.radius*2,0,0);
  Rocket.rot=Vtx(0,90,0);
  Rocket.vel=Vtx(0,1000000,0);
  Rocket.rvel=Vtx(0,0,0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(camera.x,1,0,0);
  glRotatef(camera.y,0,1,0);
  glRotatef(camera.z,0,0,1);
  glTranslatef(-Rocket.pos.x,-Rocket.pos.y,-Rocket.pos.z-Rocket.radius*zoom);
  glColor3f(.25,.3,1);
  // glDisable(GL_LIGHTING);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, earth_dif);
  glutWireSphere(Earth.radius,36,18);
  // glEnable(GL_LIGHTING);
  glLoadIdentity();
  glLightfv(GL_LIGHT0,GL_POSITION,sun_pos);
  glTranslatef(0,0,-Rocket.radius*zoom);
  glRotatef(camera.x,1,0,0);
  glRotatef(camera.y,0,1,0);
  glRotatef(camera.z,0,0,1);
  glRotatef(Rocket.rot.x,1,0,0);
  glRotatef(Rocket.rot.y,0,1,0);
  glRotatef(Rocket.rot.z,0,0,1);
  glColor3f(.7,.7,.7);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, rocket_dif);
  glutSolidCone(2.5,5,10,5);
  glutSwapBuffers();
}

void timer(int t)
{
  if(altitude(Earth,Rocket)>0)
    Rocket.vel=Rocket.vel+(Rocket.pos.direction(Earth.pos)*gravity(Earth,Rocket));
  else
    Rocket.vel=Vtx(0,0,0);
  Rocket.pos=Rocket.pos+Rocket.vel/24;
  Rocket.rot=Rocket.rot+Rocket.rvel/24;
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
    camera.y--;
    break;
  case 'd':
    camera.y++;
    break;
  case 'w':
    camera.x--;
    break;
  case 's':
    camera.x++;
    break;
  case ' ':
    cout << altitude(Rocket,Earth) << endl;
  default:
    break;
  }
}

int main(int argc, char **argv)
{
  initEarth();
  initRocket();
  camera=Vtx(0,180,0);
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
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //  glEnable(GL_DEPTH_TEST);
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
