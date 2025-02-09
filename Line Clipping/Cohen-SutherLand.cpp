#include <freeglut.h> // Include the GLUT header file
#include<iostream>
#include<cmath>

#define PI 3.14159265358979323846

using namespace std;

float windowSizeX=800;
float windowSizeY=540;


// Defining region codes for the Cohen-Sutherland algorithm
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

float maxOf(float T[]);
float minOf(float T[]);

void displaySubWin1();
void displaySubWin2();
void drawNewWindow();
void drawNewWindow2();

int currentScene=0;

// Window boundaries
float xMin, yMin, xMax, yMax;
float X1,X2,Y1,Y2;
float t1=0,t2=1;
int clickCount=0;
bool isClipped = false;
bool animateClipping = false;
bool isDiscarded=false;
float animT1 = 0, animT2 = 1; // Animation progress variables

// Line coordinates (original values)
float lineX1, lineY1, lineX2, lineY2;
int clickCount1=0;
// Clipping region's boundary codes for the endpoints
int code1, code2;
// Flag to track if we're setting the clipping window or the line
bool settingClippingWindow = true; // Start with setting the clipping window
float xMinCS, yMinCS, xMaxCS, yMaxCS;
float outX1,outY1,outX2,outY2;

float angle= 10;

void animate(int value);
//void animateCohenSutherland(int value);

// Function to compute the region code for a point (x, y)
int computeCode(float x, float y) 
{
    int code = INSIDE;

    if (x < xMinCS)       // to the left of the rectangle
        code |= LEFT;            // |= is Bitwise OR Operation
    else if (x > xMaxCS)  // to the right of the rectangle
        code |= RIGHT;
    if (y < yMinCS)       // below the rectangle
        code |= BOTTOM;
    else if (y > yMaxCS)  // above the rectangle
        code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping algorithm
void cohenSutherlandClip(float lineX1, float lineY1, float lineX2, float lineY2) 
{
    bool accept = false;
    int code1 = computeCode(lineX1, lineY1); // Compute region codes for the endpoints
    int code2 = computeCode(lineX2, lineY2);

    while (true) {
        if ((code1 == INSIDE) && (code2 == INSIDE)) {
            // Trivial accept: Both points are inside the rectangle
            accept = true;
            break;
        } else if (code1 & code2) {
            // Trivial reject: Both points are outside and in the same region
            return; // The line is completely outside, no need to draw anything
        } else {
            // Partial acceptance: Clip the line
            int codeOut;
            float x, y;

            // Choose one of the points that is outside
            if (code1 != INSIDE)
                codeOut = code1;
            else
                codeOut = code2;

            // Calculate the intersection point using the boundary where the point is outside
            if (codeOut & TOP) { // Point is above the clipping window
                x = lineX1 + (lineX2 - lineX1) * (yMaxCS - lineY1) / (lineY2 - lineY1);  //Using Equation of line y-y1=(delY/delX)*(x-x1)
                y = yMaxCS;
            } else if (codeOut & BOTTOM) { // Point is below the clipping window
                x = lineX1 + (lineX2 - lineX1) * (yMinCS - lineY1) / (lineY2 - lineY1);
                y = yMinCS;
            } else if (codeOut & RIGHT) { // Point is to the right of the clipping window
                y = lineY1 + (lineY2 - lineY1) * (xMaxCS - lineX1) / (lineX2 - lineX1);
                x = xMaxCS;
            } else if (codeOut & LEFT) { // Point is to the left of the clipping window
                y = lineY1 + (lineY2 - lineY1) * (xMinCS - lineX1) / (lineX2 - lineX1);
                x = xMinCS;
            }

            // Replace the point outside the clipping window with the intersection point
            if (codeOut == code1) {
                lineX1 = x;
                lineY1 = y;
                code1 = computeCode(lineX1, lineY1); // Recompute the region code for the new point
            } else {
                lineX2 = x;
                lineY2 = y;
                code2 = computeCode(lineX2, lineY2);
            }
        }
    }

    // If the line is accepted, set the output coordinates and draw the clipped line
    if (accept) {
        outX1 = lineX1;
        outY1 = lineY1;
        outX2 = lineX2;
        outY2 = lineY2;

        // Drawing the clipped line
        /*glBegin(GL_LINES);
            glColor3f(0.0, 1.0, 0.0); // Set the color to green for the clipped line
            glVertex2f(outX1, outY1);
            glVertex2f(outX2, outY2);
        glEnd();
        glFlush();*/
        return;
    } else {
        outX1 = outY1 = outX2 = outY2 = -1; // Set to invalid values if rejected
    }
}


float maxOf(float T[])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0;  j < 4;  j++)
        {
            if (T[i]<T[j])
            {
                float temp = T[i];
                T[i]=T[j];
                T[j]=temp;
            }
        }
    }

    return T[3];
}

