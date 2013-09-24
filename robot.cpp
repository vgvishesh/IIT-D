#include<iostream>
#include<gl\glew.h>
#include<GL\GLU.h>
#include <stdio.h>
#include<gl/glut.h>
#include<math.h>
#define PI 3.14159265

float h=0,LA=0,la=0,RA=0,ra=0,LL=0,ll=0,RR=0,rr=0,t=0.0;

using namespace std;
class robo;
class node;
void keyPress(unsigned char,int,int);
void display();

class shape
{
public:
	virtual void draw()
	{}
};

class cube : public shape
{
private:
	float length;
public:
	cube()
	{
		length=1;
	}
	void draw()
	{
		glutSolidCube(length);
	}
};

class sphere: public shape
{
private:
	float radius;
public:
	sphere()
	{
		radius=1;
	}
	void draw()
	{
		glutSolidSphere(radius,50,50);
	}
};

class Matrix
{
	friend class node;
private:
	float M[16];
public:
	Matrix()
	{
		Identity();
	}
	
	void printMatrix()
	{
		for(int i=0;i<16;i++)
		cout<<"\t"<<M[i];
	}

	void Identity()
	{
		for(int i=0;i<16;i++)
			M[i]=(i%5)==0?1:0;
	}

	void copyMatrix(Matrix temp)
	{
		for(int i=0;i<16;i++)
			M[i]=temp.M[i];
	}

	void Multiply(Matrix B)
	{
		Matrix temp;
		int s,k,j;
		for(int i=0;i<16;i++)
		{
			s=i/4;
			k=i%4;
			s=s*4;
			for(j=s;j<s+4;j++)
			{
				temp.M[i]+=M[k]*B.M[j];
				k+=4;
			}
		}
		copyMatrix(temp);
	}

	float& operator [](int n)
	{
		return M[n];
	}

	void operator =(Matrix temp)
	{
		for(int i=0;i<16;i++)
			M[i]=temp.M[i];
	}
};

class Entity
{
	friend class robo;
private:
	float r,g,b;
	Matrix localMatrix;
	shape *object;
public:
	Entity()
	{
		r=g=b=0.5;
		localMatrix.Identity();
	}	
	void SetColor(float rr,float gg,float bb)
	{
		this->r=rr;
		this->b=bb;
		this->g=gg;
	}

	void translate(float x,float y,float z)
	{
		Matrix matrix;
		matrix[12]+=x;
		matrix[13]+=y;
		matrix[14]+=z;

		//matrix.printMatrix();
		cout<<"\n";
		//localMatrix.printMatrix();
		localMatrix.Multiply(matrix);
		cout<<"\n";
		//printMatrix();
	}

	void rotate(float angle,float x,float y,float z)
	{
		Matrix matrix;
		matrix.Identity();
		float rad=angle * PI/180.0;
		if(x==0 && y==0 && z==1)
		{
			// rotation about z-axis
			matrix[0]=cos(rad);
			matrix[1]=sin(rad);
			matrix[4]=(-1)*sin(rad);
			matrix[5]=cos(rad);
		}
		else if(x==0 && y==1 && z==0)
		{
			// rotation about z-axis
			matrix[0]=cos(rad);
			matrix[8]=sin(rad);
			matrix[2]=(-1)*sin(rad);
			matrix[10]=cos(rad);
		}
		else if(x==1 && y==0 && z==0)
		{
			// rotation about z-axis
			matrix[5]=cos(rad);
			matrix[6]=sin(rad);
			matrix[9]=(-1)*sin(rad);
			matrix[10]=cos(rad);
		}
		localMatrix.Multiply(matrix);
	}

	void scale(float x,float y,float z)
	{
		localMatrix[0]*=x;
		localMatrix[5]*=y;
		localMatrix[10]*=z;
	}
};

class node
{
	friend class robo;
private:
	class Matrix parentMatrix;
protected:
	int num_child;
	Entity *entity;
	node **child;
public:
	node():parentMatrix()
	{
		num_child=0;
		entity=new Entity();
	}

	void translate(float x,float y,float z)
	{
		Matrix matrix;
		matrix[12]+=x;
		matrix[13]+=y;
		matrix[14]+=z;

		//matrix.printMatrix();
		cout<<"\n";
		//parentMatrix.printMatrix();
		parentMatrix.Multiply(matrix);
		cout<<"\n";
		//printMatrix();
	}

	void rotate(float angle,float x,float y,float z)
	{
		Matrix matrix;
		matrix.Identity();
		float rad=angle * PI/180.0;
		if(x==0 && y==0 && z==1)
		{
			// rotation about z-axis
			matrix[0]=cos(rad);
			matrix[1]=sin(rad);
			matrix[4]=(-1)*sin(rad);
			matrix[5]=cos(rad);
		}
		else if(x==0 && y==1 && z==0)
		{
			// rotation about y-axis
			matrix[0]=cos(rad);
			matrix[8]=sin(rad);
			matrix[2]=(-1)*sin(rad);
			matrix[10]=cos(rad);
		}
		else if(x==1 && y==0 && z==0)
		{
			// rotation about x-axis
			matrix[5]=cos(rad);
			matrix[6]=sin(rad);
			matrix[9]=(-1)*sin(rad);
			matrix[10]=cos(rad);
		}
		parentMatrix.Multiply(matrix);
	}

	void scale(float x,float y,float z)
	{
		parentMatrix[0]*=x;
		parentMatrix[5]*=y;
		parentMatrix[10]*=z;
	}

	void SetChildren(int n)
	{
		num_child=n;
		if(num_child!=0)
			child=new node*[num_child];
		/*
		for(int i=0;i<num_child;i++)
			child[i]->parentMatrix=this->parentMatrix;*/
	}

