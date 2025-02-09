#include <freeglut.h>
#include <iostream>
#include <stack>
#include <utility>
#include <string>
#include <math.h>
#include <cmath>
#include <array>

// Initialize Value of PIE for DEGREE to RADIAN Conversion
const float PI = 3.14159265358979323846;

using namespace std;

// Initializing values of Triangle Verices
float X1=0,X2=0,X3=0;
float Y1=0,Y2=0,Y3=0;

// Initializing Values of Window Sizzes
float windowSizeX=400;
float windowSizeY=400;

// Initializing Variables
int clickCount=0;					//For keeping count of mouse clicks
int currentScene=0;					//For keeping record of displays

// Tranformation Factors
float tx=0,ty=0;					//Translation
float angle=0;
float theta=angle*(PI/180);			//Rotation
float Sx=0,Sy=0;					//Scaling
float SHx=0,SHy=0;					//Shearing

// Transformation Matrices
float Tr[3][3]={1	,0	 ,tx,						//Translation Transformation Matrix
				0	,1	 ,ty,
				0	,0	 ,1};

float Rot[3][3]={cos(theta)	,-sin(theta) ,0,		//Rotation Transformation Matrix
				 sin(theta)	,cos(theta)	 ,0,
				 0			,0			 ,1};

float Sc[3][3]={Sx	,0	 ,0,						//Scaling Transformation Matrix
				0	,Sy	 ,0,
				0	,0	 ,1};

float RefX[3][3]={1	 ,0	 ,0,						//Reflection (About X-Axis) Transformation Matrix
				  0	 ,-1 ,0,
				  0	 ,0	 ,1};

float RefY[3][3]={-1 ,0	 ,0,						//Reflection (About Y-Axis) Transformation Matrix
				  0	 ,1  ,0,
				  0	 ,0	 ,1};

float Shear[3][3]={1	,SHx ,0,					//Shearing Transformation Matrix
				   SHy	,1	 ,0,
				   0	,0	 ,1};

// ORIGINAL Position Matrix
float originalP[3][3]= {X1, X2, X3,  // X-coordinates
						Y1, Y2, Y3,  // Y-coordinates
						1 , 1 , 1 }; // Homogeneonus Coordinates


// Matrix Multiplication Method
void MatMultiply(float T[3][3], float P[3][3])
{
	float A[3][3]={0,0,0,
				   0,0,0,
				   0,0,0};

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				A[i][j]+=T[i][k]*P[k][j];
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for(int j=0; j < 3; j++)
		{
			P[i][j]=A[i][j];
		}
	}
}


// View Position Matrix in terminal with the help of this method
void displayMat()
{
	cout<<"P = ";
	for (int i = 0; i < 3; i++)
	{
		for(int j=0; j < 3; j++)
		{
			cout<<originalP[i][j]<<" ";
		}
		cout<<endl;
		cout<<"    ";
	}
	cout<<endl;
}

