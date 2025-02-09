#include <freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <utility>
#include <string>
#include <set>

using namespace std;

// Window dimensions
const int windowSizeX = 400;
const int windowSizeY = 400;

float X1=1.0, Y1=0.4; 

bool isFilling=true;

// Initialize Value of PIE for DEGREE to RADIAN Conversion
const float M_PI = 3.14159265358979323846;


// Structure to store 2D points
struct Point {
    float x, y;
    Point(float _x, float _y) : x(_x), y(_y) {}
};

vector<Point> boundary = {};

struct Color {
    float r, g, b;
};

Point calculateBezierPoint(const std::vector<Point>& controlPoints, float t) 
{
    //This is the Blending Function or Basis Function
    float x = 0, y = 0;
    int n = controlPoints.size() - 1;           // Degree Of Curve (Drawback:- Dependence on CPs)
    
    for (int i = 0; i <= n; i++) 
    {
        /*tgamma function is used to calculate the "n C i" selected values 
        without using factorial function to reduce time complexity and increase efficiency.*/

        // Coefficient = Blending Function

        float coefficient = pow(1 - t, n - i) * pow(t, i) * tgamma(n + 1) / (tgamma(i + 1) * tgamma(n - i + 1));

        x += coefficient * controlPoints[i].x;
        y += coefficient * controlPoints[i].y;
    }
    
    return Point(x, y);
}

void circle()
{
    float x = 0, y = 0;

    for (int i = 0; i < 360; i++) 
    {
        float angle = i * M_PI / 180.0f;
        x=140.0 * cos(angle);
        y=140.0 * sin(angle) - 40.0;
        glVertex2f(x, y);
        boundary.push_back(Point(x, y));
    }

    
}

