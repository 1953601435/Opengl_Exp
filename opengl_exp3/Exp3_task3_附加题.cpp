#include <GL/glut.h>

// ȫ�ֱ��������ڴ洢���պͲ��ʲ���
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 100.0 };

// ȫ�ֱ��������ڴ洢��ת�ǶȺ���ת�ٶ�
float angle = 0.0f;
float rotation_speed = 0.05f;

// ȫ�ֱ��������ڴ洢����ǿ��
GLfloat light_intensity = 1.0;

// ��ʼ������
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // ���ù���
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ���ù��պͲ��ʲ���
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// ��������ת����
void rotateCube() {
    angle += rotation_speed;
    if (angle > 360)
        angle -= 360;
    glutPostRedisplay();
}

// ���������庯��
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glutSwapBuffers();
}

// ���ڴ�С�仯�ص�����
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

// �Ҽ��˵��ص�����
void control(int value) {
    switch (value) {
    case 1:
        // ���ӹ���ǿ��
        light_intensity += 0.5;
        if (light_intensity > 1.0)
            light_intensity = 1.0;
        // ���¹��ղ���
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        for (int i = 0; i < 3; ++i)
            light_diffuse[i] = light_intensity;
        break;
    case 2:
        // ���ٹ���ǿ��
        light_intensity -= 0.5;
        if (light_intensity < 0.0)
            light_intensity = 0.0;
        // ���¹��ղ���
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        for (int i = 0; i < 3; ++i)
            light_diffuse[i] = light_intensity;
        break;
    case 3:
        // �޸Ĳ��ʵ���������ɫΪ��ɫ
        mat_diffuse[0] = 1.0;
        mat_diffuse[1] = 0.0;
        mat_diffuse[2] = 0.0;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        break;
    case 4:
        // �޸Ĳ��ʵ���������ɫΪ��ɫ
        mat_diffuse[0] = 0.0;
        mat_diffuse[1] = 1.0;
        mat_diffuse[2] = 0.0;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(rotateCube);

    // �����Ҽ��˵�
    glutCreateMenu(control);
    glutAddMenuEntry("add lightpower", 1);
    glutAddMenuEntry("lower lightpower", 2);
    glutAddMenuEntry("red material", 3);
    glutAddMenuEntry("green material", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