// Big Text Rendering Method
void displayMethodName(char shapeName[], int PosX, int PosY)
{
    glRasterPos2f(PosX,PosY);
    void * font1 = GLUT_BITMAP_TIMES_ROMAN_24;
    for (int i = 0; i<strlen(shapeName);i++)
    {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// Small Text Rendering Method
void displaySentence(char shapeName[], int PosX, int PosY)
{
    glRasterPos2f(PosX,PosY);
    void * font1 = GLUT_BITMAP_HELVETICA_12;
    for (int i = 0; i<strlen(shapeName);i++)
    {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// Circle Drawing Function
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the circle
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * PI * i / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

// Information Button Drawing Method
void drawInfoButton() 
{
    glColor3f(0, 0, 0);
    drawCircle(370, 380, 5);

    glBegin(GL_QUADS);
        glVertex2f(367, 345);
        glVertex2f(367, 370);
        glVertex2f(373, 370);
        glVertex2f(373, 345);
    glEnd();
}

// Main Display Function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Drawing i Button
	glColor3f(0.75,0.75,0.75);
	drawCircle(370,365,25);
	drawInfoButton();

	// Drawing AXES
	glColor3f(1,1,1);
	glBegin(GL_LINES);
		glVertex2f(0, windowSizeY);
		glVertex2f(0, -windowSizeY);
		glVertex2f(-windowSizeX, 0);
		glVertex2f(windowSizeX, 0);
    glEnd();

	// Rendering Text Using Bitmap
	glColor3f(0.0, 1.0, 1.0);
    char Name1[] = "2D-TRANSFORMATIONS";
    displayMethodName(Name1, -300,375);

	glColor3f(0.0, 1.0, 1.0);
    char Name2[] = "Made By :- Aryan Dubey 500109841";
    displaySentence(Name2, 10,380);

	// Rendering First Point of Triangle
	if (clickCount == 1) 
    {
        glColor3f(1.0, 1.0, 0.0);
		drawCircle(originalP[0][0], originalP[1][0],2);
    }

	// Rendering First & Second Point of Triangle
	if (clickCount == 2) 
    {
        glColor3f(1.0, 1.0, 0.0);
		drawCircle(originalP[0][0], originalP[1][0],2);
        drawCircle(originalP[0][1], originalP[1][1],2);
    }

	// Rendering Whole Triangle
    if (clickCount >= 3) 
    {
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
			glVertex2f(originalP[0][0], originalP[1][0]);		//x1,y1
			glVertex2f(originalP[0][1], originalP[1][1]);		//x2,y2
			glVertex2f(originalP[0][2], originalP[1][2]);		//x3,y3
        glEnd();
    }

	glFlush();
}

// Information Display Function
void drawNewWindow()
{
    glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0,1,1);
	char rule[] = "RULES FOR IMPLEMENTATION";
	displayMethodName(rule, -150, 230);

	glColor3f(1,0,0);
	char back[] = "BACK";
	displaySentence(back, 350, 350);

    glColor3f(0,1,0);
	
    char rule1[] = "1) The First 3 Left clicks record the Coordinates of the Triangle.";
    char rule2[] = "2) Press 'R' to Rotate the Triangle About its Centroid.";
    char rule3[] = "3) Press 'T' to Translate the Triangle.";
    char rule4[] = "4) Press 'S' to Scale the Triangle";
    char rule5[] = "5) Press 'H' to Shear the Triangle";
    char rule6[] = "6) Press 'X' to Reflect the Triangle About X-Axis";
    char rule7[] = "7) Press 'Y' to Reflect the Triangle About Y-Axis";
    char rule8[] = "8) Make sure to give the Transformation Factors in the terminal.";
	char rule9[] = "9) Click Right Mouse Button (RMB) to Clear the screen";
    char rule10[] = "10) Press 'I' or 'i button' For Info";
    char rule11[] = "11) Press 'I' Again or 'back button' To go back";
    char rule12[] = "12) Press Escape Button to Exit";

	displaySentence(rule1, -150, 150);
	displaySentence(rule2, -150, 135);
	displaySentence(rule3, -150, 120);
	displaySentence(rule4, -150, 105);
	displaySentence(rule5, -150, 90);
	displaySentence(rule6, -150, 75);
	displaySentence(rule7, -150, 60);
	displaySentence(rule8, -150, 45);
	displaySentence(rule9, -150, 30);
	displaySentence(rule10, -150, 15);
	displaySentence(rule11, -150, 0);
	displaySentence(rule12, -150, -15);

    
    glFlush();
}

// Function to shift between Displays
void display1()
{
	if (currentScene==0)
    {
        display();
    }
    else if (currentScene==1)
    {
        drawNewWindow();
    }
}

//MouseClick Function to add functionality to MouseClicks
void mouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float X = (float)x - windowSizeX;
        float Y = windowSizeY - (float)y;

		// For Back Button 
		if (currentScene==1)
		{
			if (X>340 && X<370 && Y>335 && Y<365)
			{
				currentScene--;
			}
			return;
		}

		// For i Button 
		if (X>350 && X<390 && Y>340 && Y<390)
		{
			currentScene++;
		}

        //cout<<"X = "<<X<<endl;
        //cout<<"Y = "<<Y<<endl;

		else if (clickCount == 0) {
            X1 = X;
            Y1 = Y;
            clickCount++;
			cout<<"1st Coordinate Captured !!"<<endl;
        } else if (clickCount == 1) {
            X2 = X;
            Y2 = Y;
            clickCount++;
			cout<<"2nd Coordinate Captured !!"<<endl;
        } else if (clickCount == 2) {
            X3 = X;
            Y3 = Y;
            clickCount++;
			cout<<"3rd Coordinate Captured !!"<<endl;
			cout<<endl;

			displayMat();
        }

		// Assigning New Captured Values to the ORIGINAL Position Matrix

		originalP[0][0] = X1;
		originalP[1][0] = Y1;

        originalP[0][1] = X2; 
		originalP[1][1] = Y2;
		
		originalP[0][2] = X3;
		originalP[1][2] = Y3;
		
        glutPostRedisplay();
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && currentScene==0)
    {
        clickCount=0;
        glClear(GL_COLOR_BUFFER_BIT);		//This will Clear the Transformation Screen
        glFlush();
        glutPostRedisplay();
		cout<<"Right Mouse Button Pressed !!"<<endl;
		cout<<"The Screen is Refreshed."<<endl;
		cout<<endl;
	}
}

