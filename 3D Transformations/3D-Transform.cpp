#include <freeglut.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <math.h>
#include <iomanip>


using namespace std;

float windowSizeX=400;
float windowSizeY=400;

const float PI = (float)22/7;

int clickCount=0;
int currentScene=0;

bool shapeMenu=false;
int selectedShape=0;

float eyeX = 0.0f, eyeY = 5.0f, eyeZ = 10.0f;  // Camera position (initially at (0, 0, 5))

float upX = 0.0f, upY = 1.0f, upZ = 0.0f;  // Camera up vector
// Movement speed
float moveSpeed = 0.5f;
float lookSpeed = 0.05f;

float Ex=0,Ey=0,Ez=0;

// Tranformation Factors
float tx=0,ty=0,tz=0;					//Translation
float angle=0;
float theta=angle*(PI/180.0f);			    //Rotation
float Sx=0,Sy=0,Sz=0;					//Scaling
float SHxy=0,SHyz=0,SHxz=0;				//Shearing
float SHyx=0,SHzy=0,SHzx=0;				//Shearing

// Transformation Matrices
float Tr[4][4]={1	,0	 ,0  , 0,						        //Translation Transformation Matrix
				0	,1	 ,0  , 0,
				0	,0	 ,1  , 0,
                tx  ,ty  ,tz , 1 };

float RotX[4][4]={1	 ,0			 ,0             ,0,
                  0  ,cos(theta) ,sin(theta)   ,0, 		        //Rotation in X-Axis Transformation Matrix
				  0  ,-sin(theta) ,cos(theta)	,0,
                  0  ,0          ,0             ,1};

float RotY[4][4]={cos(theta) ,0     ,-sin(theta) ,0,
                  0          ,1     ,0           ,0, 		    //Rotation in Y-Axis Transformation Matrix
				  sin(theta),0     ,cos(theta)	,0,
                  0          ,0     ,0           ,1};

float RotZ[4][4]={cos(theta)	,sin(theta) ,0     ,0, 		    //Rotation in Z-Axis Transformation Matrix
				 -sin(theta)	    ,cos(theta)	 ,0     ,0,
				 0			    ,0			 ,1     ,0,
                 0              ,0           ,0     ,1};

float Sc[4][4]={Sx	,0	 ,0 ,0,						            //Scaling Transformation Matrix
				0	,Sy	 ,0 ,0,
				0	,0	 ,Sz,0,
                0   ,0   ,0 ,1};

float RefXY[4][4]={1  ,0	 ,0 ,0,						        //Reflection (About XY-Plane) Transformation Matrix
				  0	 ,1  ,0 ,0,
				  0	 ,0	 ,-1 ,0,
                  0  ,0  ,0 ,1};

float RefYZ[4][4]={-1  ,0	,0 ,0,						        //Reflection (About YZ-Plane) Transformation Matrix
				  0	 ,1 ,0 ,0,
				  0	 ,0	,1 ,0,
                  0  ,0 ,0 ,1};

float RefZX[4][4]={1  ,0	,0  ,0,						        //Reflection (About XZ-Plane) Transformation Matrix
				  0	 ,-1 ,0  ,0,
				  0	 ,0	,1  ,0,
                  0  ,0 ,0  ,1};

float Shear[4][4]={1	,SHxy   ,SHxz  ,0,					    //Shearing Transformation Matrix
				   SHyx	,1	    ,SHyz  ,0,
				   SHzx	,SHzy   ,1  ,0,
                   0    ,0      ,0  ,1};

float verticesOfPyramid[4][4] = {
    // Front face
     0.0f,  1.0f,  0.0f, 1.0f, // Vertex Top
     -1.0f, 0.0f,  1.0f, 1.0f, // Vertex Left
     1.0f,  0.0f,  1.0f, 1.0f, // Vertex Right
     0.0f,  0.0f,  -1.0f, 1.0f, // Vertex Behind
};

