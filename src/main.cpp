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
GLUquadricObj *quadric;
double throttle=100; //holds the throttle setting for the rocket

GLfloat sun_pos[] = {1.0, 0.0, 0.0, 0.0};
GLfloat sun_amb[] = {0.1, 0.1, 0.1, 1.0};
GLfloat sun_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_spe[] = {1.0, 1.0, 1.0, 1.0};
GLfloat earth_dif[] = {1.0, 1.0, 1.0, 1.0};
GLfloat rocket_dif[] = {0.5, 0.5, 0.5, 1.0};
GLfloat ball_dif[] = {0.7, 0.7, 0.7, 1.0};
GLfloat ball_amb[] = {0.5, 0.5, 0.5, 1.0};

GLuint earthTexture=0,ballTexture=0, rocketTexture=0;

void quit()
{
  exit(0);
}

void loadTexture(const char *filename, GLuint &tex)
{
  int w=0, h=0;
  string comment;
  GLubyte *tD;
  ifstream infile;
  infile.open(filename);
  getline(infile, comment); //Magic Number (should always be P6
  getline(infile, comment); //comment (doesn't matter)
  infile >> w >> h;
  getline(infile, comment); //color depth (should always be 255)
  GLubyte *temp=new GLubyte[h*h*3];
  tD=new GLubyte[w*h*3];
  for(int line=0;line<h;line++) {
    for(int byte=w*3;byte>0;byte-=3) {
      tD[line*w*3+byte+2]=infile.get();
      tD[line*w*3+byte]=infile.get();
      tD[line*w*3+byte+1]=infile.get();
    }
  }
  infile.close();
  cout << w << "x" << h << endl;
  glGenTextures(1,&tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,
	       w, h, 
	       GL_RGB, GL_UNSIGNED_BYTE, tD);
  delete [] tD,temp;
}

void initEarth(void)
{
  loadTexture(DATADIR "earth.pnm", earthTexture);
  Earth.mass=5.9736E24;
  Earth.radius=6371E3;
  Earth.pos=Vtx(0,0,0);
  Earth.rot=Quat(23.5,Vtx(0,1,0))*Quat(90,Vtx(1,0,0)); //give the Earth it's tilt
  Earth.vel=Vtx(0,0,0);
  Earth.rvel=Quat(360/24/60/60/24,Vtx(0,0,1)); //start the Earth spinning
  
}

void initRocket(void)
{
  loadTexture(DATADIR "rocket.pnm", rocketTexture);
  loadTexture(DATADIR "8ball.pnm", ballTexture);
  Rocket.mass=1000;
  Rocket.radius=5;
  Rocket.pos=Vtx(Earth.radius+35786000,0,0);
  Rocket.rot=Quat(1,0,0,0);
  Rocket.vel=Vtx(0,0,110680*3); //TODO: find a good value FIXME: use a config
  Rocket.rvel=Quat(1,0,0,0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  Quat r=Rocket.rot.getRotation();

  glLoadIdentity();
  glTranslatef(0,-1.5,-2);
  glDisable(GL_TEXTURE_2D);
  //  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(2,-2,1);
  glVertex3f(1,-1,1);
  glVertex3f(-1,-2,1);
  glVertex3f(-1,-1,1);
  glVertex3f(-2,-2,1);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  //  glEnable(GL_CULL_FACE);
  glRotatef(50,1,0,0);
  glRotatef(-r.w,r.x,-r.z,r.y);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, ball_dif);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ball_amb);
  glBindTexture(GL_TEXTURE_2D, ballTexture);
  gluSphere(quadric,.25,36,18);

  glLoadIdentity();
  glRotatef(-camera.rot.vtx().y,0,1,0);
  glRotatef(-camera.rot.vtx().x,1,0,0);
  glRotatef(-camera.rot.vtx().z,0,0,1);
  glTranslatef(camera.pos.x,camera.pos.y,camera.pos.z);
  glLightfv(GL_LIGHT0,GL_POSITION,sun_pos);

  glPushMatrix();
  glEnable(GL_DEPTH_TEST);
  glBegin(GL_TRIANGLES);
  glVertex3f(0,0,0);
  glVertex3f(0,-1,0);
  glVertex3f(Rocket.thrust.x,Rocket.thrust.y,Rocket.thrust.z);
  glEnd();
  
  glRotatef(r.w,r.x,r.y,r.z);

  glBindTexture(GL_TEXTURE_2D, rocketTexture);  
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, rocket_dif);
  glScalef(1,.25,1);
  glRotatef(180,0,0,1);
  gluCylinder(quadric,2.5,0,5,10,5);
  glScalef(1,1,.25);
  gluSphere(quadric, 2.5, 10, 10);
  glScalef(0.01,1,3);
  glRotatef(-9,1,0,0);
  glTranslatef(0,-.8,0);
  gluCylinder(quadric,4,0,5,10,5);
  glPopMatrix();

  glPushMatrix();
  r=Earth.rot.getRotation();
  glTranslatef(-Rocket.pos.x,-Rocket.pos.y,-Rocket.pos.z);
  glRotatef(r.w,r.x,r.y,r.z);
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, earth_dif);
  gluSphere(quadric,Earth.radius,36,18);
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
  Rocket.rot=Rocket.rvel.normalize()*Rocket.rot.normalize();
  Earth.rot=Earth.rvel.normalize()*Earth.rot.normalize();
  Rocket.setThrust(throttle);
  glutPostRedisplay();
  glutTimerFunc(t,timer,t);
}

void special(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_LEFT:
    Rocket.rvel=Quat(-.1,Vtx(0,0,1))*Rocket.rvel;
    break;
  case GLUT_KEY_RIGHT:
    Rocket.rvel=Quat(.1,Vtx(0,0,1))*Rocket.rvel;
    break;
  case GLUT_KEY_UP:
    Rocket.rvel=Quat(.1,Vtx(1,0,0))*Rocket.rvel;
    break;
  case GLUT_KEY_DOWN:
    Rocket.rvel=Quat(-.1,Vtx(1,0,0))*Rocket.rvel;
    break;
  default:
    break;
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key){
  case 27:
    quit();
    break;
  case ',':
    Rocket.rvel=Quat(.1,Vtx(0,1,0))*Rocket.rvel;
    break;
  case '.':
    Rocket.rvel=Quat(-.1,Vtx(0,1,0))*Rocket.rvel;
    break;
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
  quadric=gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluQuadricTexture(quadric, GL_TRUE);
  gluQuadricNormals(quadric, GLU_SMOOTH);
  initEarth();
  initRocket();
  glutKeyboardFunc(keyboard);
    camera.rot.y-=2;
    camera.pos=Vtx(0,0,Rocket.radius*zoom*-1).rotate(-camera.rot.y,Y).rotate(camera.rot.x,X);
  glutMainLoop();
  
  return 0;
}
