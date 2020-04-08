#ifndef OPENGLGAME_MYDRAWUTIL_H
#define OPENGLGAME_MYDRAWUTIL_H

#include <list>
#include <vector>
#include <GL/glut.h>
#include <cmath>

template<class A>
struct MyPos {
    A x{}, y{}, z{};

    void set(A x, A y, A z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool operator==(const MyPos<float> b) {
        return x == b.x && y == b.y && z == b.z;
    }

    MyPos() = default;

    MyPos(A x, A y, A z) : x(x), y(y), z(z) {}
};

void drawSketch(std::list<MyPos<float >> lt) {
    /* 绘制线段操作 */
    glBegin(GL_LINE_STRIP);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
    }
    glEnd();
}

// 拉伸操作lx,ly,lz为草图一系列点，h为高
void drawPull(std::list<MyPos<float >> lt, float h) {
    glBegin(GL_QUAD_STRIP);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
        glVertex3f(item.x, item.y, item.z + h);
    }
    glEnd();
    // 相等表示闭合，绘制两个面加侧面，不相等绘制侧面
    if (lt.back() == lt.front()) {
        glBegin(GL_POLYGON);
        for (auto &item:lt) {
            glVertex3f(item.x, item.y, item.z);
        }
        glEnd();
        glBegin(GL_POLYGON);
        for (auto &item:lt) {
            glVertex3f(item.x, item.y, item.z + h);
        }
        glEnd();
    }
}

void drawPull2(std::list<MyPos<float >> lt, float h) {
    glBegin(GL_LINES);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
        glVertex3f(item.x, item.y, item.z + h);
    }
    glEnd();
    glBegin(GL_LINES);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z + h);
    }
    glEnd();
}

// https://blog.csdn.net/sac761/article/details/52179585
std::vector<double> screen2world(int x, int y) {
    int viewport[4];
    double modelview[16];
    double projection[16];
    float winX, winY, winZ;
    double posX, posY, posZ;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
    // 把已经绘制好的像素（它可能已经被保存到显卡的显存中）读取到内存
    // GL_DEPTH_COMPONENT 为深度读取
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    std::vector<double> v{4, posX, posY, posZ, 1.0};
    return v;
}

inline float myRound(float a) {
    return std::round(10 * a) / 10.0f;
}

struct Per3dObject {
    std::list<MyPos<float >> sketch;
    float h = 0;

    int index;
    void finish() {
        index = glGenLists(1);//glGenLists()唯一的标识一个显示列表
        glNewList(index, GL_COMPILE);//用于对显示列表进行定界。第一个参数是一个整形索引值，由glGenLists()指定
        glPushMatrix();
        glColor4f(0.7, 0.7, 0.7, 0);
        drawPull(sketch, h);
        glPopMatrix();
        glPushMatrix();
        glColor4f(0, 0, 0, 0);
        drawPull2(sketch, h);
        glPopMatrix();
        glEndList();
    }
};

#endif //OPENGLGAME_MYDRAWUTIL_H