float centroidX = (verticesOfPyramid[0][0] + verticesOfPyramid[1][0] + verticesOfPyramid[2][0] + verticesOfPyramid[3][0]) / 4;
float centroidY = (verticesOfPyramid[0][1] + verticesOfPyramid[1][1] + verticesOfPyramid[2][1] + verticesOfPyramid[3][1]) / 4;
float centroidZ = (verticesOfPyramid[0][2] + verticesOfPyramid[1][2] + verticesOfPyramid[2][2] + verticesOfPyramid[3][2]) / 4;

float centerX = centroidX, centerY = centroidY, centerZ = centroidZ;  // Camera looks at the Centroid

float verticesOfCube[8][4] = {
    // Front top-left
    {-1.0f,  1.0f,  1.0f, 1.0f},
    // Front top-right
    { 1.0f,  1.0f,  1.0f, 1.0f},
    // Front bottom-left
    {-1.0f, -1.0f,  1.0f, 1.0f},
    // Front bottom-right
    { 1.0f, -1.0f,  1.0f, 1.0f},
    // Back top-left
    {-1.0f,  1.0f, -1.0f, 1.0f},
    // Back top-right
    { 1.0f,  1.0f, -1.0f, 1.0f},
    // Back bottom-left
    {-1.0f, -1.0f, -1.0f, 1.0f},
    // Back bottom-right
    { 1.0f, -1.0f, -1.0f, 1.0f}
};


void displayMat()
{
	cout<<"P = ";
	for (int i = 0; i < 4; i++)
	{
		for(int j=0; j < 4; j++)
		{
			cout<<verticesOfPyramid[i][j]<<" ";
		}
		cout<<endl;
		cout<<"    ";
	}
	cout<<endl;
}

void displayMat8x4()
{
	cout<<"P = ";
	for (int i = 0; i < 8; i++)
	{
		for(int j=0; j < 4; j++)
		{
			cout<<verticesOfCube[i][j]<<" ";
		}
		cout<<endl;
		cout<<"    ";
	}
	cout<<endl;
}

void MatMultiply8x4(float T[4][4], GLfloat P[8][4]) {
    for (int i = 0; i < 8; i++) {
        GLfloat temp[4] = {0};
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                temp[j] += T[j][k] * P[i][k];
            }
        }
        for (int j = 0; j < 4; j++) {
            P[i][j] = temp[j];
        }
    }
}

void MatMultiply(float T[4][4], float P[4][4]) 
{
    float A[4][4] = { 0 };
    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
    A[i][j] = 0; // Initialize to 0
    for (int k = 0; k < 4; k++) 
    {
       A[i][j] += P[i][k] * T[k][j];
    }
}
}

    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
        P[i][j] = A[i][j];
    }
}
}

void init() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(-windowSizeX,windowSizeX,-windowSizeY,windowSizeY);
}

void init1() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(-windowSizeX,windowSizeX,-windowSizeY,windowSizeY);
}

void init2() {
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(-windowSizeX,windowSizeX,-windowSizeY,windowSizeY);
}   


void updateCamera() 
{
    /*centroidX = (verticesOfPyramid[0][0] + verticesOfPyramid[1][0] + verticesOfPyramid[2][0] + verticesOfPyramid[3][0]) / 4;
    centroidY = (verticesOfPyramid[0][1] + verticesOfPyramid[1][1] + verticesOfPyramid[2][1] + verticesOfPyramid[3][1]) / 4;
    centroidZ = (verticesOfPyramid[0][2] + verticesOfPyramid[1][2] + verticesOfPyramid[2][2] + verticesOfPyramid[3][2]) / 4;

    centerX = centroidX;
    centerY = centroidY; 
    centerZ = centroidZ;  // Camera looks at the Centroid
    */
    

    gluLookAt(eyeX, eyeY, eyeZ,   // Camera position
              centerX, centerY, centerZ,  // Camera target (look-at point)
              upX, upY, upZ);  // Up direction
}

