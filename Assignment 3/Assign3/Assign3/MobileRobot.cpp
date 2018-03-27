#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include "glut.h"
#include <math.h>

#define TERR_D 75
#define TERR_W 750

float cam_xrot=15, cam_yrot=45, cam_zrot=0;
float xc, yc, zc;
float alpha = 0, angle_alpha = 0, base_angle = 0, angle1=50, upper_arm_angle = -90, needle_angle= 0, needle_pos = 5+10+10;


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
void drawPlatform();
void drawBase();
void drawWheel();
void drawArm();
void drawneedle();

void cleanup();
void camera();

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
	 glClearColor (0.0, 0.0, 0.0, 0.0);
	 initLight();
}

///////////////////////////////////////////////////////////
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera();

	drawTerrain();
	drawAxes();

	 
	//Write your code here

	glTranslatef(alpha, 2, 0);

	// rectangular platform, height = 2

	glPushMatrix();
		glScalef(2,.2,1); // heght = 2 = 10*.2, length = 10*2 = 20, width = 10*1 = 10
		glTranslatef(0,5,0); // to push it up, height is 10 for now
		drawPlatform();
	glPopMatrix();

	// 4 wheels

	glPushMatrix();
		glTranslatef(8, 1, 5);
		glRotatef(angle_alpha, 0.0, 0.0, 1.0);
		drawWheel();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(8, 1, -6);
		glRotatef(angle_alpha, 0.0, 0.0, 1.0);
		drawWheel();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-8, 1, 5);
		glRotatef(angle_alpha, 0.0, 0.0, 1.0);
		drawWheel();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-8, 1, -6);
		glRotatef(angle_alpha, 0.0, 0.0, 1.0);
		drawWheel();
	glPopMatrix();


	// circular base, height = 3

	glPushMatrix();
		
		glTranslatef(0, 5, 0);
		glRotatef(base_angle, 0.0, 1.0, 0.0); // base rotation
		glTranslatef(0, -5, 0);
		

		glPushMatrix();

			glTranslatef(0, 5, 0);
			glRotatef(angle1, 0, 0, 1); // this rotation will be apllied to arms and needle, but not the base
			glTranslatef(0, -5, 0);
		

			// lower arm
			glPushMatrix();
				glScalef(.1,1,.4); // height = 10 = 10*1, length = 10*.1 = 1, width = 10*.4 = 4
				glTranslatef(5,5+5,0); // 2+3 -> platform+base, height is 10, 5 for pushing it above
				drawArm();
			glPopMatrix();

			glTranslatef(0, 15, 0);
			glRotatef(upper_arm_angle, 0, 0, 1); // this rotation will be applied to upper arm and needle only
			glTranslatef(0, -15, 0);

			// upper arm
			glPushMatrix();
				glScalef(.1,1,.4); // height = 10 = 10*1, length = 10*.1 = 1, width = 10*.4 = 4
				glTranslatef(5,5+5+10,0); // 2+3 -> platform+base, height is 10, 15 for pushing it above lower arm
				drawArm();
			glPopMatrix();


			// needle
			glPushMatrix();
				
				glScalef(.1,1,.4); // height = 10 = 10*1, length = 10*.1 = 1, width = 10*.4 = 4
				glTranslatef(5,needle_pos,0); // 2+3 -> platform+base, height is 10, 15 for pushing it above upper arm
				glRotatef(needle_angle, 0, 1, 0);
				drawneedle();
			glPopMatrix();
		
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0, 3+2, 0);		
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			drawBase();
		glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
}

////////////////////////////////////////////////////////////
//Add more draw functions here