void drawBezierCurve(const std::vector<Point>& controlPoints, int segments = 50) 
{
    glLineWidth(7.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = static_cast<float>(i) / segments;
        Point p = calculateBezierPoint(controlPoints, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawCrown(const std::vector<Point>& controlPoints, int segments = 50) 
{
    glLineWidth(7.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = static_cast<float>(i) / segments;
        Point p = calculateBezierPoint(controlPoints, t);
        boundary.push_back(p);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

// Coloring Algos Below

void getPixelColor(int x, int y, GLfloat color[3]) 
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

// Function to set the color of a pixel (x, y)
void setPixelColor(int x, int y, GLfloat color[3]) 
{
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

bool isInsideCrownBoundary(float x, float y) 
{
    // Simple bounding box check for now (you could use a more advanced method)
    for (const auto& p : boundary) {
        if (fabs(x - p.x) < 2.0f && fabs(y - p.y) < 2.0f) 
        {
            return true;  // Pixel is near the boundary
        }
    }

    return false;
}


// Stack-based iterative flood fill function
void floodFill(int x, int y, GLfloat fillColor[3], GLfloat boundaryColor[3]) 
{
    stack<pair<int, int>> pixelStack;
    set<pair<int, int>> visitedPixels; // Track visited pixels
    pixelStack.push({x, y});

    GLfloat currentColor[3];
    const float tolerance = 0.01f;  // Tolerance for color comparison

    while (!pixelStack.empty() && isFilling == true) 
    {
        pair<int, int> p = pixelStack.top();
        pixelStack.pop();

        int px = p.first;
        int py = p.second;

        // Check if the pixel has already been visited
        if (visitedPixels.find(p) != visitedPixels.end()) {
            continue;
        }
        visitedPixels.insert(p);

        // Ensure the coordinates are within the bounds
        if (isInsideCrownBoundary(px, py)) {
            continue;  // Stop if we are on the boundary of the crown
        }

        // Get the color of the current pixel
        getPixelColor(px, py, currentColor);

        // If the current pixel is neither the fill color nor the boundary color, we can fill it
        if ((fabs(currentColor[0] - boundaryColor[0]) > tolerance ||
             fabs(currentColor[1] - boundaryColor[1]) > tolerance ||
             fabs(currentColor[2] - boundaryColor[2]) > tolerance) &&
            (fabs(currentColor[0] - fillColor[0]) > tolerance ||
             fabs(currentColor[1] - fillColor[1]) > tolerance ||
             fabs(currentColor[2] - fillColor[2]) > tolerance)) 
        {
            // Set the pixel to the fill color
            setPixelColor(px, py, fillColor);

            // Push neighboring pixels onto the stack with integer increments for pixels
            pixelStack.push({px + 1, py});  // Right
            pixelStack.push({px - 1, py});  // Left
            pixelStack.push({px, py + 1});  // Top
            pixelStack.push({px, py - 1});  // Bottom
        }
        else if (fabs(currentColor[0] - boundaryColor[0]) <= tolerance && fabs(currentColor[1] - boundaryColor[1]) <= tolerance 
            && fabs(currentColor[2] - boundaryColor[2]) <= tolerance) 
        {
            continue;  // Stop filling if we hit the boundary color
        }
    }
    isFilling = false;
    cout << "Filling Done !!!" << endl;
}


void displayMethodName(char shapeName[], int PosX, int PosY)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY); // 2D projection for text rendering


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); // Reset modelview matrix for 2D rendering

    glTranslatef(PosX, PosY, 0.0f);  // Adjust text position

    glPushMatrix(); // Push matrix for scaling
    glScalef(0.5f, 0.5f, 1.0f);
    void *font1 = GLUT_STROKE_ROMAN;
    for (int i = 0; i < strlen(shapeName); i++) {
        char c1 = shapeName[i];
        glutStrokeCharacter(font1, c1);
    }
    glPopMatrix(); // Pop matrix to return to original state

    // Restore matrices to return to 3D
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void displayBound()
{
    for (const Point& p : boundary) 
    {
        cout << "Point(" << p.x << ", " << p.y << ")" << endl;
    }
}

void drawRealMadridLogo() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    // Set color to Royal Gold
    glColor3f(1.0f, 0.843f, 0.0f);

    std::vector<std::vector<Point>> crownCurves = {
        // Upper Left Curve
        {Point(-120.0, 160.0), Point(-60.0, 120.0), Point(0.0, 200.0)},
        // Upper Right Curve
        {Point(0.0, 200.0), Point(60.0, 120.0), Point(120.0, 160.0)},
        // Left
        {Point(-80.0, 88.0), Point(-80.0, 140.0), Point(-120.0, 160.0)},
        // Right
        {Point(80.0, 88.0), Point(80.0, 140.0), Point(120.0, 160.0)},
        // Base
        {Point(-80.0, 88.0), Point(0.0, 140.0), Point(80.0, 88.0)}
    };
    
    // Draw crown
    for (const auto& curve : crownCurves) {
        drawCrown(curve);
    }
    
    GLfloat fillColor[3] = {1.0, 0.843, 0.0};
    GLfloat boundaryColor[3] = {1.0, 0.843, 0.0};
    if (isFilling)
    {
        floodFill(0.0f, 150.0f, fillColor, boundaryColor);
    }

    boundary.clear();

    // Circle for the main part of the logo
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1,1,1);
        circle();
    glEnd();
    glFlush();
    
    //displayBound();
    
    boundary.clear();

    // Blue Polygon inside the circle
    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
        glVertex2f(-80,76);
        glVertex2f(120,-115);
        glVertex2f(105,-135);
        glVertex2f(80,-157);
        glVertex2f(-130,20);
        glVertex2f(-105,55);
    glEnd();
    
    glColor3f(1.0f, 0.843f, 0.0f);

    glBegin(GL_LINE_LOOP);
        circle();
    glEnd();
    glFlush();

    // Letters "MCF"

    std::vector<std::vector<Point>> letterM = 
    {
        // Left vertical
        {Point(-40.0, 80.0), Point(-160.0, -40.0), Point(-40.0, -140.0)},
        // Middle part
        {Point(-40.0, 80.0), Point(0.0, -120.0), Point(40.0, 80.0)},
        // Right vertical
        {Point(40.0, 80.0), Point(160.0, -40.0), Point(40.0, -140.0)}
    };

    std::vector<std::vector<Point>> letterC = 
    {
        // C Curve
        {Point(50.0, 20.0), Point(-20.0, 120.0), Point(-100.0, 20.0), Point(-100.0, -120.0), Point(-20.0, -160.0), Point(40.0, -120.0)},
    };

    std::vector<std::vector<Point>> letterF = 
    {
        // Upper horizontal
        {Point(-10.0, -40.0), Point(40.0, -40.0)},
        // Middle horizontal
        {Point(-10.0, -80.0), Point(20.0, -80.0)},
        // Vertical
        {Point(-10.0, -40.0), Point(-10.0, -140.0)}
    };
    
    // Draw letters
    for (const auto& curve : letterM) 
    {
        drawBezierCurve(curve);
    }
    for (const auto& curve : letterC) {
        drawBezierCurve(curve);
    }
    for (const auto& curve : letterF) {
        drawBezierCurve(curve);
    }


    glColor3f(1.0f, 0.843f, 0.0f);
    char RMCF[]="REAL MADRID C.F";
    displayMethodName(RMCF, -275, 250);

    glFlush();
    glutSwapBuffers();
}



// Keyboard Function
void keyboard(unsigned char key, int x, int y)
{
    if (key==' ')
    {
        isFilling=true;
        cout<<"Filling = True";
        cout<<endl;
        glutPostRedisplay();
    }
    else if(key==27)
    {
        exit(0);
    }
}

//Mouse Click Function
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        isFilling=false;
        glutPostRedisplay();
    }
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY); 
    glMatrixMode(GL_MODELVIEW);
}

// Window resize callback
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / h;
    if (w >= h) {
        gluOrtho2D(-400.0 * aspect, 400.0 * aspect, -400.0, 400.0); 
    } else {
        gluOrtho2D(-400.0, 400.0, -400.0 / aspect, 400.0 / aspect); 
    }
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(2*windowSizeX, 2*windowSizeY);
    glutInitWindowPosition(700,0);
    int windowMain = glutCreateWindow("Real Madrid Logo");

    init();

    glutDisplayFunc(drawRealMadridLogo);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouseClick);


    glutMainLoop();
    return 0;
}