float minOf(float T[])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0;  j < 4;  j++)
        {
            if (T[i]<T[j])
            {
                float temp = T[i];
                T[i]=T[j];
                T[j]=temp;
            }
        }
    }

    return T[0];
}

void LiangBarskey(float x1, float y1, float x2, float y2) 
{
    float delX=x2-x1;
    float delY=y2-y1;

    int k=1;
    float Pk,Qk;
    float T2[4]={1,1,1,1};
    float T1[4]={0,0,0,0};

    for ( k = 1; k <= 4; k++)
    {
        if(k==1)
        {
            Pk = -1*delX;
            Qk = x1-xMin;
        }
        else if (k==2)
        {
            Pk = delX;
            Qk = xMax-x1;
        }
        else if (k==3)
        {
            Pk = -1*delY;
            Qk = y1-yMin; 
        }
        else if (k==4)
        {
             Pk = delY;
            Qk = yMax-y1; 
        }

        float Rk=Qk/Pk;
        
        //cout<<"Pk = "<<Pk<<" in interation no. "<<k<<endl;

        if (Pk<0)
        {
            T1[k-1]=Rk;
        }
        else if (Pk>0)
        {
            T2[k-1]=Rk;
        }
        else
        {
            cout<<"The Line is Parallel"<<endl;
            break;
            return;
        }
    }

    t1 = maxOf(T1);
    t2 = minOf(T2);

    /*cout<<"T1 = ";
    for (int i = 0; i < 4; i++)
    {
        cout<<T1[i]<<" ";
    }
    cout<<endl;
    cout<<"T2 = ";
    for (int i = 0; i < 4; i++)
    {
        cout<<T2[i]<<" ";
    }
    cout<<endl;
    cout<<"t1 = "<<t1<<endl;
    cout<<"t2 = "<<t2<<endl;*/

    float xIntersect1 = x1 + (t1*(delX));
    float xIntersect2 = x1 + (t2*(delX));

    float yIntersect1 = y1 + (t1*(delY));
    float yIntersect2 = y1 + (t2*(delY));

    if (t1>t2)
    {
        //cout<<"Line Discarded"<<endl<<endl;
        return;
    }
    else
    {
    glBegin(GL_LINES);
        glColor3f(0,1,1);
        glVertex2f(xIntersect1,yIntersect1);
        glVertex2f(xIntersect2,yIntersect2);
    glEnd();
    glFlush();
    }
}

// Display function

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

void displayMain()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void display()
{
    if (currentScene==0)
    {
        displaySubWin1();
    }
    else if (currentScene==1)
    {
        drawNewWindow();
    }
}

void display2()
{
    if (currentScene==0)
    {
        displaySubWin2();
    }
    else if (currentScene==1)
    {
        drawNewWindow2();
    }
}

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the circle
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * PI * i / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawInfoButton1() 
{
    // Draw the top circle of the question mark
    glColor3f(1, 1, 1); // White color
    drawCircle(-30, 520, 5); // Draw circle at (0, 0.5) with radius 0.1

    // Draw the line of the question mark
    glBegin(GL_QUADS);
        glVertex2f(-27, 485);
        glVertex2f(-27, 510);
        glVertex2f(-33, 510);
        glVertex2f(-33, 485);
    glEnd();
}

void drawInfoButton2() 
{
    // Draw the top circle of the question mark
    glColor3f(0, 0, 0); // Black color
    drawCircle(710, 520, 5); // Draw circle at (0, 0.5) with radius 0.1

    // Draw the line of the question mark
    glBegin(GL_QUADS);
        glVertex2f(707, 485);
        glVertex2f(707, 510);
        glVertex2f(713, 510);
        glVertex2f(713, 485);
    glEnd();
}