void drawPlatform(){

	GLfloat color[] = { 0.1,0.2,0.7 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glutSolidCube(10);
}

void drawBase(){
	
	GLfloat color[] = { .2,.5,.5 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	gluCylinder(gluNewQuadric(), 4, 4, 3, 32, 8);
	gluDisk(gluNewQuadric(), 0, 4, 32, 8);
}

void drawWheel(){
	
	GLfloat color[] = { 0,0,0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);

	gluCylinder(gluNewQuadric(), 2, 2, 1, 32, 8);
	
	glPushMatrix();
		glTranslatef(0,0,1);
		gluDisk(gluNewQuadric(), 0, 2, 32, 8);
		glTranslatef(0,0,-1);
		gluDisk(gluNewQuadric(), 0, 2, 32, 8);
	glPopMatrix();
	
	// small cubes for visibile motion

	glPushMatrix();
		glScalef(1,.5,1); 
		glTranslatef(0,0,1.5);
		glutSolidCube(1);
		glTranslatef(0,0,-1.5);
		glutSolidCube(1);
	glPopMatrix();
}

void drawArm(){
	
	GLfloat color[] = { 0.5,0.5,0.2 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glutSolidCube(10);
}

void drawneedle() {

	GLfloat color[] = { 0.3, 0.3, 0.3 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);

	glPushMatrix();

		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0, 0, -3);

		gluCylinder(gluNewQuadric(), .5, .5, 5, 32, 8);

		glColor3f(1,0,0);
		
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(gluNewQuadric(), .5,0, 2, 32, 8);
	glPopMatrix();

	
}


///////////////////////////////////////////////////////////
void drawTerrain(){
	 //carefully note the two line below. You are required to add these two lines
	 //at the begining of each of your draw function to associate material color
	 //to a specific component
	 GLfloat color[] = {0.2,0.8,0.2};
	 glMaterialfv(GL_FRONT, GL_AMBIENT, color);

	 glColor3f(0.2,0.8,0.2); // this line is not needed when lighting in enabled
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
	 switch (key) {
			case 'q':
			 cam_xrot += 1;
			 if (cam_xrot >360) cam_xrot -= 360;
				 break;
			case 'z':
				 cam_xrot -= 1;
			 if (cam_xrot < -360) cam_xrot += 360;
				 break;
			case 'a':
			 cam_yrot += 1;
			 if (cam_yrot >360) cam_yrot -= 360;
				 break;
			case 'd':
				 cam_yrot -= 1;
			 if (cam_yrot < -360) cam_yrot += 360;
				 break;
			case 'w':
			 cam_zrot += 1;
			 if (cam_zrot >360) cam_zrot -= 360;
				 break;
			case 'x':
				 cam_zrot -= 1;
			 if (cam_zrot < -360) cam_zrot += 360;
				 break;
			case 27:
				cleanup();
				exit(0);
				break;
			case 'f': // forward movement
				alpha+=.5;
				angle_alpha+=1;
				break;
			case 'F': // forward movement
				alpha-=.5;
				angle_alpha-=1;
				break;
			case 'p': // base rotation
				base_angle+=1;
				break;
			case 'P': // base rotation
				base_angle-=1;
				break;
			case 't': // whole arm movement
				angle1+=1;
				break;
			case 'T': // whole arm movement
				angle1-=1;
				break;
			case 'b': // half arm movement
				upper_arm_angle+=1;
				break;
			case 'B': // half arm movement
				upper_arm_angle-=1;
				break;
			case 'g': // needle rotation
				needle_angle-=10;
				break;
			case 'G': // needle rotation
				needle_angle+=10;
				break;
			case 'n': // needle movement
				needle_pos-=.1;
				if(needle_pos<20)
					needle_pos=20;
				break;
			case 'N': // needle movement
				needle_pos+=.1;
				if(needle_pos<20)
					needle_pos=20;
				break;
			
		default:
				 break;
	 }
	 glutPostRedisplay();
}


///////////////////////////////////////////////////////////
void cleanup() // call once when you exit program
{

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
	 glutDisplayFunc(display);
	 glutReshapeFunc(reshape);
	 glutKeyboardFunc(keyboard);
	 glutMainLoop();
	 return 0;
}
