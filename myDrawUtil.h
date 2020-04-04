#ifndef OPENGLGAME_MYDRAWUTIL_H
#define OPENGLGAME_MYDRAWUTIL_H

#include <list>
#include <utility>

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

struct MyColor {
    float r{}, g{}, b{}, a{};

    MyColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    MyColor() {
        r = g = b = a = 1.0f;
    };
};

// 绘制正(长)方体,x,y,z为正(长)方体的距中心的距离
void drawBox(float x, float y, float z, float r, float g, float b, float a) {
    glBegin(GL_QUADS);

    glColor4f(r, g, b, a);
    // FRONT
    glVertex3f(-x, -y, z);
    glVertex3f(x, -y, z);
    glVertex3f(x, y, z);
    glVertex3f(-x, y, z);
    // BACK
    glVertex3f(-x, -y, -z);
    glVertex3f(-x, y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(x, -y, -z);
    // LEFT
    glVertex3f(-x, -y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, -y, -z);
    // RIGHT
    glVertex3f(x, -y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(x, y, z);
    glVertex3f(x, -y, z);
    // TOP
    glVertex3f(-x, y, z);
    glVertex3f(x, y, z);
    glVertex3f(x, y, -z);
    glVertex3f(-x, y, -z);
    // BOTTOM
    glVertex3f(-x, -y, z);
    glVertex3f(-x, -y, -z);
    glVertex3f(x, -y, -z);
    glVertex3f(x, -y, z);
    glEnd();
}

// 拉伸操作lx,ly,lz为草图一系列点，h为高
void drawPull(std::list<MyPos<float >> lt, float h, const std::list<MyColor> &ltC) {
    auto iter = ltC.begin();
    glBegin(GL_QUAD_STRIP);
    glColor4f(iter->r, iter->g, iter->b, iter->a), ++iter;
    for (auto &item:lt) {
        glVertex3f(item.x, item.y, item.z);
        glVertex3f(item.x, item.y, item.z + h);
    }
    glEnd();
    // 相等表示闭合，绘制两个面加侧面，不相等绘制侧面
    if (lt.back() == lt.front()) {
        glBegin(GL_POLYGON);
        glColor4f(iter->r, iter->g, iter->b, iter->a), ++iter;
        for (auto &item:lt) {
            glVertex3f(item.x, item.y, item.z);
        }
        glEnd();
        glBegin(GL_POLYGON);
        glColor4f(iter->r, iter->g, iter->b, iter->a), ++iter;
        for (auto &item:lt) {
            glVertex3f(item.x, item.y, item.z + h);
        }
        glEnd();
    }
}



#endif //OPENGLGAME_MYDRAWUTIL_H