	void Load()
	{
		glMultMatrixf(parentMatrix.M);
	}
};

class robo
{
	friend void keyPress(unsigned char,int,int);
private:
	class node torso,head,
			LUA,RUA,LUL,LLL,
			RUL,RLL,LLA,RLA,
			*root;
public:
	robo():torso(),head(),LUA(),RUA(),LUL(),LLL(),RUL(),RLL(),LLA(),RLA()
	{
		torso.SetChildren(5);
		LUA.SetChildren(1);
		RUA.SetChildren(1);
		LUL.SetChildren(1);
		RUL.SetChildren(1);

		torso.child[0]=&head;
		torso.child[1]=&LUA;
		torso.child[2]=&RUA;
		torso.child[3]=&LUL;
		torso.child[4]=&RUL;

		LUA.child[0]=&LLA;
		RUA.child[0]=&RLA;
		LUL.child[0]=&LLL;
		RUL.child[0]=&RLL;

		torso.entity->SetColor(1,0,0);
		torso.entity->object=new cube();

		head.entity->SetColor(1,1,0);
		head.entity->object=new sphere();

		LUA.entity->SetColor(0,1,0);
		LUA.entity->object=new cube();

		RUA.entity->SetColor(0,1,0);
		RUA.entity->object=new cube();

		LLA.entity->SetColor(0,0.5,0);
		LLA.entity->object=new cube();

		RLA.entity->SetColor(0,0.5,0);
		RLA.entity->object=new cube();

		LUL.entity->SetColor(0,0,1);
		LUL.entity->object=new cube();

		RUL.entity->SetColor(0,0,1);
		RUL.entity->object=new cube();

		LLL.entity->SetColor(0,0,0.3);
		LLL.entity->object=new cube();

		RLL.entity->SetColor(0,0,0.3);
		RLL.entity->object=new cube();
		
		root=&torso;
	}
	
	void intiate_robo()
	{
		//glutSolidCube(1);	// torso

		// translations in global coordinate system
		float s=2;		//scaling factor
		torso.scale(s,s,s);

		head.translate(0,s/2 + (s*0.30) ,0);	// head 
		head.scale(0.3,0.3,0.3);		//0.3 of size of torso
		
		RUA.translate(s/2 + (s*0.3/2), s/2 - (s*0.60/2), 0); //right upper arm
		RUA.scale(0.3,0.6,0.3);

		RLA.translate((0.2/2.0),(-0.3 - 0.4)*s,0);		//right lower arm
		RLA.scale(0.2,0.5,0.2);

		LUA.translate((-1)*(s/2 + (s*0.3/2)), s/2 - (s*0.60/2), 0); //Left upper arm
		LUA.scale(0.3,0.6,0.3);

		LLA.translate((-1)*( 0.2/2.0),(-0.3 - 0.4)*s,0);		//Left lower arm
		LLA.scale(0.2,0.5,0.2);

		RUL.translate(0.2 + (s*0.3/2),(-1)*(s*1.5/2 + s/2),0);	//right upper leg
		RUL.scale(0.3,1.5,0.3);

		RLL.translate(0,(-1)*(1+0.3),0);		//right lower leg
		RLL.scale(0.4,0.4,0.4);

		LUL.translate((-1)*(0.2 + (s*0.3/2)),(-1)*(s*1.5/2 + s/2),0);	//left upper leg
		LUL.scale(0.3,1.5,0.3);

		LLL.translate(0,(-1)*(1.3),0);		//left lower leg
		LLL.scale(0.4,0.4,0.4);
	}

	void render_robo()
	{
		render_robo(&torso);
	}

	void render_robo(class node *bodyPart)
	{
		glPushMatrix();
		glColor3f(bodyPart->entity->r,bodyPart->entity->g,bodyPart->entity->b);
			bodyPart->Load();
			bodyPart->entity->object->draw();
			for(int i=0;i<bodyPart->num_child;i++)
				render_robo(bodyPart->child[i]);
		glPopMatrix();
	}
}R;

void keyPress(unsigned char key, int x,int y)
{
	switch(key)
	{
	case 'l':t+=5;
			R.torso.rotate(t,0,1,0);
			cout<<"\n"<<t;
			break;
	case 'k':t-=5;
			R.torso.rotate(t,0,1,0);
			cout<<"\n"<<t;
			break;
	case 'o':LA+=5;
			R.LUA.rotate(LA,1,0,0);
			cout<<"\n"<<LA;
			break;
	case 'i':LA-=5;
			R.LUA.rotate(LA,1,0,0);
			cout<<"\n"<<LA;
			break;
	case 'm':LL+=5;
			R.LUL.rotate(LL,1,0,0);
			cout<<"\n"<<LL;
			break;
	case 'n':LL-=5;
			R.LUL.rotate(LL,1,0,0);
			cout<<"\n"<<LL;
			break;
	}
	display();
}

void init()
{
	glClearColor(0.2,0.3,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,2,50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,15,0,0,0,0,1,0);
	glViewport(0,0,500,500);
	glEnable(GL_DEPTH_TEST);
	R.intiate_robo();
}

void display()
{
	glClearColor(0.4,0.3,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	R.render_robo();
	glFlush();
}

int main(int agrc,char **agrv)
{
	glutInit(&agrc,agrv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //for animation programs GLUT_DOUBLE , GLUT_DEPTH for 3d apps
	glutInitWindowSize(500,500);			//screen resolution (width,height)
	glutCreateWindow("Robo");
	glutKeyboardFunc(keyPress);
	glutInitWindowPosition(50,25);
	glutDisplayFunc(display);		//display function
	//glutIdleFunc(display);
	init();							//put all initialization commands in this funtion
	glutMainLoop();

	return 0;
}