void displaySubWin1()  //LIANG BARSKEY
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_QUADS);
        glColor3f(0,0,1);
        glVertex2f(-10,480);
        glVertex2f(-50,480);
        glVertex2f(-50,530);
        glVertex2f(-10,530);
    glEnd();

    char rule12[] = "Press I For Info";
    displaySentence(rule12, -90, 460);

    drawInfoButton1();
    if (isClipped)
    {
        glBegin(GL_POLYGON);
            glColor3f(0,1,0);
            glVertex2f(-760,490);
            glVertex2f(-760,500);
            glVertex2f(-770,500);
            glVertex2f(-770,490);
        glEnd();
        char Clipped[]="CLIPPING ON";
        displaySentence(Clipped,-750,491);
    }
    else
    {
        glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2f(-760,490);
        glVertex2f(-760,500);
        glVertex2f(-770,500);
        glVertex2f(-770,490);
        glEnd();

        char Clipped[]="CLIPPING OFF";
        displaySentence(Clipped,-750,491);
    }

    glColor3f(0.0, 0.0, 1.0);
    char Name1[] = "LIANG BARSKEY CLIPPING";
    displayMethodName(Name1, -550,520);

    glBegin(GL_LINES);
        glVertex2f(-550,515);
        glVertex2f(-245,515);
    glEnd();


    if (clickCount >= 2) 
    {
        // Draw clipping rectangle
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xMin, yMin);
        glVertex2f(xMax, yMin);
        glVertex2f(xMax, yMax);
        glVertex2f(xMin, yMax);
        glEnd();
    }

    if (clickCount >= 4) {
        // Draw the line before clipping
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_LINES);
        glVertex2f(X1, Y1);
        glVertex2f(X2, Y2);
        glEnd();
        
        // Perform clipping
        if (isClipped && !isDiscarded)
        {
            glBegin(GL_LINES);
                glColor3f(1,1,1);
                glVertex2f(X1, Y1);
                glVertex2f(X2, Y2);
            glEnd();
            LiangBarskey(X1,Y1,X2,Y2);
        }
    }

    glFlush();
}

void displaySubWin2()   //COHEN SUTHERLAND
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the window

    glBegin(GL_QUADS);
        glColor3f(0,1,1);
        glVertex2f(690,480);
        glVertex2f(730,480);
        glVertex2f(730,530);
        glVertex2f(690,530);
    glEnd();

    char rule12[] = "Press I For Info";
    displaySentence(rule12, 650, 460);

    drawInfoButton2();

    glColor3f(0,1,1);
    char Name1[] = "COHEN SUTHERLAND CLIPPING";
    displayMethodName(Name1, 200,520); // Display method name on the screen

    glBegin(GL_LINES);
        glVertex2f(200,515);
        glVertex2f(570,515);
    glEnd();


    if (isClipped)
    {
        glBegin(GL_POLYGON);
            glColor3f(0,1,0);
            glVertex2f(10,490);
            glVertex2f(10,500);
            glVertex2f(20,500);
            glVertex2f(20,490);
        glEnd();
        char Clipped[]="CLIPPING ON";
        displaySentence(Clipped,30,491);
    }
    else
    {
        glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2f(10,490);
        glVertex2f(10,500);
        glVertex2f(20,500);
        glVertex2f(20,490);
        glEnd();

        char Clipped[]="CLIPPING OFF";
        displaySentence(Clipped,30,491);
    }

    // If two clicks have been made, draw the clipping rectangle
    if (clickCount1 >= 2) 
    {
        // Draw the clipping window rectangle
        glColor3f(0.0, 1.0, 1.0); // Blue for the clipping rectangle
        glBegin(GL_LINE_LOOP);
        glVertex2f(xMinCS, yMinCS);
        glVertex2f(xMaxCS, yMinCS);
        glVertex2f(xMaxCS, yMaxCS);
        glVertex2f(xMinCS, yMaxCS);
        glEnd();
    }

    // If four clicks have been made (two for the rectangle and two for the line)
    if (clickCount1 >= 4) 
    {
        glColor3f(1.0, 1.0, 0.0); // White for the line (before clipping)
            glBegin(GL_LINES);
            glVertex2f(lineX1, lineY1);
            glVertex2f(lineX2, lineY2);
        glEnd();

        if (isClipped)
        {
        // Clip the line using Cohen-Sutherland algorithm
        cohenSutherlandClip(lineX1, lineY1, lineX2, lineY2);
        
        glColor3f(0.0, 0.0, 0.0); // White for the line (before clipping)
            glBegin(GL_LINES);
            glVertex2f(lineX1, lineY1);
            glVertex2f(lineX2, lineY2);
        glEnd();

        // If the line is successfully clipped, draw it
        if (outX1 != -1 && outY1 != -1 && outX2 != -1 && outY2 != -1) // Ensure the line is not rejected
        {
            // Now draw the clipped line
            glColor3f(1.0, 0.0, 0.0); // Green for the clipped line
            glBegin(GL_LINES);
            glVertex2f(outX1, outY1);
            glVertex2f(outX2, outY2);
            glEnd();
        }
        }
    }

    glFlush(); // Flush the drawing commands to the buffer
}

