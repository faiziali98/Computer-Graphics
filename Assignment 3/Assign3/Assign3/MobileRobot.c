#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include "glut.h"
#include <math.h>

#define TERR_D 75
#define TERR_W 750

float cam_xrot=15, cam_yrot=45, cam_zrot=0;


GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

// Function Prototypes ////////////////////////////////////
void drawTerrain();
void drawAxes();
void drawBase();
void drawCircularBase();
void drawRect(float x, float y, float z);
void drawWheel();
void drawLowArm();
void drawUpArm();
void drawDrill();
void cleanup();
void camera();

// initial varaibles
float th = -45;
float th1 = 100;
float initt = 0;
float xd = 0;
float initx = 5;
float initb = 0;
float thd = 0;
/*  Initialize z-buffer, projection matrix, light source,
 *  and lighting model.  Do not specify a material property here.
 */
void initLight(void)
{
   GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
   GLfloat lmodel_ambient[] = { 0.4, 0.8, 0.4, 1.0 };
   GLfloat local_view[] = { 0.0 };

   glClearColor(0.0, 0.1, 0.1, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

//How to create cylinder in OpenGL
//http://www.opengl.org/discussion_boards/showthread.php/169815-Trying-to-create-a-glu-Cylinder

// Init ///////////////////////////////////////////////////
void init(void)
{
   glClearColor (0.0, 0.0, 0.0,0.0);
   initLight();
}

///////////////////////////////////////////////////////////
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   camera();
   drawTerrain();
   glPushMatrix();
	   glTranslatef(initx, 2, 5);
	   drawBase();
	   glPushMatrix();
			glTranslatef(2.5, 1, 10);
			glRotatef(initt, 0.0, 0.0, 1.0);
			drawWheel();
	   glPopMatrix();
	   glPushMatrix();
			glTranslatef(17.5, 1, 10);
			glRotatef(initt, 0.0, 0.0, 1.0);
			drawWheel();
	   glPopMatrix();
	   glPushMatrix();
			glTranslatef(2.5, 1, 0);
			glRotatef(initt, 0.0, 0.0, 1.0);
			glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		   drawWheel();
	   glPopMatrix();
	   glPushMatrix();
			glTranslatef(17.5, 1, 0);
			glRotatef(initt, 0.0, 0.0, 1.0);
			glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		   drawWheel();
	   glPopMatrix();
	   glPushMatrix();
		   glTranslatef(10, 5.5, 5);
		   glRotatef(initb, 0, 1, 0);
		   glTranslatef(-10, -5.5, -5);
		   glPushMatrix();
			    glPushMatrix();
					glTranslatef(10, 5, 3);
					glRotatef(th, 0, 0, 1);
					glTranslatef(-10, -5, -3);
					glPushMatrix();
						glTranslatef(9.5, 5, 3);
						glRotatef(0, 0, 0, 1);
						drawLowArm();
					glPopMatrix();
					glPushMatrix();
						glTranslatef(10, 15, 3);
						glRotatef(th1, 0, 0, 1);
						glTranslatef(-10, -15, -3);
						glPushMatrix();
							glTranslatef(9.5, 15, 3);
							drawUpArm();
							glTranslatef(0.5, 13, 2);
							drawDrill();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
				glTranslatef(10, 5, 5);
				drawCircularBase();
		   glPopMatrix();
	   glPopMatrix();
   glPopMatrix();
   //Write your code here

   glutSwapBuffers();
}

////////////////////////////////////////////////////////////
//Add more draw functions here
void drawDrill() {
	GLfloat color[] = { 0.2, 0.2, 0.2 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	GLUquadric* quadric = gluNewQuadric();
	glPushMatrix();
		glRotatef(thd, 0.0f, 1.0f, 0.0f);
		glTranslatef(0, xd, 0);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0, 0, -3);
		gluCylinder(quadric, 0.5, 0.5, 10, 32, 8);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.5,0, 2, 32, 8);
	glPopMatrix();
}
void drawWheel() {
	GLfloat color[] = { 0.1, 0.1, 0.1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	GLUquadric* quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 2.5, 32, 8);
	glPushMatrix();
		glTranslatef(0, 0, 1);
		gluDisk(quadric, 0.0f, 2.5, 32, 8);
	glPopMatrix();
	gluCylinder(quadric, 2.5, 2.5, 1, 32, 8);
	glTranslatef(-1, -0.5, 1);
	drawRect(2,1,0.5);
}
void drawRect(float x, float y, float z) {
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, z);
		glVertex3f(x, 0, z);
		glVertex3f(x, y, z);
		glVertex3f(0, y, z);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(x, 0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(0, y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(0, y, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x, y, z);
		glVertex3f(0, y, z);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(x, 0, 0);
		glVertex3f(x, 0, z);
		glVertex3f(0, 0, z);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, y, 0);
		glVertex3f(0, y, z);
		glVertex3f(0, 0, z);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(x, 0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x, y, z);
		glVertex3f(x, 0, z);
	glEnd();
}
void drawBase() {
	GLfloat color[] = { 0.5,0.3,0.4 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glPushMatrix();
		drawRect(20, 2, 10);
	glPopMatrix();
}

void drawCircularBase() {
	GLfloat color[] = { 0.2,0.4,0.6 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	GLUquadric* quadric = gluNewQuadric();
	glPushMatrix();
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		gluDisk(quadric, 0.0f, 4, 32, 8);
		gluCylinder(quadric, 4, 4, 3, 32, 8);
	glPopMatrix();
}

void drawLowArm() {
	GLfloat color[] = { 0.2,0.8,0.2 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glPushMatrix();
		drawRect(1, 10, 4);
	glPopMatrix();
}

void drawUpArm() {
	GLfloat color[] = { 0.2,0.8,0.2 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glPushMatrix();
		drawRect(1, 10, 4);
	glPopMatrix();
}
///////////////////////////////////////////////////////////
void drawTerrain(){
   //carefully note the two line below. You are required to add these two lines
   //at the begining of each of your draw function to associate material color
   //to a specific component
   GLfloat color[] = {0.2,0.8,0.2};
   glMaterialfv(GL_FRONT, GL_AMBIENT, color);
   glPushMatrix();
	   glTranslatef (-TERR_W/2, 0.0, -TERR_D/2);
	   glBegin(GL_POLYGON);
		   glVertex3f(0,0,0);
		   glVertex3f(TERR_W,0,0);
		   glVertex3f(TERR_W,0,TERR_D);
		   glVertex3f(0,0,TERR_D);
		   glVertex3f(0,0,0);
	   glEnd();
   glPopMatrix();
}

///////////////////////////////////////////////////////////
void drawAxes(){
   glColor3d(1,0,0);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(3,0,0);
   glEnd();

   glColor3d(0,1,0);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(0,3,0);
   glEnd();

   glColor3d(0,0,1);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(0,0,3);
   glEnd();
}

///////////////////////////////////////////////////////////
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 120.0);
   glMatrixMode(GL_MODELVIEW);
   camera();
}

///////////////////////////////////////////////////////////
void camera(){
   glLoadIdentity();
   glTranslatef (0.0, 0.0,-55.0);
   glRotatef(cam_xrot,1,0,0);
   glRotatef(cam_yrot,0,1,0);
   glRotatef(cam_zrot,0,0,1);
}

///////////////////////////////////////////////////////////
void keyboard (unsigned char key, int x, int y)
{
   // Camera controls - Rotation along principle axis
	int mod = glutGetModifiers();
	if (mod==0) {
		switch (key) {
		case 'q':
			cam_xrot += 1;
			if (cam_xrot > 360) cam_xrot -= 360;
			break;
		case 'z':
			cam_xrot -= 1;
			if (cam_xrot < -360) cam_xrot += 360;
			break;
		case 'a':
			cam_yrot += 1;
			if (cam_yrot > 360) cam_yrot -= 360;
			break;
		case 'd':
			cam_yrot -= 1;
			if (cam_yrot < -360) cam_yrot += 360;
			break;
		case 'w':
			cam_zrot += 1;
			if (cam_zrot > 360) cam_zrot -= 360;
			break;
		case 'x':
			cam_zrot -= 1;
			if (cam_zrot < -360) cam_zrot += 360;
			break;
		case 27:
			cleanup();
			exit(0);
			break;
		case 'n':
			initx += 0.5;
			initt -= 10;
			break;
		
		case 'h':
			initb++;
			break;
		case 'j':
			th++;
			break;
		case 'g':
			th1++;
			break;
		case 'k':
			xd = xd + 0.3;
			if (xd > 0) xd = 0;
			break;
		case 'm':
			thd++;
			break;
		default:
			break;
		}
		
	}
	else if (mod == 1) {
		switch (key) {
			case 'N':
				initx -= 0.5;
				initt += 10;
				break;
			case 'H':
				initb--;
				break;
			case 'J':
				th--;
				break;
			case 'G':
				th1--;
				break;
			case 'K':
				xd = xd - 0.3;
				if (xd < -6) xd = -6;
				break;
		}
	}
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////
void cleanup() // call once when you exit program
{

}
void idle() {
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutIdleFunc(idle);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   
   glutMainLoop();
   return 0;
}

