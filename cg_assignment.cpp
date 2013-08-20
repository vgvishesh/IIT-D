#include<stdio.h>
#include<iostream>
#include<math.h>
#include<gl\glut.h>
#define ASPECT 4/3
#define PI 3.142857

using namespace std;

void init()
{
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120,ASPECT,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,5,5,0,0,0,0,1,0);
	glViewport(0,0,640,480);
}

void draw_cylinder(int n,float rad)
{
	float delta_theta=2*PI/n,theta;
	float *x1=new float[n];
	float *z1=new float[n];
	float y1=1,y2=0;
	glBegin(GL_QUAD_STRIP);
	for(int i=0;i<n;i++)
	{
		theta=i*delta_theta;
		x1[i]=rad*cos(theta);
		z1[i]=rad*sin(theta);

		glVertex3f(x1[i],y1,z1[i]);
		glVertex3f(x1[i],y2,z1[i]);

		cout<<x1<<"\t"<<z1<<"\n";
	}
	glVertex3f(rad,y1,0);
	glVertex3f(rad,y2,0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<n;i++)
			glVertex3f(x1[i],y1,z1[i]);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<n;i++)
			glVertex3f(x1[i],y2,z1[i]);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.5,0.3,0.1);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(5,0,0);
		glVertex3f(0,0,0);
		glVertex3f(0,5,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,5);
	glEnd();
	glColor3f(0.2,0.30,0.40);
	glPushMatrix();
		glScalef(1,1,1);
		draw_cylinder(100,1);
	glPopMatrix();

	glColor3f(0.9,0.30,0.40);
	glPushMatrix();
		glRotatef(90,1,0,0);
		glTranslatef(5,0,0);
		glScalef(1,1,1);
		draw_cylinder(100,1);
	glPopMatrix();
	glFlush();
}


int main(int agrc,char **agrv)
{
	glutInit(&agrc,agrv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //for animation programs GLUT_DOUBLE , GLUT_DEPTH for 3d apps
	glutInitWindowSize(640,480);			//screen resolution (width,height)
	glutCreateWindow("shader");
	glutInitWindowPosition(50,25);
	glutDisplayFunc(display);		//display function
	init();							//put all initialization commands in this funtion
	glutMainLoop();

	return 0;
}