void drawNewWindow()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,0,1);

    char rule1[] = "1) The First 2 Left clicks record the size of the clipping window";
    char rule2[] = "2) Make sure that the first clicks coordinates are lesser than the second";
    char rule3[] = "3) Make sure to follow point no.2 while drawing the line";
    char rule4[] = "4) Press Space Bar to Clip the line";
    char rule5[] = "5) Press 'R' to Turn OFF Clipping";
    char rule6[] = "6) Press the WASD Keys & ARROW KEYS to move the LB & CS Clipping Windows respectively";
    char rule7[] = "7) Press the NUMPAD 1,3,7,9 Keys & QEZC Keys to move the CS and LB Clipping Window Diagonally respecctively";
    char rule8[] = "8) Click RMB to Clear the screen";
    char rule9[] = "9) Press the ULKJ Keys & THGF Keys to Rotate the LB & CS CLipping Windows respectively";
    char rule10[] = "10) Press '+' Button to Scale UP the Window";
    char rule11[] = "11) Press '-' & '/' Button to Scale DOWN the Window";
    char rule12[] = "12) Press I For Info";
    char rule13[] = "13) Press Y to go back to Clipping";
    char rule14[] = "14) Press Escape Button to Exit";

    displaySentence(rule1, -750, 150);
    displaySentence(rule2, -750, 135);
    displaySentence(rule3, -750, 120);
    displaySentence(rule4, -750, 105);
    displaySentence(rule5, -750, 90);
    displaySentence(rule6, -750, 75);
    displaySentence(rule7, -750, 60);
    displaySentence(rule8, -750, 45);
    displaySentence(rule9, -750, 30);
    displaySentence(rule10, -750, 15);
    displaySentence(rule11, -750, 0);
    displaySentence(rule12, -750, -15);
    displaySentence(rule13, -750, -30);
    displaySentence(rule14, -750, -45);
    
    glFlush();
}

