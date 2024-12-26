#include<freeglut.h>
#include<math.h>

float arrowX = -8.0f;  // Initial x-position of the arrow
bool isShooting = false; //Initially player is not shooting nor stretching
bool isStretching = false;
bool reload = false;

float cloudX = -10.0f;  // Initial X position of the cloud
float cloudSpeed = 0.01f;  // Speed of cloud movement

// Function to draw the bow
void drawBow() {
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINE_STRIP);
    for (float angle = 3.14/2; angle <= 3*(3.14f)/2; angle += 0.1f) {
        float x = -7.25f + 1.0f * -cos(angle);
        float y = 1.0f * -sin(angle);
        glVertex2f(x,y);
    }
    glEnd();
}

// Function to draw the Stretch
void drawStretch()
{
    glBegin(GL_LINES);
        glVertex2f(-7,1);
        glVertex2f(-8,0);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(-7,-1);
        glVertex2f(-8,0);
    glEnd();
}

// Function to draw the arrow
void drawArrow() {
    glBegin(GL_LINES);
    glVertex2f(arrowX, 0.0f);
    glVertex2f(arrowX + 2.0f, 0.0f);
    glEnd();

    // Arrowhead
    glBegin(GL_TRIANGLES);
    glVertex2f(arrowX + 2.0f, 0.0f);
    glVertex2f(arrowX + 1.8f, 0.2f);
    glVertex2f(arrowX + 1.8f, -0.2f);
    glEnd();
}

void drawCircle(float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN); //Triangle fan isiliye use kar rahe hai kyoki a circle is composed of small segments of triangle shape
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14 * i / segments;
        float x = 8 + radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw the archery board & Stand
void drawArcheryBoard() {
    // Colors for the circles
    float colors[5][3] = {
        {1.0f, 1.0f, 1.0f},  // White
        {0.0f, 0.0f, 0.0f},  // Black
        {0.0f, 0.0f, 1.0f},  // Blue
        {1.0f, 0.0f, 0.0f},  // Red
        {1.0f, 1.0f, 0.0f}   // Yellow
    };

    // Radii for the circles
    float radii[5] = {1.0f, 0.8f, 0.6f, 0.4f, 0.2f};

    // Draw each circle with appropriate color
    for (int i = 0; i < 5; i++) {
        glColor3fv(colors[i]);
        drawCircle(radii[i], 100);
    }
}

void archeryBoardStand()
{
    glBegin(GL_LINES);
        glColor3f(0,0,0);
        glVertex2f(7.5f,-2.0f);
        glVertex2f(8.0f,0.0f);
        glVertex2f(8.0f,0.0f);
        glVertex2f(8.5f,-2.0f);
    glEnd();
}

