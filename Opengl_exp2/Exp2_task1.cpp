#include <GL/glut.h>
#include <Windows.h>
#include <cmath>
#define M_PI 3.1415926535
float t = 0.0; // ���ڿ�����ɫ�仯��ʱ�����
float angle = 0.0;
float scale = 1.0;
void myinit(void)
{
    /* attributes */
    glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */
    /* set up viewing */
    /* 50.0 �� 50.0 camera coordinate window with origin lower left */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, 0.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void updateColor()
{
    float frequency = 2.0 * M_PI / 5.0; // ���һ������ɫѭ����ʱ��Ϊ5��

    float red = sin(frequency * t + 0) * 0.5 + 0.5;
    float green = sin(frequency * t + 2 * M_PI / 3) * 0.5 + 0.5;
    float blue = sin(frequency * t + 4 * M_PI / 3) * 0.5 + 0.5;

    glColor3f(red, green, blue);
}

void display(void)
{
    /* A triangle */
    GLfloat vertices[3][2] = { {0.0,0.0},{25.0,50.0},{50.0,0.0} };

    int i, j, k;
    int rand();       /* standard random number generator */
    GLfloat p[2] = { 7.5,5.0 };  /* an arbitrary initial point inside
                              traingle */
    glClear(GL_COLOR_BUFFER_BIT);  /* clear the window */
    glPushMatrix();
    glTranslatef(25.0f, 25.0f, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, 1.0); // �������Ч��
    glTranslatef(-25.0f, -25.0f, 0.0f);
    updateColor();
    glBegin(GL_POINTS);
    /* compute and plots 20000 new points */
    for (k = 0; k < 20000; k++)
    {
        j = rand() % 3; /* pick a vertex at random */
        /* Compute point halfway between selected vertex and old point */
        p[0] = (p[0] + vertices[j][0]) / 2.0;
        p[1] = (p[1] + vertices[j][1]) / 2.0;

        /* plot new point */
        glVertex2fv(p);

    }
    glEnd();
    glPopMatrix();
    glFlush(); /* clear buffers */
}

void timer(int value)
{
    t += 0.01; // ������ɫ�仯�ٶȣ����Ը�����Ҫ����
    if (t > 5.0) t -= 5.0; // ����t���Ա���ѭ��
    angle += 1.0f;
    if (angle > 360.0)angle -= 360.0;
    //updateColor();
    if (angle <= 180.0) {
        scale = 0.5 + 0.5 * angle / 180.0;
    }else
    {
        scale = 0.5 + 0.5 * (360.0 - angle) / 180.0;
    }
    glutPostRedisplay(); // ���»�����Ļ
    glutTimerFunc(16, timer, 0); // ÿ100�������һ��
}

int main(int argc, char** argv)
{
    /* Standard GLUT initialization */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(500, 500); /* 500 �� 500 pixel window */
    glutInitWindowPosition(0, 0); /* place window top left on display */
    glutCreateWindow("Sierpinski Gasket"); /* window title */
    myinit(); /* set attributes */
    glutDisplayFunc(display); /* display callback invoked when window opened */
    glutTimerFunc(16, timer, 0); // ���ö�ʱ���Կ�����ɫ�仯
    glutMainLoop(); /* enter event loop */
    return 0;
}