void drawNewWindow2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,1,1);

    char rule1[] = "1) The First 2 Left clicks record the size of the clipping window";
    char rule2[] = "2) Make sure that the first clicks coordinates are lesser than the second";
    char rule3[] = "3) Make sure to follow point no.2 while drawing the line";
    char rule4[] = "4) Press Space Bar to Clip the line";
    char rule5[] = "5) Press 'R' to Turn OFF Clipping";
    char rule6[] = "6) Press the WASD Keys & ARROW KEYS to move the LB & CS Clipping Windows respectively";
    char rule7[] = "7) Press the NUMPAD 1,3,7,9 Keys & QEZC Keys to move the CS and LB Clipping Window Diagonally respecctively";
    char rule8[] = "8) Click RMB to Clear the screen";
    char rule9[] = "9) Press the ULKJ Keys & THGF Keys to Rotate the LB & CS CLipping Windows respectively";
    char rule10[] = "10) Press '+' Button to Scale UP the Window";
    char rule11[] = "11) Press '-' & '/' Button to Scale DOWN the Window";
    char rule12[] = "12) Press I For Info";
    char rule13[] = "13) Press Y to go back to Clipping";
    char rule14[] = "14) Press Escape Button to Exit";

    displaySentence(rule1, 50, 150);
    displaySentence(rule2, 50, 135);
    displaySentence(rule3, 50, 120);
    displaySentence(rule4, 50, 105);
    displaySentence(rule5, 50, 90);
    displaySentence(rule6, 50, 75);
    displaySentence(rule7, 50, 60);
    displaySentence(rule8, 50, 45);
    displaySentence(rule9, 50, 30);
    displaySentence(rule10,50, 15);
    displaySentence(rule11,50, 0);
    displaySentence(rule12,50, -15);
    displaySentence(rule13,50, -30);
    displaySentence(rule14,50, -45);
    
    glFlush();
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float X = (float)x - windowSizeX;
        float Y = windowSizeY - (float)y; // Convert y-coordinate to match OpenGL coordinate system

        //cout<<"X = "<<X<<endl;
        //cout<<"Y = "<<Y<<endl;

        if (clickCount == 0) {
            // First click: set xMin, yMin
            xMin = X;
            yMin = Y;
            clickCount++;
        } else if (clickCount == 1) {
            // Second click: set xMax, yMax
            xMax = X;
            yMax = Y;
            clickCount++;
        } else if (clickCount == 2) {
            // Third click: set X1, Y1
            X1 = X;
            Y1 = Y;
            clickCount++;
        } else if (clickCount == 3) {
            // Fourth click: set X2, Y2
            X2 = X;
            Y2 = Y;
            clickCount++;
        }

        glutPostRedisplay();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        clickCount=0;
        isDiscarded=false;
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        glutPostRedisplay();
    }
}

