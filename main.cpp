#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include "myDrawUtil.h"
#include "snake.h"

static float angle = 0.0, ratio;  // angle绕y轴的旋转角，ratio窗口高宽比
static float x = 0.0f, y = 0.0f, z = 3.0f;  //相机位置
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //视线方向，初始设为沿着Z轴负方向

bool mouseDown = false;
float xrot = 0.0f, yrot = 0.0f;
float xdiff = 0.0f, ydiff = 0.0f;

float zoom = 0.1f;
int nowDir = -1;
snake mySnake(4);
MyPos<int> myFood;

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
        mouseDown = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
        mouseDown = false;
}

/**
 * 鼠标移动事件
 */
void mouseMotion(int x, int y) {
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
//        case 'A':
//            angle -= 0.01f;
//            orientMe(angle);
//            break;
//        case 'D':
//            angle += 0.01f;
//            orientMe(angle);
//            break;
        case GLUT_KEY_PAGE_DOWN:
            moveMeFlat(-1);
            break;
        case GLUT_KEY_PAGE_UP:
            moveMeFlat(1);
            break;
        case GLUT_KEY_RIGHT:
            if (nowDir != 1) nowDir = 0;
            break;
        case GLUT_KEY_LEFT:
            if (nowDir != 0) nowDir = 1;
            break;
        case GLUT_KEY_UP:
            if (nowDir != 3) nowDir = 2;
            break;
        case GLUT_KEY_DOWN:
            if (nowDir != 2) nowDir = 3;
            break;
        case GLUT_KEY_HOME:
            if (nowDir != 5) nowDir = 4;
            break;
        case GLUT_KEY_END:
            if (nowDir != 4) nowDir = 5;
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

    // 创建透视效果视图
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW);

    // 平面方块铺设
    glPushMatrix();
    glTranslatef(-25 * zoom, -25 * zoom, 0);
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
        for (int j = 0; j < 50; j++) {
            drawBox(zoom / 2, zoom / 2, zoom / 2, (i & 1) * 0.2, 0.2, (j & 1) * 0.2, 0.1f);
            glTranslatef(0, zoom, 0);
        }
        glPopMatrix();
        glTranslatef(zoom, 0, 0);
    }
    glPopMatrix();

    // 贪吃蛇
    for (auto &item:mySnake.s) {
        glPushMatrix();
        glTranslatef(item.x * zoom, item.y * zoom, item.z * zoom);
        drawBox(zoom / 2, zoom / 2, zoom / 2, 1.0f, 1.0f, 0, 1);
        glPopMatrix();
    }

    // 食物
    glPushMatrix();
    glTranslatef(myFood.x * zoom, myFood.y * zoom, myFood.z * zoom);
    drawBox(zoom / 2, zoom / 2, zoom / 2, 1.0f, 0, 0, 1);
    glPopMatrix();

    glPushMatrix();
    std::list<MyPos<float >> lt;
    lt.push_back({zoom * 2, zoom * 2, zoom * 20});
    lt.push_back({zoom * 2, -zoom * 2, zoom * 20});
    lt.push_back({-zoom * 2, -zoom * 2, zoom * 20});
    lt.push_back({-zoom * 2, zoom * 2, zoom * 20});
    std::list<MyColor> ltC;
    ltC.push_back({float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10});
    ltC.push_back({float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10});
    ltC.push_back({float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10, float(rand() % 10) / 10});
    drawPull(lt, zoom*4, ltC);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

/**
 * 计时增加角度
 */
void myIdle() {
    static int m = 0;
    m++;
    if (m % 5000000 == 0) {
        mySnake.updateSnake(nowDir);
        if (mySnake.s.front().x == myFood.x &&
            mySnake.s.front().y == myFood.y &&
            mySnake.s.front().z == myFood.z) {
            mySnake.eatFood(nowDir);
            myFood.set(rand() % 50 - 25, rand() % 50 - 25, 1);
        }
        myDisplay();
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    myFood.set(rand() % 50 - 25, rand() % 50 - 25, 1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("太阳，地球和月亮");  // 改了窗口标题

    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);  // 表示在CPU空闲的时间调用某一函数
    glutSpecialFunc(processSpecialKeys);  // 按键
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    // 在OpenGL中，默认是没有开启深度检测的，后绘制的物体覆盖先绘制的物体。
    // GL_DEPTH_TEST 用来开启更新深度缓冲区的功能
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
    glutMainLoop();
    return 0;
}