// Big Text Rendering Method
void displayMethodName(char shapeName[], int PosX, int PosY)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, 0, 50); // 2D projection for text rendering


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glRasterPos2f(PosX, PosY);  // Adjust text position
    void *font1 = GLUT_BITMAP_TIMES_ROMAN_24;
    for (int i = 0; i < strlen(shapeName); i++) {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void displayMethodName1(char shapeName[], int PosX, int PosY)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY); // 2D projection for text rendering


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glRasterPos2f(PosX, PosY);  // Adjust text position
    void *font1 = GLUT_BITMAP_TIMES_ROMAN_24;
    for (int i = 0; i < strlen(shapeName); i++) {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Small Text Rendering Method
void displaySentence(char shapeName[], int PosX, int PosY)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, -400, 400); // 2D projection for text rendering


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glRasterPos2f(PosX, PosY);  // Adjust text position
    void *font1 = GLUT_BITMAP_HELVETICA_12;
    for (int i = 0; i < strlen(shapeName); i++) {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void displayAxes()
{
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(-windowSizeX,0,0);
        glVertex3f(windowSizeX,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,-windowSizeY,0);
        glVertex3f(0,windowSizeY,0);

        glColor3f(0,0.5,1);
        glVertex3f(0,0,-400);
        glVertex3f(0,0,400);
     glEnd();
}

void displayCamLocation()
{
    char EyeLoc[20];  // Make sure it's large enough to hold the integer string
    snprintf(EyeLoc, sizeof(EyeLoc), "Eye : (%d,%d,%d)",(int)Ex,(int)Ey,(int)Ez);

    // Ensure text is rendered in 2D space
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, 0, 50); // 2D projection for text rendering

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glRasterPos2f(-350, 10);  // Adjust text position
    void *font1 = GLUT_BITMAP_HELVETICA_12;
    for (int i = 0; i < strlen(EyeLoc); i++) {
        char c1 = EyeLoc[i];
        glutBitmapCharacter(font1, c1);
    }

    char PosXAxis[]="X-Axis Keys : Same";
    char NegXAxis[]="X-Axis Keys : Opposite";

    if (Ez>=0)
    {
        glRasterPos2f(-40, 10);  // Adjust text position
        void *font1 = GLUT_BITMAP_HELVETICA_12;
        for (int i = 0; i < strlen(PosXAxis); i++) {
        char c1 = PosXAxis[i];
        glutBitmapCharacter(font1, c1);
    }
    }
    else
    {
        glRasterPos2f(-40, 10);  // Adjust text position
        void *font1 = GLUT_BITMAP_HELVETICA_12;
        for (int i = 0; i < strlen(NegXAxis); i++) {
        char c1 = NegXAxis[i];
        glutBitmapCharacter(font1, c1);
    }
    }

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void displayOptions(char shapeName[], int PosX, int PosY)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, 0, 50); // 2D projection for text rendering


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glRasterPos2f(PosX, PosY);  // Adjust text position
    void *font1 = GLUT_BITMAP_HELVETICA_12;
    for (int i = 0; i < strlen(shapeName); i++) {
        char c1 = shapeName[i];
        glutBitmapCharacter(font1, c1);
    }

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawPyramid()
{
    glBegin(GL_TRIANGLES);

        glColor3f(0,0.75,0.5);
        glVertex3f(verticesOfPyramid[0][0], verticesOfPyramid[0][1] , verticesOfPyramid[0][2]); // Vertex Top
        glVertex3f(verticesOfPyramid[1][0], verticesOfPyramid[1][1] , verticesOfPyramid[1][2]); // Vertex Left
        glVertex3f(verticesOfPyramid[2][0], verticesOfPyramid[2][1] , verticesOfPyramid[2][2]); // Vertex Right

        glColor3f(1,1,0);
        glVertex3f(verticesOfPyramid[0][0], verticesOfPyramid[0][1] , verticesOfPyramid[0][2]); // Vertex Top
        glVertex3f(verticesOfPyramid[1][0], verticesOfPyramid[1][1] , verticesOfPyramid[1][2]); // Vertex Left
        glVertex3f(verticesOfPyramid[3][0], verticesOfPyramid[3][1] , verticesOfPyramid[3][2]); // Vertex Behind

        glColor3f(1,0,1);
        glVertex3f(verticesOfPyramid[0][0], verticesOfPyramid[0][1] , verticesOfPyramid[0][2]); // Vertex Top
        glVertex3f(verticesOfPyramid[2][0], verticesOfPyramid[2][1] , verticesOfPyramid[2][2]); // Vertex Right
        glVertex3f(verticesOfPyramid[3][0], verticesOfPyramid[3][1] , verticesOfPyramid[3][2]); // Vertex Behind

        glColor3f(1,1,1);
        glVertex3f(verticesOfPyramid[1][0], verticesOfPyramid[1][1] , verticesOfPyramid[1][2]); // Vertex Left
        glVertex3f(verticesOfPyramid[2][0], verticesOfPyramid[2][1] , verticesOfPyramid[2][2]); // Vertex Right
        glVertex3f(verticesOfPyramid[3][0], verticesOfPyramid[3][1] , verticesOfPyramid[3][2]); // Vertex Behind


    glEnd();
}