// Keyboard input function
void keyboard(unsigned char key, int x, int y) 
{
    if (key == ' ') 
    {
        isClipped = true;
        animateClipping = true;
        animT1=t1; // Reset animation variables
        animT2=t2;
        glutTimerFunc(1000/60, animate, 0); // Start animation
        
    } 
    else if (key == 'r') {
        isClipped = false;
        animateClipping = false;
        isDiscarded=false;
        glutPostRedisplay();        // Redraw to remove clipping
    }

    //MOVEMENT OF WINDOW

    else if (key == 'w') {
        yMin+=1;
        yMax+=1;
        glutPostRedisplay();        // Redraw to remove clipping
    }
    else if (key == 's') 
    {
        yMin-=1;
        yMax-=1;
        glutPostRedisplay();        
    }
    else if (key == 'd') 
    {
        xMin+=1;
        xMax+=1;
        glutPostRedisplay();        
    }
    else if (key == 'a') 
    {
        xMin-=1;
        xMax-=1;
        glutPostRedisplay();        
    }

    // Diagonal movements are Below

    else if (key == 'q') {
        yMin+=1;
        yMax+=1;
        xMin-=1;
        xMax-=1;
        glutPostRedisplay();        
    }
    else if (key == 'e') {
        yMin+=1;
        yMax+=1;
        xMin+=1;
        xMax+=1;
        glutPostRedisplay();        
    }
    else if (key == 'z') {
        yMin-=1;
        yMax-=1;
        xMin-=1;
        xMax-=1;
        glutPostRedisplay();        
    }
    else if (key == 'c') 
    {
        yMin-=1;
        yMax-=1;
        xMin+=1;
        xMax+=1;
        glutPostRedisplay();        
    }

    //SCALING THE WINDOW

    else if(key=='+')               // Scaling Up the Window
    {
        yMin-=1;
        yMax+=1;
        xMin-=1;
        xMax+=1;

        yMinCS-=1;
        yMaxCS+=1;
        xMinCS-=1;
        xMaxCS+=1;
        glutPostRedisplay();
    }

    else if(key=='-')               // Scaling Down the Window of LIANG BARSKEY
    {
        const int minSize = 1;

        if (xMax - xMin <= minSize && yMax - yMin <= minSize ) 
        {
            //cout << "Window has reached minimum size" << endl;
            glutPostRedisplay();
            return;
        }

        // Case where the window is collapsed along x-axis but not y-axis
        else if (xMax - xMin <= minSize && yMax - yMin > minSize) 
        {
            yMin += 1;
            yMax -= 1;
        }

        // Case where the window is collapsed along y-axis but not x-axis
        else if (yMax - yMin <= minSize && xMax - xMin > minSize) 
        {
            xMin += 1;
            xMax -= 1;
        }

        // Normal scaling case where both x and y dimensions are still larger than the minimum size
        else {
            yMin += 1;
            yMax -= 1;
            xMin += 1;
            xMax -= 1;
        }

        glutPostRedisplay();
    }

    else if(key=='/')               // Scaling Down the Window of COHEN SUTHERLAND
    {
        const int minSize = 1;

        if (xMaxCS - xMinCS <= minSize && yMaxCS - yMinCS <= minSize)
        {
            //cout << "Window has reached minimum size" << endl;
            glutPostRedisplay();
            return;
        }

        else if (xMaxCS - xMinCS <= minSize && yMaxCS - yMinCS > minSize) 
        {
            yMinCS += 1;
            yMaxCS -= 1;
        }
        
        else if (yMaxCS - yMinCS <= minSize && xMaxCS - xMinCS > minSize) 
        {
            xMinCS += 1;
            xMaxCS -= 1;
        }

        // Normal scaling case where both x and y dimensions are still larger than the minimum size
        else {
            yMinCS += 1;
            yMaxCS -= 1;
            xMinCS += 1;
            xMaxCS -= 1;
        }

        glutPostRedisplay();
    }

    //ROTATING LIANG BARSKEY WINDOW

    else if (key=='l')               // Rotating the Window on y-axis (Clockwise)
    {
        float cX = (xMin+xMax)/2.0;
        float cY = (yMin+yMax)/2.0;

        float radians = 1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1 = xMin - cX;
        float y1 = yMin - cY;
        float x2 = xMax - cX;
        float y2 = yMax - cY;


        xMin = cX + (x1 * cos(radians) - y1 * sin(radians));
        xMax = cX + (x2 * cos(radians) - y2 * sin(radians));
        //yMin = cY + (x1 * sin(radians) + y1 * cos(radians));
        //yMax = cY + (x2 * sin(radians) + y2 * cos(radians));

        glutPostRedisplay();
    }
    else if (key=='j')              // Rotating the Window on y-axis (Anti-Clockwise)
    {
        float cX = (xMin+xMax)/2.0;
        float cY = (yMin+yMax)/2.0;

        float radians = -1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1 = xMin - cX;
        float y1 = yMin - cY;
        float x2 = xMax - cX;
        float y2 = yMax - cY;

        xMin = cX + (x1 * cos(radians) - y1 * sin(radians));
        xMax = cX + (x2 * cos(radians) - y2 * sin(radians));
        //yMin = cY + (x1 * sin(radians) + y1 * cos(radians));
        //yMax = cY + (x2 * sin(radians) + y2 * cos(radians));

        glutPostRedisplay();
    }
    else if (key=='u')              // Rotating the Window on x-axis (Clockwise)
    {
        float cX = (xMin+xMax)/2.0;
        float cY = (yMin+yMax)/2.0;

        float radians = 1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1 = xMin - cX;
        float y1 = yMin - cY;
        float x2 = xMax - cX;
        float y2 = yMax - cY;

        //xMin = cX + (x1 * cos(radians) - y1 * sin(radians));
        //xMax = cX + (x2 * cos(radians) - y2 * sin(radians));
        yMin = cY + (x1 * sin(radians) + y1 * cos(radians));
        yMax = cY + (x2 * sin(radians) + y2 * cos(radians));

        glutPostRedisplay();
    }
    else if (key=='k')              // Rotating the Window on x-axis (Anti-Clockwise)
    {
        float cX = (xMin+xMax)/2.0;
        float cY = (yMin+yMax)/2.0;

        float radians = -1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1 = xMin - cX;
        float y1 = yMin - cY;
        float x2 = xMax - cX;
        float y2 = yMax - cY;

        //xMin = cX + (x1 * cos(radians) - y1 * sin(radians));
        //xMax = cX + (x2 * cos(radians) - y2 * sin(radians));
        yMin = cY + (x1 * sin(radians) + y1 * cos(radians));
        yMax = cY + (x2 * sin(radians) + y2 * cos(radians));

        glutPostRedisplay();
    }

    //ROTATING COHEN SUTHERLAND WINDOW

    else if(key == 'h')               // Rotating the Window on y-axis (Clockwise)
    {
        float cXCS = (xMinCS+xMaxCS)/2;
        float cYCS = (yMinCS+yMaxCS)/2;

        float radians = 1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1CS = xMinCS - cXCS;
        float y1CS = yMinCS - cYCS;
        float x2CS = xMaxCS - cXCS;
        float y2CS = yMaxCS - cYCS;


        xMinCS = cXCS + (x1CS * cos(radians) - y1CS * sin(radians));
        xMaxCS = cXCS + (x2CS * cos(radians) - y2CS * sin(radians));
        //yMinCS = cYCS + (x1CS * sin(radians) + y1CS * cos(radians));
        //yMaxCS = cYCS + (x2CS * sin(radians) + y2CS * cos(radians));

        glutPostRedisplay();
    }

    else if(key == 'f')               // Rotating the Window on y-axis (Clockwise)
    {
        float cXCS = (xMinCS+xMaxCS)/2;
        float cYCS = (yMinCS+yMaxCS)/2;

        float radians = -1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1CS = xMinCS - cXCS;
        float y1CS = yMinCS - cYCS;
        float x2CS = xMaxCS - cXCS;
        float y2CS = yMaxCS - cYCS;


        xMinCS = cXCS + (x1CS * cos(radians) - y1CS * sin(radians));
        xMaxCS = cXCS + (x2CS * cos(radians) - y2CS * sin(radians));
        //yMinCS = cYCS + (x1CS * sin(radians) + y1CS * cos(radians));
        //yMaxCS = cYCS + (x2CS * sin(radians) + y2CS * cos(radians));

        glutPostRedisplay();
    }

    else if(key == 't')               // Rotating the Window on y-axis (Clockwise)
    {
        float cXCS = (xMinCS+xMaxCS)/2;
        float cYCS = (yMinCS+yMaxCS)/2;

        float radians = 1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1CS = xMinCS - cXCS;
        float y1CS = yMinCS - cYCS;
        float x2CS = xMaxCS - cXCS;
        float y2CS = yMaxCS - cYCS;


        //xMinCS = cXCS + (x1CS * cos(radians) - y1CS * sin(radians));
        //xMaxCS = cXCS + (x2CS * cos(radians) - y2CS * sin(radians));
        yMinCS = cYCS + (x1CS * sin(radians) + y1CS * cos(radians));
        yMaxCS = cYCS + (x2CS * sin(radians) + y2CS * cos(radians));

        glutPostRedisplay();
    }

    else if(key == 'g')               // Rotating the Window on y-axis (Clockwise)
    {
        float cXCS = (xMinCS+xMaxCS)/2;
        float cYCS = (yMinCS+yMaxCS)/2;

        float radians = -1.0 * (PI/180);

        //cout<<"Radian angle = "<<radians;

        float x1CS = xMinCS - cXCS;
        float y1CS = yMinCS - cYCS;
        float x2CS = xMaxCS - cXCS;
        float y2CS = yMaxCS - cYCS;


        //xMinCS = cXCS + (x1CS * cos(radians) - y1CS * sin(radians));
        //xMaxCS = cXCS + (x2CS * cos(radians) - y2CS * sin(radians));
        yMinCS = cYCS + (x1CS * sin(radians) + y1CS * cos(radians));
        yMaxCS = cYCS + (x2CS * sin(radians) + y2CS * cos(radians));

        glutPostRedisplay();
    }

    else if (key=='i')
    {
        currentScene=1;
        glutPostRedisplay();
    }

    else if (key=='y')
    {
        currentScene=0;
        glutPostRedisplay();
    }

    else if(key == 27)              // Escape to EXIT
    {
        exit(0);
    }
}

