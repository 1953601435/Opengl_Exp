#include <gl/glut.h>
#include<math.h>
#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692
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
// ������
bool firstMouse = true;
float yaw = -90.0f;  // ��ʼ��yawֵ������
float pitch = 0.0f;
float lastX = 400, lastY = 300;  // ��ʼ�����λ��Ϊ��Ļ����
float fov = 45.0f;  // �ӽ�
bool cameraLocked = false;
GLsizei width = 600, height = 600;          //��Ĵ�С
int uStepsNum = 50, vStepNum = 50;          //�ߵ���������һ�����㵽����һ�����������
float radians(float degrees) {
    return degrees * 3.14159265358979323846 / 180.0f;
}
// ������׼������
void normalize(float v[3]) {
    float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length != 0) {
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}
class Point                                //������ݽṹ
{
public:
    Point() {};
    Point(double a, double b, double c) :x(a), y(b), z(c) {};
public:
    double x;
    double y;
    double z;                             //xyz��������
};
Point getPoint(double u, double v)        //��������(u,v)ת������������(x,y,z)����
{
    double x = sin(PI * v) * cos(PI2 * u);
    double y = sin(PI * v) * sin(PI2 * u);
    double z = cos(PI * v);
    return Point(x, y, z);               //������������ת����ʽ
}
void drawWire()                         //��������
{
    double ustep = 1 / (double)uStepsNum, vstep = 1 / (double)vStepNum; //��ɢ����������������С
    double u = 0, v = 0;   //��ʼ��λ�ã�v���������ԭ���������z������ļнǣ�u��ʾ������xyƽ���ͶӰ��x������ļн�
    //�����¶���������
    for (int i = 0;i < uStepsNum;i++)
    {
        glBegin(GL_LINE_LOOP);              //GL_LINE_LOOP���Ƶ�
        Point a = getPoint(0, 0);
        glVertex3d(a.x, a.y, a.z);             //������ά�ռ������
        Point b = getPoint(u, vstep);
        glVertex3d(b.x, b.y, b.z);
        Point c = getPoint(u + ustep, vstep);//��γ���ϵĵ�
        glVertex3d(c.x, c.y, c.z);
        u += ustep;
        glEnd();
    }                                        //�¶�����������Ҫ�ǻ��Ƽ��㸽����ͼ��
    //�����м��ı�����
    u = 0, v = vstep;
    for (int i = 0;i < vStepNum - 1;i++)
    {
        for (int j = 0;j < uStepsNum;j++)
        {
            glBegin(GL_LINE_LOOP);
            Point a = getPoint(u, v);
            Point b = getPoint(u + ustep, v);
            Point c = getPoint(u + ustep, v + vstep);
            Point d = getPoint(u, v + vstep);//��γ�߸�
            glVertex3d(a.x, a.y, a.z);
            glVertex3d(b.x, b.y, b.z);
            glVertex3d(c.x, c.y, c.z);
            glVertex3d(d.x, d.y, d.z);
            u += ustep;
            glEnd();
        }
        v += vstep;
    }
    //�����¶���������
    u = 0;
    for (int i = 0;i < uStepsNum;i++)
    {
        glBegin(GL_LINE_LOOP);
        Point a = getPoint(0, 1);
        Point b = getPoint(u, 1 - vstep);
        Point c = getPoint(u + ustep, 1 - vstep);
        glVertex3d(a.x, a.y, a.z);
        glVertex3d(b.x, b.y, b.z);
        glVertex3d(c.x, c.y, c.z);
        glEnd();
    }
}
void init()
{
    glClearColor(0, 0, 1, 1);//ʹ��ָ����ɫ�����ɫ������
    glClearDepth(0.0);//ָ����Ȼ����������ֵ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Ҫ�����ɫ�����Լ���Ȼ���
    glClearColor(1, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; //��Դλ��
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f }; //������
    GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.2f };//�������
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 0.2f };//���淴���
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //����ָ���Ĺ�Դ,light������GL_LIGHT0��GL_LIGHT1��pname������ǹ�Դ�����ԣ���ָ����һ������������params��ʾ��ʾpname���Խ�Ҫ�����õ�ֵ
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL); //���������������ù���
    glEnable(GL_LIGHTING);        //������Դ
    glEnable(GL_LIGHT0);          //��Դ����ɫΪ��ɫ
    glEnable(GL_AUTO_NORMAL);     //ִ�к�ͼ���ܰѹⷴ�䵽��������
    glEnable(GL_NORMALIZE);        //���ݺ���glNormal���������������÷�����
    glEnable(GL_DEPTH_TEST);        //������Ȳ��ԡ����������Զ���Զ����ر���ס��ͼ�Σ����ϣ�
    glDepthFunc(GL_LESS);        //���С��ʱ�����Ⱦ
}
void displayFunc()
{
    glMatrixMode(GL_MODELVIEW);  //��ģ���Ӿ��������
    glLoadIdentity();
    	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraPos[0]+cameraFront[0], cameraPos[1]+cameraFront[1], cameraPos[2]+ cameraFront[2],//ע��Ŀ��λ��
		cameraUp[0], cameraUp[1], cameraUp[2]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(1.0);//ָ��դ�񻯵��ֱ��
    glRotated(rotationAngle+30, 1, 0, 0);//�����ת����ĵ�ǰ�ľ���
    glRotated(rotationAngle+60, 0, 1, 0);
    glRotated(rotationAngle+90, 0, 0, 1);
    
    drawWire();
    glutSwapBuffers();//��������������ָ��
}
void idleFunc() {
    rotationAngle += 0.1f;
    if (rotationAngle > 360.0) {
        rotationAngle -= 360.0;
    }
    glutPostRedisplay();
}
void reshapeFunc(int w, int h) {
    if (h == 0)h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //��������ͶӰ�����ݴ��ڵ��������Ӿ���
    if (w <= h) {
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    }
    else {
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'l' || key == 'L') {
		cameraLocked = !cameraLocked; // �л������������״̬
		return;
	}
	if (!cameraLocked) {


		GLfloat length;
		switch (key)
		{
		case 27:
			exit(0);
			break;
		case 'q':
			cameraPos[0] += 0.3f * cameraUp[0];
			cameraPos[1] += 0.3f * cameraUp[1];
			cameraPos[2] += 0.3f * cameraUp[2];
			break;
		case 'e':
			cameraPos[0] -= 0.3f * cameraUp[0];
			cameraPos[1] -= 0.3f * cameraUp[1];
			cameraPos[2] -= 0.3f * cameraUp[2];
			break;
		case 'a':
			GLfloat left[3];
			left[0] = cameraFront[1] * cameraUp[2] - cameraFront[2] * cameraUp[1];
			left[1] = cameraFront[2] * cameraUp[0] - cameraFront[0] * cameraUp[2];
			left[2] = cameraFront[0] * cameraUp[1] - cameraFront[1] * cameraUp[0];
			// ��׼��
			length = sqrt(left[0] * left[0] + left[1] * left[1] + left[2] * left[2]);
			left[0] /= length;
			left[1] /= length;
			left[2] /= length;
			// �ƶ����
			cameraPos[0] -= 0.3f * left[0];
			cameraPos[1] -= 0.3f * left[1];
			cameraPos[2] -= 0.3f * left[2];
			break;
		case 'd':
			// ��������ҷ�������
			GLfloat right[3];
			right[0] = cameraFront[2] * cameraUp[1] - cameraFront[1] * cameraUp[2];
			right[1] = cameraFront[0] * cameraUp[2] - cameraFront[2] * cameraUp[0];
			right[2] = cameraFront[1] * cameraUp[0] - cameraFront[0] * cameraUp[1];
			// ��׼��
			length = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
			right[0] /= length;
			right[1] /= length;
			right[2] /= length;
			// �ƶ����
			cameraPos[0] -= 0.3f * right[0];
			cameraPos[1] -= 0.3f * right[1];
			cameraPos[2] -= 0.3f * right[2];
			break;
		case 'w':
			cameraPos[0] += 0.3f * cameraFront[0];
			cameraPos[1] += 0.3f * cameraFront[1];
			cameraPos[2] += 0.3f * cameraFront[2];
			break;
		case 's':
			cameraPos[0] -= 0.3f * cameraFront[0];
			cameraPos[1] -= 0.3f * cameraFront[1];
			cameraPos[2] -= 0.3f * cameraFront[2];
			break;
		}
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}
	glutPostRedisplay();
}
void mouse_callback(int xPos,int yPos)
{
	if (cameraLocked) return; // ������������������������¼�
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)pitch = 89.0f;
	if (pitch < -89.0f)pitch = -89.0f;

	float front[3];
	front[0] = cos(radians(pitch)) * cos(radians(yaw));
	front[1] = sin(radians(pitch));
	front[2] = cos(radians(pitch)) * sin(radians(yaw));
	normalize(front);
	cameraFront[0] = front[0];
	cameraFront[1] = front[1];
	cameraFront[2] = front[2];
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("�߿�����");

    init();
    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc); // ע��reshape����
    glutIdleFunc(idleFunc); // ע��idle����
    glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse_callback);
    glutMainLoop();
}

