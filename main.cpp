#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <vector>
#include "myDrawUtil.h"

static float angle = 0.0, ratio;  // angle绕y轴的旋转角，ratio窗口高宽比
static float x = 0.0f, y = 0.0f, z = 6.0f;  //相机位置
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //视线方向，初始设为沿着Z轴负方向

bool mouseDown = false;
float xrot = 0.0f, yrot = 0.0f;
float xdiff = 0.0f, ydiff = 0.0f;

const float zoom = 0.1f;
const int width = 100;
const int height = 100;

int useUtil = 0;
std::list<Per3dObject> glp; // 全局已完成的图形
Per3dObject now;    // 正在绘制的图形

/**
 * 定义观察方式
 */
void changeSize(int w, int h) {
    //除以0的情况
    if (h == 0) h = 1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //设置视口为整个窗口大小
    glViewport(0, 0, w, h);
    //设置可视空间
    gluPerspective(45, ratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 视野漫游函数
 */
void orientMe(float ang) {
    lx = std::sin(ang);
    lz = -std::cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 视野漫游函数
 */
void moveMeFlat(int direction) {
    x += direction * (lx) * 0.1;
    z += direction * (lz) * 0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

/**
 * 鼠标事件
*/
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (useUtil == 1) {
            std::vector<double> world = screen2world(x, y);
//            std::cout << func(world[1]) << " " << func(world[2]) << " " << myRound(world[3]) << std::endl;
            now.sketch.emplace_back(myRound(world[1]), myRound(world[2]), world[3]);
        } else {
            mouseDown = true;
            xdiff = x - yrot;
            ydiff = -y + xrot;
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (useUtil == 2) {
            useUtil = 0;
            glp.push_back(now);
            now.sketch.clear();
            now.h = 0;
        }
    } else
        mouseDown = false;
}

/**
 * 鼠标移动事件
 */
void mouseMotion(int x, int y) {
    if (useUtil == 2) {
        std::vector<double> world = screen2world(x, y);
        now.h = world[2];
        glutPostRedisplay();
    }
    if (mouseDown) {
        yrot = x - xdiff;
        xrot = y + ydiff;
        glutPostRedisplay();
    }
}

/**
 * 加入按键控制
 */
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_PAGE_DOWN:
            moveMeFlat(-1);
            break;
        case GLUT_KEY_PAGE_UP:
            moveMeFlat(1);
            break;
        default:
            break;
    }
}

void processNormalKeys(unsigned char key, int x, int y) {
    useUtil = 0;
    switch (key) {
        case 'z':
            // 画笔
            useUtil = 1;
            xrot = 0.0f, yrot = 0.0f;
            break;
        case 'q':
            // 拉升
            useUtil = 2;
            xrot = -45.0f, yrot = 45.0f;
            break;
        case 'a':
            // 切除
            break;
        default:
            break;
    }
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    // 实现鼠标旋转的核心
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    // 最后画草图
    glPushMatrix();
    glColor4f(0, 0, 0, 0);
    drawSketch(now.sketch);
    glPopMatrix();

    if (useUtil == 2) {
        glPushMatrix();
        glColor4f(0.9, 0.9, 0.9, 0);
        drawPull(now.sketch, now.h);
        glPopMatrix();
        glPushMatrix();
        glColor4f(0, 0, 0, 0);
        drawPull2(now.sketch, now.h);
        glPopMatrix();
    }

    // 然后画已完成
    for (auto &item:glp) {
        glPushMatrix();
        glColor4f(0.7, 0.7, 0.7, 0);
        drawPull(item.sketch, item.h);
        glPopMatrix();
        glPushMatrix();
        glColor4f(0, 0, 0, 0);
        drawPull2(item.sketch, item.h);
        glPopMatrix();
    }

    // 然后坐标系
    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glPushMatrix();
    glColor4f(1, 0, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(-width * zoom, 0, 0);
    glVertex3f(width * zoom, 0, 0);
    glEnd();

    glPushMatrix();
    glColor4f(1, 0, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, -height * zoom, 0);
    glVertex3f(0, height * zoom, 0);
    glEnd();

    // 再画线
    glPushMatrix();
    glTranslatef(-width * zoom, 0, 0);
    glColor4f(0, 1, 0, 0);
    for (int i = 0; i < 2 * width; i++) {
        glBegin(GL_LINES);
        glVertex3f(0, -height * zoom, 0);
        glVertex3f(0, height * zoom, 0);
        glEnd();
        glTranslatef(zoom, 0, 0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -height * zoom, 0);
    for (int i = 0; i < 2 * height; i++) {
        glBegin(GL_LINES);
        glVertex3f(-width * zoom, 0, 0);
        glVertex3f(width * zoom, 0, 0);
        glEnd();
        glTranslatef(0, zoom, 0);
    }
    glPopMatrix();

    // 先画平面
    glColor4f(1, 1, 1, 1);
    glRectf(-width * zoom, -height * zoom, width * zoom, height * zoom);

    glFlush();
    glutSwapBuffers();
}

/**
 * 计时增加角度
 */
void myIdle() {
    myDisplay();
}

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Demo");  // 改了窗口标题

    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);  // 表示在CPU空闲的时间调用某一函数
    glutSpecialFunc(processSpecialKeys);  // 按键
    glutKeyboardFunc(processNormalKeys);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    // 在OpenGL中，默认是没有开启深度检测的，后绘制的物体覆盖先绘制的物体。
    // GL_DEPTH_TEST 用来开启更新深度缓冲区的功能
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
//    const GLubyte *name = glGetString(GL_VENDOR);
//    const GLubyte *biaoshifu = glGetString(GL_RENDERER);
//    const GLubyte *OpenGLVersion = glGetString(GL_VERSION);
//    const GLubyte *gluVersion = gluGetString(GLU_VERSION);
//    std::cout << name << std::endl;
//    std::cout << biaoshifu << std::endl;
//    std::cout << OpenGLVersion << std::endl;
//    std::cout << gluVersion << std::endl;
    glutMainLoop();
    return 0;
}
