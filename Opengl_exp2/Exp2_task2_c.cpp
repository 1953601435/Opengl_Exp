#include <GL/glut.h>
#include<math.h>

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
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	//????????????????Ч??
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);
}
// 角度转换为弧度
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
void display(void)
{
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
		cameraPos[0]+cameraFront[0], cameraPos[1]+cameraFront[1], cameraPos[2]+ cameraFront[2],//注视目标位置
		cameraUp[0], cameraUp[1], cameraUp[2]);
	
	//立方体的旋转
	glPushMatrix();
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);

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
	glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 0.5,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

}
void idlefunc() {
	/*beta += 0.05;
	if (beta > 360)beta -= 360;*/
	rotationAngle += 0.05;
	if (rotationAngle > 360)
		rotationAngle -= 360;
	glutPostRedisplay();
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



int main(int argc, char** argv) {
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
	glutPassiveMotionFunc(mouse_callback);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}