void drawCube() {
    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1, 0, 0);
    glVertex3f(verticesOfCube[0][0], verticesOfCube[0][1], verticesOfCube[0][2]); // Top-left
    glVertex3f(verticesOfCube[1][0], verticesOfCube[1][1], verticesOfCube[1][2]); // Top-right
    glVertex3f(verticesOfCube[3][0], verticesOfCube[3][1], verticesOfCube[3][2]); // Bottom-right
    glVertex3f(verticesOfCube[2][0], verticesOfCube[2][1], verticesOfCube[2][2]); // Bottom-left

    // Back face (green)
    glColor3f(0, 1, 0);
    glVertex3f(verticesOfCube[4][0], verticesOfCube[4][1], verticesOfCube[4][2]); // Top-left
    glVertex3f(verticesOfCube[5][0], verticesOfCube[5][1], verticesOfCube[5][2]); // Top-right
    glVertex3f(verticesOfCube[7][0], verticesOfCube[7][1], verticesOfCube[7][2]); // Bottom-right
    glVertex3f(verticesOfCube[6][0], verticesOfCube[6][1], verticesOfCube[6][2]); // Bottom-left

    // Left face (blue)
    glColor3f(0, 0, 1);
    glVertex3f(verticesOfCube[4][0], verticesOfCube[4][1], verticesOfCube[4][2]); // Top-left
    glVertex3f(verticesOfCube[0][0], verticesOfCube[0][1], verticesOfCube[0][2]); // Top-right
    glVertex3f(verticesOfCube[2][0], verticesOfCube[2][1], verticesOfCube[2][2]); // Bottom-right
    glVertex3f(verticesOfCube[6][0], verticesOfCube[6][1], verticesOfCube[6][2]); // Bottom-left

    // Right face (yellow)
    glColor3f(1, 1, 0);
    glVertex3f(verticesOfCube[1][0], verticesOfCube[1][1], verticesOfCube[1][2]); // Top-left
    glVertex3f(verticesOfCube[5][0], verticesOfCube[5][1], verticesOfCube[5][2]); // Top-right
    glVertex3f(verticesOfCube[7][0], verticesOfCube[7][1], verticesOfCube[7][2]); // Bottom-right
    glVertex3f(verticesOfCube[3][0], verticesOfCube[3][1], verticesOfCube[3][2]); // Bottom-left

    // Top face (magenta)
    glColor3f(1, 0, 1);
    glVertex3f(verticesOfCube[0][0], verticesOfCube[0][1], verticesOfCube[0][2]); // Top-left
    glVertex3f(verticesOfCube[1][0], verticesOfCube[1][1], verticesOfCube[1][2]); // Top-right
    glVertex3f(verticesOfCube[5][0], verticesOfCube[5][1], verticesOfCube[5][2]); // Bottom-right
    glVertex3f(verticesOfCube[4][0], verticesOfCube[4][1], verticesOfCube[4][2]); // Bottom-left

    // Bottom face (cyan)
    glColor3f(0, 1, 1);
    glVertex3f(verticesOfCube[2][0], verticesOfCube[2][1], verticesOfCube[2][2]); // Top-left
    glVertex3f(verticesOfCube[3][0], verticesOfCube[3][1], verticesOfCube[3][2]); // Top-right
    glVertex3f(verticesOfCube[7][0], verticesOfCube[7][1], verticesOfCube[7][2]); // Bottom-right
    glVertex3f(verticesOfCube[6][0], verticesOfCube[6][1], verticesOfCube[6][2]); // Bottom-left

    glEnd();
}

