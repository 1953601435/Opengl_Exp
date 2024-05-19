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
// 鼠标参数
bool firstMouse = true;
float yaw = -90.0f;  // 初始的yaw值，向左看
float pitch = 0.0f;
float lastX = 400, lastY = 300;  // 初始化鼠标位置为屏幕中心
float fov = 45.0f;  // 视角
bool cameraLocked = false;
GLsizei width = 600, height = 600;          //球的大小
int uStepsNum = 50, vStepNum = 50;          //线的条数，从一个极点到另外一个极点的条数
float radians(float degrees) {
    return degrees * 3.14159265358979323846 / 180.0f;
}
// 向量标准化函数
void normalize(float v[3]) {
    float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length != 0) {
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}
class Point                                //点的数据结构
{
public:
    Point() {};
    Point(double a, double b, double c) :x(a), y(b), z(c) {};
public:
    double x;
    double y;
    double z;                             //xyz三个坐标
};
Point getPoint(double u, double v)        //参数坐标(u,v)转换成球面坐标(x,y,z)函数
{
    double x = sin(PI * v) * cos(PI2 * u);
    double y = sin(PI * v) * sin(PI2 * u);
    double z = cos(PI * v);
    return Point(x, y, z);               //球面参数坐标的转换公式
}
void drawWire()                         //绘制球面
{
    double ustep = 1 / (double)uStepsNum, vstep = 1 / (double)vStepNum; //离散化处理，设置增量大小
    double u = 0, v = 0;   //初始化位置，v是球面点与原点的连线与z轴正向的夹角，u表示连线在xy平面的投影与x轴正向的夹角
    //绘制下端三角形组
    for (int i = 0;i < uStepsNum;i++)
    {
        glBegin(GL_LINE_LOOP);              //GL_LINE_LOOP绘制的
        Point a = getPoint(0, 0);
        glVertex3d(a.x, a.y, a.z);             //绘制三维空间坐标点
        Point b = getPoint(u, vstep);
        glVertex3d(b.x, b.y, b.z);
        Point c = getPoint(u + ustep, vstep);//画纬线上的点
        glVertex3d(c.x, c.y, c.z);
        u += ustep;
        glEnd();
    }                                        //下端三角形组主要是绘制极点附近的图形
    //绘制中间四边形组
    u = 0, v = vstep;
    for (int i = 0;i < vStepNum - 1;i++)
    {
        for (int j = 0;j < uStepsNum;j++)
        {
            glBegin(GL_LINE_LOOP);
            Point a = getPoint(u, v);
            Point b = getPoint(u + ustep, v);
            Point c = getPoint(u + ustep, v + vstep);
            Point d = getPoint(u, v + vstep);//经纬线格
            glVertex3d(a.x, a.y, a.z);
            glVertex3d(b.x, b.y, b.z);
            glVertex3d(c.x, c.y, c.z);
            glVertex3d(d.x, d.y, d.z);
            u += ustep;
            glEnd();
        }
        v += vstep;
    }
    //绘制下端三角形组
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
    glClearColor(0, 0, 1, 1);//使用指定颜色清除颜色缓冲区
    glClearDepth(0.0);//指定深度缓冲区的清除值
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//要清除颜色缓冲以及深度缓冲
    glClearColor(1, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; //光源位置
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f }; //环境光
    GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.2f };//漫反射光
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 0.2f };//镜面反射光
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //创建指定的光源,light可以是GL_LIGHT0、GL_LIGHT1，pname定义的是光源的属性，它指定了一个命名参数。params表示表示pname属性将要被设置的值
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL); //开启材质颜料设置功能
    glEnable(GL_LIGHTING);        //启动光源
    glEnable(GL_LIGHT0);          //光源的颜色为白色
    glEnable(GL_AUTO_NORMAL);     //执行后，图形能把光反射到各个方向
    glEnable(GL_NORMALIZE);        //根据函数glNormal的设置条件，启用法向量
    glEnable(GL_DEPTH_TEST);        //启用深度测试。根据坐标的远近自动隐藏被遮住的图形（材料）
    glDepthFunc(GL_LESS);        //深度小的时候才渲染
}
void displayFunc()
{
    glMatrixMode(GL_MODELVIEW);  //对模型视景矩阵操作
    glLoadIdentity();
    	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraPos[0]+cameraFront[0], cameraPos[1]+cameraFront[1], cameraPos[2]+ cameraFront[2],//注视目标位置
		cameraUp[0], cameraUp[1], cameraUp[2]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(1.0);//指定栅格化点的直径
    glRotated(rotationAngle+30, 1, 0, 0);//相乘旋转矩阵的当前的矩阵
    glRotated(rotationAngle+60, 0, 1, 0);
    glRotated(rotationAngle+90, 0, 0, 1);
    
    drawWire();
    glutSwapBuffers();//交换两个缓冲区指针
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
    //利用正交投影，根据窗口调整矩形视景体
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
		cameraLocked = !cameraLocked; // 切换相机控制锁定状态
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
			// 标准化
			length = sqrt(left[0] * left[0] + left[1] * left[1] + left[2] * left[2]);
			left[0] /= length;
			left[1] /= length;
			left[2] /= length;
			// 移动相机
			cameraPos[0] -= 0.3f * left[0];
			cameraPos[1] -= 0.3f * left[1];
			cameraPos[2] -= 0.3f * left[2];
			break;
		case 'd':
			// 计算相机右方向向量
			GLfloat right[3];
			right[0] = cameraFront[2] * cameraUp[1] - cameraFront[1] * cameraUp[2];
			right[1] = cameraFront[0] * cameraUp[2] - cameraFront[2] * cameraUp[0];
			right[2] = cameraFront[1] * cameraUp[0] - cameraFront[0] * cameraUp[1];
			// 标准化
			length = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
			right[0] /= length;
			right[1] /= length;
			right[2] /= length;
			// 移动相机
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
	if (cameraLocked) return; // 如果相机被锁定，不处理鼠标事件
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
    glutCreateWindow("线框球体");

    init();
    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc); // 注册reshape函数
    glutIdleFunc(idleFunc); // 注册idle函数
    glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse_callback);
    glutMainLoop();
}

