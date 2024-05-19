#include <GL/glut.h>
#include <cmath>
#include <chrono>
#define DEG_TO_RAD 0.017453
const GLfloat RR = 2.0; //camera rad
GLfloat beta = 0;
GLfloat rotationAngle = 0;
float x = RR * sin(beta * DEG_TO_RAD);
float y = RR * cos(beta * DEG_TO_RAD) * cos(45.0);
float z = RR * cos(beta * DEG_TO_RAD) * sin(45);
GLfloat cameraPos[3] = { x,y,z };
float lx = -x, ly = -y, lz = -z;
GLfloat cameraFront[3] = { lx,ly,lz };
GLfloat cameraUp[3] = { 0.0,1.0,0.0 };
// Light and material properties
GLfloat matAmbient[] = {0.329412, 0.223529, 0.027451, 1.0};
GLfloat matDiffuse[] = {0.780392, 0.568627, 0.113725, 1.0};
GLfloat matSpecular[] = {0.992157, 0.941176, 0.807843, 1.0};
GLfloat matShininess[] = {27.8974};
GLfloat lightPos[] = {1.0, 1.0, 1.0, 0.0};
GLfloat lightAmbient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat lightDiffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0};
void init() {
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}
void setLight() {
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}
void setMaterial() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

void drawCube() {
    glBegin(GL_TRIANGLES);
    // face A, part 1
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, -1);
    //glEnd();

    // face A, part 2
    glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0, 0);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, 1, 1);
    glEnd();
    // face B, part 1
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 0);
    glVertex3f(1, 1, -1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, -1);
    glEnd();
    // face B, part 2
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 0.5);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1);
    glEnd();
    // face C
    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(-1, -1, 1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0.5, 0);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);
    glEnd();
    // face D
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, 1, -1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 0.5);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
    glEnd();
    // face E
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(-1, -1, -1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0, 0.5);
    glVertex3f(-1, 1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
    glEnd();
    //face F
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 1);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
        cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2],//注视目标位置
        cameraUp[0], cameraUp[1], cameraUp[2]);
    setLight();
    setMaterial();
    glPushMatrix();
    glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    drawCube();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idlefunc() {
    rotationAngle+=0.05f;
    if (rotationAngle > 360)
        rotationAngle -= 360;
    glutPostRedisplay();
}
// Rainbow colors
GLfloat rainbowColors[][4] = {
    {1.0, 0.0, 0.0, 1.0}, // Red
    {1.0, 0.5, 0.0, 1.0}, // Orange
    {1.0, 1.0, 0.0, 1.0}, // Yellow
    {0.0, 1.0, 0.0, 1.0}, // Green
    {0.0, 0.0, 1.0, 1.0}  // Blue
};

int currentColorIndex = 0;
auto lastColorUpdateTime = std::chrono::high_resolution_clock::now();
void newidlefunc() {
    rotationAngle += 0.05;
    if (rotationAngle > 360)
        rotationAngle -= 360;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastColorUpdateTime).count();

    if (duration >= 100) { // Update every 16 milliseconds
        currentColorIndex = (currentColorIndex + 1) % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));
        memcpy(lightDiffuse, rainbowColors[currentColorIndex], sizeof(rainbowColors[currentColorIndex]));
        lastColorUpdateTime = currentTime;
    }

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'o':
        memcpy(lightAmbient, new GLfloat[4]{ 0.2, 0.2, 0.2, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(lightDiffuse, new GLfloat[4]{ 0.8, 0.8, 0.8, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(lightSpecular, new GLfloat[4]{ 1.0, 1.0, 1.0, 1.0 }, sizeof(GLfloat) * 4);
        glutIdleFunc(idlefunc);
        break;
    case'p':
        currentColorIndex = 0;
        glutIdleFunc(newidlefunc);
        
        break;
    case 'b': // Yellow Brass Material
        memcpy(matAmbient, new GLfloat[4]{ 0.329412, 0.223529, 0.027451, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matDiffuse, new GLfloat[4]{ 0.780392, 0.568627, 0.113725, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matSpecular, new GLfloat[4]{ 0.992157, 0.941176, 0.807843, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matShininess, new GLfloat[1]{ 27.8974 }, sizeof(GLfloat));
        break;
    case 'n': // Red Plastic Material
        memcpy(matAmbient, new GLfloat[4]{ 0.3f, 0.0f, 0.0f, 1.0f }, sizeof(GLfloat) * 4);
        memcpy(matDiffuse, new GLfloat[4]{ 0.6f, 0.0f, 0.0f, 1.0f }, sizeof(GLfloat) * 4);
        memcpy(matSpecular, new GLfloat[4]{ 0.8f, 0.6f, 0.6f, 1.0f }, sizeof(GLfloat) * 4);
        memcpy(matShininess, new GLfloat[1]{ 32.0f }, sizeof(GLfloat));
        break;
    case 'm': // White Shiny Material
        memcpy(matAmbient, new GLfloat[4]{ 1.0, 1.0, 1.0, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matDiffuse, new GLfloat[4]{ 1.0, 1.0, 1.0, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matSpecular, new GLfloat[4]{ 1.0, 1.0, 1.0, 1.0 }, sizeof(GLfloat) * 4);
        memcpy(matShininess, new GLfloat[1]{ 100.0 }, sizeof(GLfloat));
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idlefunc);
    glutKeyboardFunc(keyboard);


    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
