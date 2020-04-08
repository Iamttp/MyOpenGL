#ifndef OPENGLGAME_MYDRAWUTIL_H
#define OPENGLGAME_MYDRAWUTIL_H

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

    bool operator==(const MyPos<A> &b) {
        return x == b.x && y == b.y && std::abs(z - b.z) < 0.0001f;
    }

    MyPos<A> operator-(const MyPos<A> &b) {
        return {x - b.x, y - b.y, z - b.z};
    }

    MyPos() = default;

    MyPos(A x, A y, A z) : x(x), y(y), z(z) {}
};

void drawSketch(std::vector<MyPos<float >> lt) {
    /* 绘制线段操作 */
    glBegin(GL_LINE_STRIP);
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
    }
    glEnd();
}

// 拉伸操作lx,ly,lz为草图一系列点，h为高
void drawPull(std::vector<MyPos<float >> lt, float h) {
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

void drawPull2(std::vector<MyPos<float >> lt, float h) {
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
MyPos<float> screen2world(int x, int y) {
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
    return {static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(posZ)};
}

inline float myRound(float a) {
    return std::round(10 * a) / 10.0f;
}

/**
    已知四个点坐标判断是否共面
    可以用行列式来判断
    用四个点求出三个向量分别为(x1,y1,z1),(x2,y2,z2),(x3,y3,z3)
    判断行列式
    | x1 x2 x3|
    | y1 y2 y3|
    | z1 z2 z3|
    或者它的转置是否为零
    若为零则四点共面
*/
float fourPointC(MyPos<float> p1, MyPos<float> p2, MyPos<float> p3, MyPos<float> p4) {
    MyPos<float> vx = p2 - p1, vy = p3 - p1, vz = p4 - p1;
    float x1 = vx.x, x2 = vy.x, x3 = vz.x,
            y1 = vx.y, y2 = vy.y, y3 = vz.y,
            z1 = vx.z, z2 = vy.z, z3 = vz.z;
    return (x1 * y2 * z3) + (x2 * y3 * z1) + (x3 * y1 * z2) - (x3 * y2 * z1) - (y3 * z2 * x1) - (z3 * x2 * y1);
}

struct Per3dObject {
    std::vector<MyPos<float >> sketch;
    MyPos<float> color{0.7, 0.7, 0.7};
    float h = 0;

    bool collisionDetection(MyPos<float> world) {
        for (int i = 0; i < sketch.size() - 1; ++i) {
            MyPos<float> item = sketch[i];
            item.z += h;
            if (std::abs(fourPointC(world, sketch[i], sketch[i + 1], item)) < 0.1f)
                return true;
        }
        return false;
    }

    void setColor(MyPos<float> tmpColor) {
        color = tmpColor;
    }
};

#endif //OPENGLGAME_MYDRAWUTIL_H