//Keyboard function to add functionality to Keyboard Keys
void keyboard(unsigned char key, int x, int y)
{
	//ROTATION (About the Centroid of Triangle)
	if (key=='r'||key=='R')
	{
		cout<<endl;
		cout<<"Enter Angle (in DEGREES) at which you want to Rotate the shape : ";
		cin>>angle;
		cout<<endl;
		
		theta=angle*(PI/180);			//Conversion from Degree to Radian

		Rot[0][0] = cos(theta);
		Rot[0][1] = -sin(theta);
		Rot[1][0] = sin(theta);
		Rot[1][1] = cos(theta);

		Tr[0][2]=-((X1+X2+X3)/3);
		Tr[1][2]=-((Y1+Y2+Y3)/3);

		MatMultiply(Tr,originalP);

		MatMultiply(Rot,originalP);

		Tr[0][2]=((X1+X2+X3)/3);
		Tr[1][2]=((Y1+Y2+Y3)/3);

		MatMultiply(Tr,originalP);
		cout<<endl;
		cout<<"ROTATED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

	//TRANSLATION
	else if(key=='t'||key=='T')
	{
		cout<<endl;
		cout<<"Enter Tralastion in x = ";
		cin>>tx;
		cout<<endl;
		cout<<"Enter Tralastion in y = ";
		cin>>ty;
		cout<<endl;

		Tr[0][2]=tx;
		Tr[1][2]=ty;

		MatMultiply(Tr,originalP);
		cout<<endl;
		cout<<"TRANSLATED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

	//REFLECTION
	else if(key=='x'||key=='X')
	{
		cout<<"Shape Reflected About X-Axis !!!";
		cout<<endl;
		MatMultiply(RefX,originalP);
		cout<<endl;
		cout<<"REFLECTED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}
	else if(key=='y'||key=='Y')
	{
		cout<<"Shape Reflected About Y-Axis !!!";
		cout<<endl;
		MatMultiply(RefY,originalP);
		cout<<endl;
		cout<<"REFLECTED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

	//SCALING
	else if(key=='s'||key=='S')
	{
		char yesNo;
		cout<<endl;
		cout<<"Do you want to scale the shape differently in both directions ? (y/n) : ";
		cin>>yesNo;
		cout<<endl;

		if (yesNo=='y')
		{
			cout<<"The Shapes will be Scaled differently across both directions";
			cout<<endl;
			cout<<endl;
			cout<<"Enter Scaling Factor to Scale the shape in x-axis (Sx) : ";
			cin>>Sx;
			cout<<endl;

			cout<<"Enter Scaling Factor to Scale the shape in y-axis (Sy) : ";
			cin>>Sy;
			cout<<endl;
		}
		else if (yesNo=='n')
		{
			cout<<"The Shapes will be Scaled similarly across both directions";
			cout<<endl;
			cout<<endl;
			cout<<"Enter Scaling Factor to Scale the shape (S) : ";
			cin>>Sx;
			Sy=Sx;
			cout<<endl;
		}

		Sc[0][0]=Sx;
		Sc[1][1]=Sy;

		MatMultiply(Sc,originalP);
		cout<<endl;
		cout<<"SCALED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}
	
	//SHEARING
	else if(key=='h'||key=='H')
	{
		char yesNo;
		cout<<endl;
		cout<<"Do you want to shear the shape differently in both directions ? (y/n) : ";
		cin>>yesNo;
		cout<<endl;

		if (yesNo=='y')
		{
			cout<<"The Shapes will be Sheared differently across both directions";
			cout<<endl;
			cout<<endl;
			cout<<"Enter Shearing Factor to Shear the shape in x-axis (SHx) : ";
			cin>>SHx;
			cout<<endl;

			cout<<"Enter Shearing Factor to Shear the shape in y-axis (SHy) : ";
			cin>>SHy;
			cout<<endl;
		}
		else if (yesNo=='n')
		{
			cout<<"The Shapes will be Sheared similarly across both directions";
			cout<<endl;
			cout<<endl;
			cout<<"Enter Shearing Factor to Scale the shape (SH) : ";
			cin>>SHx;
			SHy=SHx;
			cout<<endl;
		}

		Shear[0][1]=SHx;
		Shear[1][0]=SHy;

		MatMultiply(Shear,originalP);
		cout<<endl;
		cout<<"SHEARED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}
	
	//EXIT (Escape Key Code == 27)
	else if(key==27)
	{
		exit(0);
	}

	//INFO
	else if(key=='i'||key=='I')
	{
		if (currentScene==0)
		{
			currentScene++;
		}
		else
		{
			currentScene--;
		}
		glutPostRedisplay();
	}

	//Assigning the updated values to the coordinates
	X1 = originalP[0][0]; Y1 = originalP[1][0];
    X2 = originalP[0][1]; Y2 = originalP[1][1];
    X3 = originalP[0][2]; Y3 = originalP[1][2];
	
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(2*windowSizeX, 2*windowSizeY);
	glutInitWindowPosition(700,0);
    glutCreateWindow("2D - TRANSFORMATIONS");
    glutDisplayFunc(display1);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboard);
    init();

	glutMainLoop();
    return 0;
}