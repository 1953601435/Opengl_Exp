#include <GL/glut.h>
#include <vector>
#include <cmath>

std::vector<std::vector<GLfloat>> controlPoints;
bool isAnimating = false;
int animationIndex = 0;
const int numCurvePoints = 1000;
int windowWidth = 800;
int windowHeight = 600;
int selectedPointIndex = -1;
bool isMovingPoint = false;
const float pointSize = 5.0f;
const float selectionRadius = 10.0f;

void drawBezierRecursive(const std::vector<std::vector<GLfloat>>& points, float t) {
    std::vector<std::vector<GLfloat>> tempPoints = points; // 拷贝一份数据用于递归计算
    while (tempPoints.size() > 1) {
        glColor3f(0.5f, 0.5f, 0.5f); // Grey for intermediate lines
        glBegin(GL_LINE_STRIP);
        for (const auto& point : tempPoints) {
            glVertex2f(point[0], point[1]);
        }
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f); // Red for intermediate points
        //glPointSize(pointSize);
        glBegin(GL_POINTS);
        for (const auto& point : tempPoints) {
            glVertex2f(point[0], point[1]);
        }
        glEnd();

        std::vector<std::vector<GLfloat>> newPoints;
        for (size_t i = 0; i < tempPoints.size() - 1; ++i) {
            GLfloat x = (1 - t) * tempPoints[i][0] + t * tempPoints[i + 1][0];
            GLfloat y = (1 - t) * tempPoints[i][1] + t * tempPoints[i + 1][1];
            newPoints.push_back({ x, y });
        }
        tempPoints = newPoints;
    }

    glColor3f(0.0f, 1.0f, 0.0f); // Green for final point on the curve
    glBegin(GL_POINTS);
    glVertex2f(tempPoints[0][0], tempPoints[0][1]);
    glEnd();
}

void drawBezierCurve(const std::vector<std::vector<GLfloat>>& points) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= numCurvePoints; ++i) {
        float t = (float)i / (float)numCurvePoints;
        GLfloat x = 0.0f, y = 0.0f;
        int n = points.size() - 1;
        for (int j = 0; j <= n; ++j) {
            float binomialCoeff = 1.0f;
            for (int k = 0; k < j; ++k) {
                binomialCoeff *= (n - k) / (float)(k + 1);
            }
            float blend = binomialCoeff * pow(t, j) * pow(1 - t, n - j);
            x += points[j][0] * blend;
            y += points[j][1] * blend;
        }
        glVertex2f(x, y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!controlPoints.empty()) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for control points
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        for (const auto& point : controlPoints) {
            glVertex2fv(point.data());
        }
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines connecting control points
        glBegin(GL_LINE_STRIP);
        for (const auto& point : controlPoints) {
            glVertex2fv(point.data());
        }
        glEnd();
    }

    if (isAnimating && controlPoints.size() > 1) {
        GLfloat t = (float)animationIndex / (float)numCurvePoints;
        drawBezierRecursive(controlPoints, t); // 绘制递归中间过程
        glColor3f(0.0f, 0.0f, 1.0f); // 蓝色贝塞尔曲线
        drawBezierCurve(controlPoints); // 绘制贝塞尔曲线

        animationIndex = (animationIndex + 1) % (numCurvePoints + 1);
    }

    glutSwapBuffers();
}


void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Check if a control point is selected
            float mx = (float)x;
            float my = (float)(windowHeight - y);
            for (int i = 0; i < controlPoints.size(); ++i) {
                float dx = controlPoints[i][0] - mx;
                float dy = controlPoints[i][1] - my;
                if (sqrt(dx * dx + dy * dy) <= selectionRadius) {
                    selectedPointIndex = i;
                    isMovingPoint = true;
                    return;
                }
            }

            // If no point is selected, add a new control point
            controlPoints.push_back({ mx, my });
            if (controlPoints.size() > 1) {
                isAnimating = true;
            }
        }
        else if (state == GLUT_UP) {
            isMovingPoint = false;
            selectedPointIndex = -1;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        controlPoints.clear();
        isAnimating = false;
        animationIndex = 0;
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isMovingPoint && selectedPointIndex != -1) {
        controlPoints[selectedPointIndex][0] = (GLfloat)x;
        controlPoints[selectedPointIndex][1] = (GLfloat)(windowHeight - y);
        glutPostRedisplay();
    }
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Interactive Bezier Curve");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
