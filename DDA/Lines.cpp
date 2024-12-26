#include<freeglut.h>
#include <iostream>
using namespace std;

float m;

float X1;
float Y1;
float X2;
float Y2;

float windowSizeX = 400;
float windowSizeY = 400;

bool isDrawing = false;

void display();

void DDA(float x1, float x2, float y1, float y2);
void DDA2(float x1, float x2, float y1, float y2);


void DDA(float x1, float x2, float y1, float y2)
{
    m = (y1-y2)/(x1-x2);
    float x;
    float y;
    

    if (m<=1 && m>=0)
    {
        cout<<"m<=1 && m>=0"<<endl;
        for ( float i = X1; i >= X2; i=i-0.01)
        {
            x = i;
            y = y1+ ((m)*(i-X1));
            glBegin(GL_POINTS);
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
    if (m>1 || m<-1)
    {
        cout<<"m>1 && m<-1"<<endl;
        for ( float i = Y1; i >= Y2; i=i-0.01)
        {
            x = x1+ ((1/m)*(i-Y1));
            y = i;
            glBegin(GL_POINTS);
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
    
    if (m<0 && m>-1)
    {
        cout<<"m<0 && m>-1"<<endl;
        for ( float i = X2; i >= X1; i=i-0.01)
        {
            x = i;
            y = y2+ ((m)*(i-X2));
            glBegin(GL_POINTS);
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
}

void DDA2(float x1, float x2, float y1, float y2)
{
    m = (y1-y2)/(x1-x2);
    float x;
    float y;
    
    cout<<"DDA 2"<<endl;

    if (m<=1 && m>=0)
    {
        cout<<"m<=1 && m>=0"<<endl;
        for ( float i = X1; i >= X2; i=i-0.01)
        {
            x = i;
            y = y1+ ((m)*(i+X1));
            glBegin(GL_POINTS);
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
    if (m>1 || m<-1)
    {
        cout<<"m>1 && m<-1"<<endl;
        for ( float i = Y2; i >= Y1; i=i-0.01)
        {
            x = x1+ ((1/m)*(i-Y1));
            y = i;
            glBegin(GL_POINTS);
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
    
    if (m<0 && m>-1)
    {
        cout<<"m<0 && m>-1"<<endl;
        for ( float i = X2; i >= X1; i=i-1)
        {
            x = i;
            y = y1+ ((m)*(i-X2));
            glBegin(GL_POINTS);
                cout<<"y="<<y<<endl;
                glPointSize(2.0);
                glColor3f(1.0,1.0,1.0);
                glVertex2f(x,y);
            glEnd();
        }
    }
}



void drawLine()
{
    if (Y1>Y2)
    {
        DDA(X1, X2, Y1, Y2);
    }
    else if (Y1<Y2)
    {
        DDA2(X1, X2, Y1, Y2);
    }
    cout<<"The Slope of the line is = "<<m<<endl;
}

void displayAxes()
{
    glColor3f(0.7,1,1);
    glBegin(GL_LINES);
        
        glVertex2f(-windowSizeX,0);
        glVertex2f(windowSizeX,0);

        glVertex2f(0,-windowSizeY);
        glVertex2f(0,windowSizeY);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    displayAxes();

    if(isDrawing)
    {
        drawLine();
        isDrawing=false;
    }

    glColor4f(1.0f,1.0f,0.0f,1.0f);

    glRasterPos2f(230,-390);

    char s[] = "Made By Aryan Dubey [G&G(Batch-2)]";
    void * font = GLUT_BITMAP_TIMES_ROMAN_10;
    for (int i = 0; i<strlen(s);i++)
    {
        char c = s[i];
        glutBitmapCharacter(font, c);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glutSwapBuffers();
}

// Function to handle mouse click events
void mouse(int button, int state, int x, int y) 
{
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        // Convert window coordinates to OpenGL coordinates
        X1 = (float)x-windowSizeX;
        Y1 = windowSizeY-(float)y;

        cout << "Mouse Clicked at: (" << X1 << ", " << Y1 << ")"<<endl;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    {
        // Convert window coordinates to OpenGL coordinates
        X2 = (float)x-windowSizeX;
        Y2 = windowSizeY-(float)y;

        cout << "Mouse Released at: (" << X2 << ", " << Y2 << ")"<<endl;
        isDrawing=true;
    }
    
}

void timer(int)
{
    glutPostRedisplay();
}


void init() 
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowSizeX*2, windowSizeY*2);
    glutInitWindowPosition(350, 0);
    glutCreateWindow("Line Drawing using DDA Algorithm");

    init();

    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}