void SpecialKeyboardFunc(int key, int x, int y)
{
    switch (key) 
    {
        case GLUT_KEY_UP:
            yMinCS += 1;
            yMaxCS += 1;
            break;
        case GLUT_KEY_DOWN:
            yMinCS -= 1;
            yMaxCS -= 1;
            break;
        case GLUT_KEY_RIGHT:
            xMinCS += 1;
            xMaxCS += 1;
            break;
        case GLUT_KEY_LEFT:
            xMinCS -= 1;
            xMaxCS -= 1;
            break;
        case GLUT_KEY_END:        // NUMPAD 1
            yMinCS-=1;
            yMaxCS-=1;
            xMinCS-=1;
            xMaxCS-=1;
            break;
        case GLUT_KEY_PAGE_DOWN:  // NUMPAD 3
            yMinCS-=1;
            yMaxCS-=1;
            xMinCS+=1;
            xMaxCS+=1;
            break;
        case GLUT_KEY_PAGE_UP:    // NUMPAD 9
            yMinCS+=1;
            yMaxCS+=1;
            xMinCS+=1;
            xMaxCS+=1;
            break;
        case GLUT_KEY_HOME:       // NUMPAD 7
            yMinCS+=1;
            yMaxCS+=1;
            xMinCS-=1;
            xMaxCS-=1;
            break;
        
    }
    glutPostRedisplay();  // Redraw to remove clipping
}