void drawSelectedShape() {
    switch (selectedShape) {
        case 0:
            drawPyramid();
            break;
        case 1:
            drawCube();
            break;
        case 2:
            //drawTetrahedron();
            break;
        default:
            drawPyramid(); // Default to pyramid
            break;
    }
}

void displayMain()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    updateCamera();  // Update the camera view
    displayAxes();

    drawSelectedShape();

    glutSwapBuffers();
}

void update1(int value) 
{
    Ex=eyeX;
    Ey=eyeY;
    Ez=eyeZ;
    displayCamLocation();
    glutPostRedisplay();
    glutTimerFunc(16, update1, 0);  // 60 FPS
}

void createMenu() 
{
    int menu = glutCreateMenu([](int value) {
        selectedShape = value; // Update selected shape
        glutPostRedisplay(); // Redraw the scene
    });
    glutAddMenuEntry("Pyramid", 0);
    glutAddMenuEntry("Cube", 1);
    glutAddMenuEntry("Tetrahedron", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void displaySubWin1()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1,1,1);

    update1(1000/60);

    
    char Topic[]="3D - TRANSFORMATION";
    displayMethodName(Topic,-130,30);

    char MadeBy[]="Made By:- Aryan Dubey 500109841";
    displayOptions(MadeBy,175,10);

    glColor3f(1,0,0);
    char Info[]="Press 'I' For Info";
    displayOptions(Info,-355,30);

    glutSwapBuffers();
}

void drawNewWindow()
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(-windowSizeX,windowSizeX,-windowSizeY,windowSizeY);

	glColor3f(1,1,1);
	char rule[] = "RULES FOR IMPLEMENTATION";
	displayMethodName1(rule, -180, 190);

    glColor3f(0,0,1);
	
    char rule1[] = "1) Use AD Keys to move around in X-Axis (Red Line)";
    char rule2[] = "2) Use QE Keys to move around in Y-Axis (Green Line)";
    char rule3[] = "3) Use WS Keys to move around in Z-Axis (Blue Line)";
    char rule4[] = "4) Press 'R' to Rotate the Tetrahedron About its Centroid.";
    char rule5[] = "5) Press 'T' to Translate the Tetrahedron.";
    char rule6[] = "6) Press 'K' to Scale the Tetrahedron.";
    char rule7[] = "7) Press 'H' to Shear the Tetrahedron.";
    char rule8[] = "8) Press 'X' to Reflect the Tetrahedron About XY-Plane.";
    char rule9[] = "9) Press 'Y' to Reflect the Tetrahedron About YZ-Plane.";
    char rule10[] = "10) Press 'Z' to Reflect the Tetrahedron About ZX-Plane.";
    char rule11[] = "11) Make sure to give the Transformation Factors in the terminal.";
    char rule12[] = "12) Press 'I' or 'i button' For Info.";
    char rule13[] = "13) Press 'I' Again or 'back button' To go back.";
    char rule14[] = "14) Press Escape Button to Exit.";
    char rule15[] = "15) Press 'F' to focus on the Tetrahedron.";
    
    glColor3f(1,0,0);
	displaySentence(rule1, -150, 150);
	glColor3f(0,1,0);
    displaySentence(rule2, -150, 135);
	glColor3f(0,0.8,1);
    displaySentence(rule3, -150, 120);
	glColor3f(1,1,1);
    displaySentence(rule4, -150, 105);
	displaySentence(rule5, -150, 90);
	displaySentence(rule6, -150, 75);
	displaySentence(rule7, -150, 60);
	displaySentence(rule8, -150, 45);
	displaySentence(rule9, -150, 30);
	displaySentence(rule10, -157.5, 15);
	displaySentence(rule11, -157.5, 0);
	displaySentence(rule12, -157.5, -15);
    displaySentence(rule13, -157.5, -30);
    displaySentence(rule14, -157.5, -45);
    displaySentence(rule15, -157.5, -60);

    
    glutSwapBuffers();
}

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

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, windowSizeX / windowSizeY, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void update(int value) 
{
    angle += 0.0f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // 60 FPS
}

