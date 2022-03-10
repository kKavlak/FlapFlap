// A re-creation of the popular video game 'Flappy Bird' in C

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

#define TIMER_PERIOD  20
#define TIMER_ON        1 
#define D2R 0.0174532

int  winWidth, winHeight, tanim, tcnt = 0;
bool up = false, down = false, right = false, left = false,
sb = false, t = true, birdlife = true, randflag = true;

int birdycor, pipexcor = 50, gapycor;

int points = 0;

void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

int gap() {
    if (randflag) {
        int r;
        r = rand() % 200 - 100;
        randflag = false;
        return r;
    }
}

void drawbird(int x, int y, bool key, int var, bool life)
{
    glColor3f(1, 1, 0);
    circle(x, y, 15);
    glRectf(x - 20, y + 15, x, y - 15);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 20, y + 15);
    glVertex2f(x - 40, y + 15);
    glVertex2f(x - 20, y - 15);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 20, y + 15);
    glVertex2f(x - 40, y - 15);
    glVertex2f(x - 20, y - 15);
    glEnd();
    glColor3f(1, 0.3, 0.3);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 10, y);
    glVertex2f(x + 30, y);
    glVertex2f(x + 10, y - 5);
    glEnd();

    if (life) {
        glColor3f(1, 1, 1);
        circle(x + 2, y + 4, 5);
    }
    glColor3f(0, 0, 0);
    circle(x + 2, y + 4, 2);

    glLineWidth(2);
    glColor3f(1, 0.3, 0.3);
    glBegin(GL_LINES);
    glVertex2f(x - 15, y - 15);
    glVertex2f(x - 17, y - 20);
    glVertex2f(x - 10, y - 20);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x - 17, y - 20);
    glVertex2f(x - 10, y - 20);
    glEnd();
    glLineWidth(1);

    if (key && life) {
        switch (var) {
        case 0:
        case 1:
        case 2:
            glColor3f(0.6, 0.6, 0);
            circle(x - 16, y, 7);
            glColor3f(1, 1, 0);
            circle(x - 16, y + 4, 8);
            break;
        case 3:
        case 4:
        case 5:
            glColor3f(1, 1, 0);
            glRectf(x - 23, y + 15, x - 9, y + 25);
            circle(x - 16, y + 25, 7);
            break;
        }
    }

}

void drawPipe(int x)
{
    glColor3f(0, 0.7, 0);
    glRectf(x - 15, winHeight / 2 - 20, x + 15, -winHeight / 2 + 20);
}

void drawGap(int x, int y)
{
    glColor3f(0.5, 0.5, 1);
    glRectf(x - 15, y + 60, x + 15, y - 60);
}

void display() {

    glClearColor(0.5, 0.5, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    drawPipe(pipexcor);
    drawGap(pipexcor, gapycor);

    drawbird(-150, birdycor, sb, tanim, birdlife);
    if (!birdlife)
    {
        glColor3f(1, 0, 0);
        vprint(-75, 75, GLUT_BITMAP_8_BY_13, "You Lost! <F1> to restart");
        vprint(-75, 25, GLUT_BITMAP_8_BY_13, "Your Score: %d", points);
    }

    glColor3f(0, 0, 0);
    vprint(-50, 147, GLUT_BITMAP_8_BY_13, "<Spacebar>: Flap wings");
    vprint(-50, 160, GLUT_BITMAP_8_BY_13, "<F1>: Restart <F2>: Pause", points);

    glColor3f(0.5, 0.3, 0);
    glRectf(-winWidth / 2, -winHeight / 2, winWidth / 2, -winHeight / 2 + 20);
    glRectf(-winWidth / 2, winHeight / 2, winWidth / 2, winHeight / 2 - 20);


    glutSwapBuffers();
}


void onKeyDown(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
    if (key == 32)
    {
        sb = true;
    }
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
    if (key == 32)
    {
        sb = false;
    }
    glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1:
        t = true;
        birdlife = true;
        birdycor = 35;
        gapycor = 0;
        pipexcor = winWidth / 2;
        points = 0;
        break;
    case GLUT_KEY_F2:
        t = !t;
        break;
    }

    glutPostRedisplay();
}


void onSpecialKeyUp(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    glutPostRedisplay();
}


void onClick(int button, int stat, int x, int y)
{



    glutPostRedisplay();
}


void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display();
}

void onMoveDown(int x, int y) {



    glutPostRedisplay();
}


void onMove(int x, int y) {



    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    if (t == 1) {
        tcnt++;
        tanim = tcnt % 6;
        pipexcor -= 6;

        if (sb)
        {
            birdycor += 6;
        }
        else
        {
            birdycor -= 6;
        }


        if (birdycor < -157 || birdycor > 160)
        {
            t = false;
            birdlife = false;
        }

        if (pipexcor < -winWidth / 2) {
            pipexcor = winWidth / 2;
            randflag = true;
            gapycor = gap();
            points++;
        }

        if (abs(pipexcor + 150) < 20)
        {
            if (birdycor + 15 > gapycor + 60 || birdycor - 15 < gapycor - 60) {
                birdlife = false;
                t = false;
            }
        }
    }
    glutPostRedisplay();

}
#endif

void Init() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("FLAPPY BIRD?");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);


    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);


    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}