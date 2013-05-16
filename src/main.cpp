#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vtx/vtx.h> 
#include <quat/quat.h>
#include <body.h>
using namespace std;

double zoom=2;

Body Earth;
Body Rocket;
Body camera; //massless
GLUquadricObj *earthMesh;
double throttle=100; //holds the throttle setting for the rocket

GLfloat sun_pos[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_amb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat sun_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_spe[] = {1.0, 1.0, 1.0, 1.0};
GLfloat earth_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat rocket_dif[] = {0.5, 0.5, 0.5, 1.0};

GLubyte *etD;
GLuint earthTexture=0;
int etW=0, etH=0;

void loadTexture(const char *filename)
{
  ifstream infile;
  infile.open(filename);
  infile >> etW >> etH;
  GLubyte *temp=new GLubyte[etW*etH*3];
  etD=new GLubyte[etW*etH*3];
  for(int b=0;b<etW*etH*3;b+=3) {
    etD[b+2]=infile.get();
    etD[b+1]=infile.get();
    etD[b]=infile.get();
  }
  infile.close();
  cout << etW << "x" << etH << endl;
  glGenTextures(1,&earthTexture);
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
	       etW, etH, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, etD);
  delete [] etD,temp;
}

void initEarth(void)
{
  loadTexture(DATADIR "earth.pnm");
  earthMesh=gluNewQuadric();
  gluQuadricDrawStyle(earthMesh, GLU_FILL);
  gluQuadricTexture(earthMesh, GL_TRUE);
  gluQuadricNormals(earthMesh, GLU_SMOOTH);
  Earth.mass=5.9736E24;
  Earth.radius=6371E3;
  Earth.pos=Vtx(0,0,0);
  Earth.rot=Quat(23.5,Vtx(1,0,0)); //give the Earth it's tilt
  Earth.vel=Vtx(0,0,0);
  Earth.rvel=Quat(360/24/60/60/24,Vtx(0,1,0)); //start the Earth spinning
  
}

void initRocket(void)
{
  Rocket.mass=1000;
  Rocket.radius=5;
  Rocket.pos=Vtx(Earth.radius+35786000,0,0);
  Rocket.rot=Quat(1,0,0,0);
  Rocket.vel=Vtx(0,0,110680*4); //TODO: find a good value FIXME: use a config
  Rocket.rvel=Quat(1,0,0,0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glLightfv(GL_LIGHT0,GL_POSITION,sun_pos);
  glRotatef(-camera.rot.vtx().y,0,1,0);
  glRotatef(-camera.rot.vtx().x,1,0,0);
  glRotatef(-camera.rot.vtx().z,0,0,1);
  glTranslatef(camera.pos.x,camera.pos.y,camera.pos.z);

  glPushMatrix();
  glEnable(GL_DEPTH_TEST);
  glBegin(GL_TRIANGLES);
  glVertex3f(0,0,0);
  glVertex3f(0,-1,0);
  glVertex3f(Rocket.thrust.x,Rocket.thrust.y,Rocket.thrust.z);
  glEnd();
  
  Quat r=Rocket.rot.getRotation();
  glRotatef(r.w,r.x,r.y,r.z);

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,0);
  glVertex3f(0,0,0);
  glTexCoord2f(1,0);
  glVertex3f(0,1.25,-1);
  glTexCoord2f(1,1);
  glVertex3f(0,0,4);
  glEnd();
  
  glColor3f(.7,.7,.7);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, rocket_dif);
  glScalef(1,.25,1);
  glutSolidCone(2.5,5,10,5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-Rocket.pos.x,-Rocket.pos.y,-Rocket.pos.z);
  glRotatef(Earth.rot.vtx().y,0,1,0);
  glRotatef(Earth.rot.vtx().x,1,0,0);
  glRotatef(Earth.rot.vtx().z,0,0,1);
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  glColor3f(.25,.3,1);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, earth_dif);
  //  glDisable(GL_DEPTH_TEST);
  //  glutSolidSphere(Earth.radius,36,18);
  gluSphere(earthMesh,Earth.radius,36,18);
  glPopMatrix();

  glutSwapBuffers();
}

void timer(int t)
{
  if(Rocket.altitude(Earth)>0) 
    Rocket.vel=Rocket.vel+(Rocket.pos.direction(Earth.pos)*Earth.gravity(Rocket));
  else {
    Rocket.vel=Vtx(0,0,0);
    Rocket.rvel=Quat();
  }
  Rocket.pos=Rocket.pos+Rocket.vel/24;
  Rocket.rot=Rocket.rot.normalize()*Rocket.rvel.normalize();
  Earth.rot=Earth.rot.normalize()*Earth.rvel.normalize();
  Rocket.setThrust(throttle);
  glutPostRedisplay();
  glutTimerFunc(1000/24,timer,1000/24);
}

void special(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_LEFT:
    Rocket.rvel=Rocket.rvel*Quat(.1,Vtx(0,0,1));
    break;
  case GLUT_KEY_RIGHT:
    Rocket.rvel=Rocket.rvel*Quat(-.1,Vtx(0,0,1));
    break;
  case GLUT_KEY_UP:
    Rocket.rvel=Rocket.rvel*Quat(-.1,Vtx(1,0,0));
    break;
  case GLUT_KEY_DOWN:
    Rocket.rvel=Rocket.rvel*Quat(.1,Vtx(1,0,0));
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
    cout << Rocket.altitude(Earth) << " " << Rocket.vel.magnitude() << " " << Rocket.pos.x << " " << Rocket.pos.y << " " << Rocket.pos.z << endl;
  default:
    break;
  }
}

int main(int argc, char **argv)
{
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
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
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
  initEarth();
  initRocket();
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  
  return 0;
}