void handleKeyPress(unsigned char key, int x, int y) 
{
    switch(key) {
        case 's':  // Move forward
            eyeZ -= moveSpeed;
            break;
        case 'w':  // Move backward
            eyeZ += moveSpeed;
            break;
        case 'a':  // Move right
            eyeX -= moveSpeed;
            break;
        case 'd':  // Move left
            eyeX += moveSpeed;
            break;
        case 'e':  // Move up
            eyeY += moveSpeed;
            break;
        case 'q':  // Move down
            eyeY -= moveSpeed;
            break;
    }

    //TRANSLATION
    if(key=='t'||key=='T')
	{
		cout<<endl;
		cout<<"Enter Tralastion in x = ";
		cin>>tx;
		cout<<endl;
		cout<<"Enter Tralastion in y = ";
		cin>>ty;
		cout<<endl;
        cout<<"Enter Tralastion in z = ";
		cin>>tz;
		cout<<endl;

		Tr[3][0]=tx;
		Tr[3][1]=ty;
        Tr[3][2]=tz;

		MatMultiply(Tr,verticesOfPyramid);
		cout<<endl;
		cout<<"TRANSLATED MATRIX";
		cout<<endl;
		cout<<endl;

		displayMat();
		glutPostRedisplay();
	}

    //ROTATION
    else if (key == 'r' || key == 'R') 
    {

        if (selectedShape==1)
        {
            displayMat8x4();
        }

        else
    {
        displayMat();
        cout << endl;
        cout << "Enter Angle (in DEGREES) at which you want to Rotate the shape: ";
        
        float angle;
        cin >> angle;
        cout << endl;

        float theta = angle * (PI / 180.0f); // Conversion from Degree to Radian

        cout << endl;
        char Axis;
        cout << "Enter the Axis around which you want to Rotate the shape (x, y, z): ";
        cin >> Axis;
        cout << endl;

        float Tr[4][4] = {0}; // Initialize translation matrix
        float RotZ[4][4] = {0}; // Initialize rotation matrix

        // Calculate the centroid of the pyramid
        centroidX = (verticesOfPyramid[0][0] + verticesOfPyramid[1][0] + 
                               verticesOfPyramid[2][0] + verticesOfPyramid[3][0]) / 4;
        centroidY = (verticesOfPyramid[0][1] + verticesOfPyramid[1][1] + 
                               verticesOfPyramid[2][1] + verticesOfPyramid[3][1]) / 4;
        centroidZ = (verticesOfPyramid[0][2] + verticesOfPyramid[1][2] + 
                               verticesOfPyramid[2][2] + verticesOfPyramid[3][2]) / 4;

        // Set the translation matrix to identity
        for (int i = 0; i < 4; i++) 
        {
            Tr[i][i] = 1.0f;
        }

        if (Axis == 'z' || Axis == 'Z') 
        {
            // Translate to origin
            Tr[3][0] = -centroidX;
            Tr[3][1] = -centroidY;
            Tr[3][2] = -centroidZ;

            MatMultiply(Tr, verticesOfPyramid);

            double roundedCos = round(cos(theta) * 100) / 100.0;
            double roundedSin = round(sin(theta) * 100) / 100.0;

            // Set up rotation matrix for Z-axis
            RotZ[0][0] = roundedCos;
            RotZ[0][1] = roundedSin;
            RotZ[1][0] = -roundedSin;
            RotZ[1][1] = roundedCos;
            RotZ[2][2] = 1.0f; // Keep Z component unchanged
            RotZ[3][3] = 1.0f; // Homogeneous coordinate

            /*cout<<endl;
            cout<<roundedCos;
            cout<<endl;*/

            MatMultiply(RotZ,verticesOfPyramid);

            // Translate back to original position
            Tr[3][0] = centroidX;
            Tr[3][1] = centroidY;
            Tr[3][2] = centroidZ;

            MatMultiply(Tr, verticesOfPyramid);
            displayMat();
        }
        else if (Axis == 'x' || Axis == 'X') 
        {
            // Translate to origin
            Tr[3][0] = -centroidX;
            Tr[3][1] = -centroidY;
            Tr[3][2] = -centroidZ;

            MatMultiply(Tr, verticesOfPyramid);

            double roundedCos = round(cos(theta) * 100) / 100.0;
            double roundedSin = round(sin(theta) * 100) / 100.0;

            // Set up rotation matrix for Z-axis
            RotX[1][1] = roundedCos;
            RotX[1][2] = roundedSin;
            RotX[2][1] = -roundedSin;
            RotX[2][2] = roundedCos;
            RotX[0][0] = 1.0f; // Keep Z component unchanged
            RotX[3][3] = 1.0f; // Homogeneous coordinate

            /*cout<<endl;
            cout<<roundedCos;
            cout<<endl;*/

            MatMultiply(RotX,verticesOfPyramid);

            // Translate back to original position
            Tr[3][0] = centroidX;
            Tr[3][1] = centroidY;
            Tr[3][2] = centroidZ;

            MatMultiply(Tr, verticesOfPyramid);
            displayMat();
        }
        else if (Axis == 'y' || Axis == 'Y') 
        {
            // Translate to origin
            Tr[3][0] = -centroidX;
            Tr[3][1] = -centroidY;
            Tr[3][2] = -centroidZ;

            MatMultiply(Tr, verticesOfPyramid);

            double roundedCos = round(cos(theta) * 100) / 100.0;
            double roundedSin = round(sin(theta) * 100) / 100.0;

            // Set up rotation matrix for Z-axis
            RotY[0][0] = roundedCos;
            RotY[0][2] = roundedSin;
            RotY[2][0] = -roundedSin;
            RotY[2][2] = roundedCos;
            RotY[1][1] = 1.0f; // Keep Z component unchanged
            RotY[3][3] = 1.0f; // Homogeneous coordinate

            /*cout<<endl;
            cout<<roundedCos;
            cout<<endl;*/

            MatMultiply(RotY,verticesOfPyramid);

            // Translate back to original position
            Tr[3][0] = centroidX;
            Tr[3][1] = centroidY;
            Tr[3][2] = centroidZ;

            MatMultiply(Tr, verticesOfPyramid);
            displayMat();
        }
    }
    }
    
    //SCALING
    else if(key=='k'||key=='K')
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

            cout<<"Enter Scaling Factor to Scale the shape in z-axis (Sz) : ";
			cin>>Sz;
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
            Sz=Sx;
			cout<<endl;
		}

		Sc[0][0]=Sx;
		Sc[1][1]=Sy;
        Sc[2][2]=Sz;

		MatMultiply(Sc,verticesOfPyramid);
		cout<<endl;
		cout<<"SCALED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

    //REFLECTION
	else if(key=='x'||key=='X')
	{
		cout<<"Shape Reflected About XY-Plane !!!";
		cout<<endl;
		MatMultiply(RefXY,verticesOfPyramid);
		cout<<endl;
		cout<<"REFLECTED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}
	else if(key=='y'||key=='Y')
	{
		cout<<"Shape Reflected About YZ-Plane !!!";
		cout<<endl;
		MatMultiply(RefYZ,verticesOfPyramid);
		cout<<endl;
		cout<<"REFLECTED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}
    else if(key=='z'||key=='Z')
	{
		cout<<"Shape Reflected About ZX-Plane !!!";
		cout<<endl;
		MatMultiply(RefZX,verticesOfPyramid);
		cout<<endl;
		cout<<"REFLECTED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

    //SHEARING
	else if(key=='h'||key=='H')
	{
		int yesNo;
		cout<<endl;
		cout<<"In How Many directions do you want to shear the shape ? (2/3) : ";
		cin>>yesNo;
		cout<<endl;
       
		if (yesNo==2)
		{
            string direction;
			cout<<"The Shapes will be Sheared in only 2 directions";
			cout<<endl;
			cout<<endl;
            cout<<"Enter the Directions : ";
            cin>>direction;
            cout<<endl;

            if(direction=="xy" )
            {
                cout<<"Enter Shearing Factor to Shear the shape in xy-axis (SHxy) : ";
			    cin>>SHxy;
			    cout<<endl;
            }
            else if(direction=="yx")
            {
                cout<<"Enter Shearing Factor to Shear the shape in yx-axis (SHyx) : ";
			    cin>>SHyx;
			    cout<<endl;
            }
            else if(direction=="yz")
            {
                cout<<"Enter Shearing Factor to Shear the shape in yz-axis (SHyz) : ";
			    cin>>SHyz;
                cout<<endl;
            }
            else if(direction=="zy")
            {
                cout<<"Enter Shearing Factor to Shear the shape in zy-axis (SHzy) : ";
			    cin>>SHzy;
                cout<<endl;
            }
            else if(direction=="xz")
            {
                cout<<"Enter Shearing Factor to Shear the shape in xz-axis (SHxz) : ";
			    cin>>SHxz;
                cout<<endl;
            }
            else if(direction=="zx")
            {
                cout<<"Enter Shearing Factor to Shear the shape in xz-axis (SHzx) : ";
			    cin>>SHzx;
                cout<<endl;
            }
		}
		else if (yesNo==3)
		{
			cout<<"The Shapes will be Sheared similarly across all directions";
			cout<<endl;
			cout<<endl;
			cout<<"Enter Shearing Factor to Scale the shape (SH) : ";
			cin>>SHxy;

            Shear[1][0] = SHxy; // xy
            Shear[0][1] = SHxy; // yx
            Shear[2][1] = SHxy; // yz
            Shear[1][2] = SHxy; // zy
            Shear[2][0] = SHxy; // xz
            Shear[0][2] = SHxy; // zx

			cout<<endl;
		}

		Shear[1][0]=SHxy;
        Shear[0][1]=SHyx;

        Shear[2][0]=SHxz;
        Shear[0][2]=SHzx;

        Shear[2][1]=SHyz;
        Shear[1][2]=SHzy;

		MatMultiply(Shear,verticesOfPyramid);
		cout<<endl;
		cout<<"SHEARED MATRIX";
		cout<<endl;
		cout<<endl;
		displayMat();
		glutPostRedisplay();
	}

    //SCREEN SWITCHING
    else if(key == 'i' || key == 'I')
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

    else if(key==27)
	{
		exit(0);
	}

    else if(key=='f'||key=='F')
    {
        centerX = centroidX, centerY = centroidY, centerZ = centroidZ;  // Camera looks at the Centroid

        eyeX=centroidX;
        eyeY=centroidY+5;
        eyeZ=centroidZ+10;
        updateCamera();
    }
    
    // Calculate the centroid of the pyramid
        centroidX = (verticesOfPyramid[0][0] + verticesOfPyramid[1][0] + 
                               verticesOfPyramid[2][0] + verticesOfPyramid[3][0]) / 4;
        centroidY = (verticesOfPyramid[0][1] + verticesOfPyramid[1][1] + 
                               verticesOfPyramid[2][1] + verticesOfPyramid[3][1]) / 4;
        centroidZ = (verticesOfPyramid[0][2] + verticesOfPyramid[1][2] + 
                               verticesOfPyramid[2][2] + verticesOfPyramid[3][2]) / 4;

    updateCamera();  // Update the camera view
    glutPostRedisplay();  // Redraw the scene
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(2*windowSizeX, 2*windowSizeY);
    glutInitWindowPosition(700,0);
    int windowMain = glutCreateWindow("3D Cube Transformations");
    glutDisplayFunc(displayMain);
    init();
    
    int subWin1 = glutCreateSubWindow(windowMain, 0, 0, 2*windowSizeX, 50);                         // White Window
    gluOrtho2D(-windowSizeX, windowSizeX, 0, 50);
    glutDisplayFunc(displaySubWin1);
    //createMenu();
    glutTimerFunc(0, update1, 0);
    init1();

    int subWin2 = glutCreateSubWindow(windowMain, 0, 0, 2*windowSizeX, 2 * windowSizeY);            // Black 3D-Space
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyPress);
    glutTimerFunc(0, update, 0);
    init2();
    glutMainLoop();
    return 0;
}
