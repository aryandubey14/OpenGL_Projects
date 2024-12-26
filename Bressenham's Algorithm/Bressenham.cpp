#include <freeglut.h>
#include <cmath>
#include <iostream>

int windowSizeX = 400;
int windowSizeY = 400;

int startX, startY, endX, endY;
int clickCount=0;
bool pointClicked = false; // Flag to check if the first point is clicked

// Function to draw a pixel at (x, y)
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's Line Algorithm
void bresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x1, y1); // Draw the pixel

        // Check if we have reached the end point
        if (x1 == x2 && y1 == y2) break;

        int err2 = err * 2;
        if (err2 > -dy) { // Move in x direction
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) { // Move in y direction
            err += dx;
            y1 += sy;
        }
    }
}

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

void displaySentence(char shapeName[], int PosX, int PosY)
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

// Display function for OpenGL
void displayMain() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 1.0);
    char Name1[] = "BRESSENHAM'S LINE ALGORITHM";
    displayMethodName(Name1, 50,770);

    if (clickCount==0)
    {
        glColor3f(1,0,0);
        glBegin(GL_QUADS);
            glVertex2f(750,780);
            glVertex2f(760,780);
            glVertex2f(760,790);
            glVertex2f(750,790);
        glEnd();
    }
    else
    {
        glColor3f(0,1,0);
        glBegin(GL_QUADS);
            glVertex2f(750,780);
            glVertex2f(760,780);
            glVertex2f(760,790);
            glVertex2f(750,790);
        glEnd();
    }

    glColor3f(0,1,1);
    // Draw the line only if both points are clicked
    if (pointClicked) 
    {
        bresenham(startX, startY, endX, endY);
    }

    glFlush();
}

// Mouse click callback function
void mouseClick(int button, int state, int x, int y) {
    // Convert mouse coordinates to OpenGL coordinates
    int yInvert = windowSizeY * 2 - y; // Invert y-axis

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (clickCount==0) {
            // First point
            startX = x;
            startY = yInvert;
            clickCount++;
            std::cout << "Start Point: (" << startX << ", " << startY << ")\n";
        } 
        else if (clickCount==1)
        {
            // Second point
            endX = x;
            endY = yInvert;
            pointClicked = true;
            std::cout << "End Point: (" << endX << ", " << endY << ")\n";
            glutPostRedisplay();  // Request display update
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
    {
        clickCount=0;
        pointClicked = false;
        glutPostRedisplay();
    }
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glColor3f(0.0, 1.0, 1.0);         // Line color = Cyan
    glPointSize(2.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(2 * windowSizeX, 2 * windowSizeY);
    glutInitWindowPosition(350, 0);
    glutCreateWindow("Bresenham's Algorithm with Mouse Clicks");
    gluOrtho2D(0, 2 * windowSizeX, 0, 2 * windowSizeY); // Adjust the orthographic projection
    glutDisplayFunc(displayMain);
    init();
    
    // Register mouse click callback function
    glutMouseFunc(mouseClick);

    glutMainLoop();
    return 0;
}
