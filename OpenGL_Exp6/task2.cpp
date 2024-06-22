#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

std::vector<std::vector<std::vector<GLfloat>>> controlPoints = {
    {{100.0f, 100.0f, 0.0f}, {300.0f, 400.0f, 0.0f}, {500.0f, 300.0f, 0.0f}},
    {{200.0f, 500.0f, 0.0f}, {400.0f, 600.0f, 0.0f}, {600.0f, 200.0f, 0.0f}},
    {{300.0f, 200.0f, 0.0f}, {500.0f, 100.0f, 0.0f}, {700.0f, 500.0f, 0.0f}}
};

bool isAnimating = true;
int animationIndex = 0;
const int numCurvePoints = 100;
int windowWidth = 800;
int windowHeight = 600;
int selectedRowIndex = -1;
int selectedColIndex = -1;
bool isMovingPoint = false;
const float pointSize = 5.0f;
const float selectionRadius = 10.0f;

float binomialCoefficient(int n, int k) {
    float res = 1.0f;
    for (int i = 1; i <= k; ++i) {
        res *= (n - i + 1) / static_cast<float>(i);
    }
    return res;
}

float bernsteinPolynomial(int n, int i, float t) {
    return binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
}

std::vector<GLfloat> interpolate(const std::vector<std::vector<GLfloat>>& points, float t) {
    int n = points.size() - 1;
    GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
    for (int i = 0; i <= n; ++i) {
        float b = bernsteinPolynomial(n, i, t);
        x += points[i][0] * b;
        y += points[i][1] * b;
        z += points[i][2] * b;
    }
    return { x, y, z };
}

void drawIntermediateLines(const std::vector<std::vector<GLfloat>>& points, float t) {
    std::vector<std::vector<GLfloat>> tempPoints = points;
    while (tempPoints.size() > 1) {
        std::vector<std::vector<GLfloat>> newPoints;
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < tempPoints.size() - 1; ++i) {
            GLfloat x = (1 - t) * tempPoints[i][0] + t * tempPoints[i + 1][0];
            GLfloat y = (1 - t) * tempPoints[i][1] + t * tempPoints[i + 1][1];
            GLfloat z = (1 - t) * tempPoints[i][2] + t * tempPoints[i + 1][2];
            newPoints.push_back({ x, y, z });
            glVertex3f(tempPoints[i][0], tempPoints[i][1], tempPoints[i][2]);
            glVertex3f(x, y, z);
        }
        glVertex3f(tempPoints.back()[0], tempPoints.back()[1], tempPoints.back()[2]);
        glEnd();
        tempPoints = newPoints;
    }
}

void drawBezierSurface(const std::vector<std::vector<std::vector<GLfloat>>>& controlPoints, float t) {
    int numUPoints = controlPoints.size();
    int numVPoints = controlPoints[0].size();

    for (int i = 0; i <= numCurvePoints; ++i) {
        float u = static_cast<float>(i) / numCurvePoints;
        std::vector<std::vector<GLfloat>> curvePoints;
        for (int j = 0; j < numVPoints; ++j) {
            std::vector<std::vector<GLfloat>> tempPoints;
            for (int k = 0; k < numUPoints; ++k) {
                tempPoints.push_back(controlPoints[k][j]);
            }
            curvePoints.push_back(interpolate(tempPoints, u));
        }
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= static_cast<int>(t * numCurvePoints); ++j) {
            float v = static_cast<float>(j) / numCurvePoints;
            std::vector<GLfloat> point = interpolate(curvePoints, v);
            glVertex3f(point[0], point[1], point[2]);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!controlPoints.empty()) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        for (const auto& row : controlPoints) {
            for (const auto& point : row) {
                glVertex3fv(point.data());
            }
        }
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        for (const auto& row : controlPoints) {
            for (size_t i = 0; i < row.size() - 1; ++i) {
                glVertex3fv(row[i].data());
                glVertex3fv(row[i + 1].data());
            }
        }
        for (size_t j = 0; j < controlPoints[0].size(); ++j) {
            for (size_t i = 0; i < controlPoints.size() - 1; ++i) {
                glVertex3fv(controlPoints[i][j].data());
                glVertex3fv(controlPoints[i + 1][j].data());
            }
        }
        glEnd();
    }

    if (isAnimating && controlPoints.size() > 1) {
        GLfloat t = static_cast<float>(animationIndex) / numCurvePoints;

        for (const auto& row : controlPoints) {
            drawIntermediateLines(row, t);
        }

        drawBezierSurface(controlPoints, t);
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
            float mx = static_cast<float>(x);
            float my = static_cast<float>(windowHeight - y);
            for (int i = 0; i < controlPoints.size(); ++i) {
                for (int j = 0; j < controlPoints[i].size(); ++j) {
                    float dx = controlPoints[i][j][0] - mx;
                    float dy = controlPoints[i][j][1] - my;
                    if (sqrt(dx * dx + dy * dy) <= selectionRadius) {
                        selectedRowIndex = i;
                        selectedColIndex = j;
                        isMovingPoint = true;
                        return;
                    }
                }
            }
        }
        else if (state == GLUT_UP) {
            isMovingPoint = false;
            selectedRowIndex = -1;
            selectedColIndex = -1;
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isMovingPoint && selectedRowIndex != -1 && selectedColIndex != -1) {
        controlPoints[selectedRowIndex][selectedColIndex][0] = static_cast<GLfloat>(x);
        controlPoints[selectedRowIndex][selectedColIndex][1] = static_cast<GLfloat>(windowHeight - y);
        glutPostRedisplay();
    }
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Interactive Bezier Surface");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