void Background()
{
    //GROUND
    glBegin(GL_POLYGON);
        glColor3f(0,1,0);
        glVertex2f(-10,-10);
        glVertex2f(-10,-2);
        glVertex2f(10,-2);
        glVertex2f(10,-10);
    glEnd();

    //GRASS
    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(-3.5,-7.5);
        glVertex2f(-2.5,-8.5);
        glVertex2f(-2.5,-7.5);
        glVertex2f(-2.5,-8.5);
        glVertex2f(-1.5,-7.5);
        glVertex2f(-2.5,-8.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(-8.5,-7.5);
        glVertex2f(-7.5,-8.5);
        glVertex2f(-7.5,-7.5);
        glVertex2f(-7.5,-8.5);
        glVertex2f(-6.5,-7.5);
        glVertex2f(-7.5,-8.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(3.5,-7.5);
        glVertex2f(2.5,-8.5);
        glVertex2f(2.5,-7.5);
        glVertex2f(2.5,-8.5);
        glVertex2f(1.5,-7.5);
        glVertex2f(2.5,-8.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(8.5,-7.5);
        glVertex2f(7.5,-8.5);
        glVertex2f(7.5,-7.5);
        glVertex2f(7.5,-8.5);
        glVertex2f(6.5,-7.5);
        glVertex2f(7.5,-8.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(-3.5,-4.5);
        glVertex2f(-2.5,-5.5);
        glVertex2f(-2.5,-4.5);
        glVertex2f(-2.5,-5.5);
        glVertex2f(-1.5,-4.5);
        glVertex2f(-2.5,-5.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(-8.5,-4.5);
        glVertex2f(-7.5,-5.5);
        glVertex2f(-7.5,-4.5);
        glVertex2f(-7.5,-5.5);
        glVertex2f(-6.5,-4.5);
        glVertex2f(-7.5,-5.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(3.5,-4.5);
        glVertex2f(2.5,-5.5);
        glVertex2f(2.5,-4.5);
        glVertex2f(2.5,-5.5);
        glVertex2f(1.5,-4.5);
        glVertex2f(2.5,-5.5);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0,0.5,0);
        glVertex2f(8.5,-4.5);
        glVertex2f(7.5,-5.5);
        glVertex2f(7.5,-4.5);
        glVertex2f(7.5,-5.5);
        glVertex2f(6.5,-4.5);
        glVertex2f(7.5,-5.5);
    glEnd();
}

void drawCloudCircle(float x, float y, float r, float s)
{
    //CLOUD
    glColor3f(1.0f, 1.0f, 1.0f);  // Cloud color (white)
    glBegin(GL_TRIANGLE_FAN);
    //Triangle fan isiliye use kar rahe hai kyoki a circle is composed of triangle shaped very small segments
    for (int i = 0; i <= s; i++) {
        float angle = 2.0f * 3.14 * i / s;
        float x1 = r * cosf(angle);
        float y1 = 8 + r * sinf(angle);
        glVertex2f(x+x1, y+y1);
    }
    glEnd();
}

// Display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    //CLOUD 1
    drawCloudCircle(cloudX-9,-1,1,100);
    drawCloudCircle(cloudX-8,-1,1,100);
    drawCloudCircle(cloudX-7.5,0,1,100);
    drawCloudCircle(cloudX-7,-1,1,100);
    drawCloudCircle(cloudX-6,-1,1,100);

    //CLOUD 2
    drawCloudCircle(cloudX+1.5,0,1,100);
    drawCloudCircle(cloudX+0.5f,0,1,100);
    drawCloudCircle(cloudX,1,1,100);
    drawCloudCircle(cloudX-0.5f,0,1,100);
    drawCloudCircle(cloudX-1.5,0,1,100);

    //CLOUD 3
    drawCloudCircle(cloudX+9,-1,1,100);
    drawCloudCircle(cloudX+8,-1,1,100);
    drawCloudCircle(cloudX+7.5,0,1,100);
    drawCloudCircle(cloudX+7,-1,1,100);
    drawCloudCircle(cloudX+6,-1,1,100);

    Background();

    archeryBoardStand();
    drawArcheryBoard();
    drawBow();
    drawArrow();

    if (isStretching)
    {
        drawStretch();
    }

    glColor4f(0.0f,0.0f,1.0f,1.0f);

    glRasterPos2f(-0.1,-9.75);

    char s[] = "Made By Aryan Dubey [G&G(Batch-2)]";
    void * font = GLUT_BITMAP_TIMES_ROMAN_24;
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


// Timer function to update the arrow position
void timer(int) {

    if (isShooting) {
        isStretching=false;
        arrowX += 0.1f;  // Moving arrow to the right
        if (arrowX > 6.0f) {
            isShooting = false;  // Stop shooting once the arrow is off screen
        }
    }
    if (reload)
    {
        arrowX = -8.0f;  // Reset arrow position
        reload=false;
    }
    glutPostRedisplay();  // Redisplay the scene
    glutTimerFunc(1000/60, timer, 0);  // 1000/60 ~= 16ms timer for ~60fps
}

void timer1(int)
{
    cloudX += cloudSpeed; // Moving the cloud

    
    if (cloudX > 10.0f) 
    {
        cloudX = -10.0f; // Reset cloud position when it moves out of the screen
    }

    // Redraw the scene
    glutPostRedisplay();

    // Call update again after 16 milliseconds (~60 FPS)
    glutTimerFunc(1000/60, timer1, 0);
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ')
    {
        if (isStretching)
        {
            isShooting = true;  // Start shooting when spacebar is pressed
        }
    }
    if (key == 's')
    {
        isStretching = true; //Start the Stretch
    }
    if (key =='r')
    {
        if (!isShooting)
        {
            reload = true; //Reload the Bow
        }
    }
}

// Initialization function
void init() {
    glClearColor(0.7f, 1.0f, 1.0f, 1.0f); //cyan
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(350, 0);
    glutCreateWindow("Archery Animation");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, timer1, 0);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