// Animation function called by the timer
void animate(int value) 
{
    if (animateClipping) 
    {
        if (animT1 < 1.0) 
        {
            animT1 += 0.05; // Incrementally increase t1 to animate clipping
        } else if (animT2 > 0.0) 
        {
            animT2 -= 0.05; // Incrementally decrease t2 to animate clipping
        } else 
        {
            animateClipping = false; // Stop animation when fully clipped
        }

        glutPostRedisplay();

        glutTimerFunc(1000/60, animate, 0); // Re-call the animation after 50ms
    }
}


// Mouse click callback function
void mouseClick2(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert mouse coordinates to OpenGL coordinates
        float normalizedX = (float)x;
        float normalizedY = windowSizeY - (float)y; // Invert Y coordinate


        if (clickCount1 == 0) {
            // First click, set the first corner of the clipping window
            xMinCS = normalizedX;
            yMinCS = normalizedY;
            cout<<"X="<<xMinCS<<" Y="<<yMinCS<<endl;
            clickCount1++;
        } else if (clickCount1 == 1) {
            // Second click, set the second corner of the clipping window
            xMaxCS = normalizedX;
            yMaxCS = normalizedY;
            //cout<<"X="<<xMaxCS<<" Y="<<yMaxCS<<endl;
            clickCount1++;
        } else if (clickCount1 == 2) {
            // First line point
            lineX1 = normalizedX;
            lineY1 = normalizedY;
            //cout<<"X1="<<lineX1<<" Y1="<<lineY1<<endl;
            clickCount1++;
        } else if (clickCount1 == 3) {
            // Second line point; clip the line
            lineX2 = normalizedX;
            lineY2 = normalizedY;
            //cout<<"X2="<<lineX2<<" Y2="<<lineY2<<endl;
            clickCount1++;
        }

        glutPostRedisplay();
    }

    if(button== GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        // Reset click count for the next operation
        clickCount1 = 0; // Reset click count
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        glutPostRedisplay();
    }
}


// Initialization function
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-windowSizeX, windowSizeX, -windowSizeY, windowSizeY);
}

void init1() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
}

void init2() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
}

// Main function
int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(2*windowSizeX, 2*windowSizeY);
    int windowMain = glutCreateWindow("Line Clipping");
    glutDisplayFunc(displayMain);
    glutFullScreen();
    init();
    

    // Initialinzing SubWindow1 (COHEN SUTHERLAND)
    int subWin1 = glutCreateSubWindow(windowMain, windowSizeX, 0, windowSizeX, 2 * windowSizeY);
    gluOrtho2D(0, windowSizeX, -windowSizeY, windowSizeY);
    glutDisplayFunc(display2);
    glutMouseFunc(mouseClick2);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKeyboardFunc);
    init2();

    // Initialinzing SubWindow2 (LIANG BARSKEY)
    int subWin2 = glutCreateSubWindow(windowMain, 0, 0, windowSizeX, 2 * windowSizeY);
    gluOrtho2D(-windowSizeX, 0, -windowSizeY, windowSizeY);
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    init1();

    glutMainLoop();
    return 0;
}
