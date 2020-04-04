#ifndef OPENGLGAME_SNAKE_H
#define OPENGLGAME_SNAKE_H

#include <iostream>
#include <list>

class snake {
public:
    struct node {
        int x, y, z;
    };
    std::list<node> s;

    explicit snake(int num) {
        for (int i = 0; i < num; i++)
            s.push_back({i, 0, 1});
    }

    void updateSnake(int nowDir) {
        node newNode = s.front();
        switch (nowDir) {
            case 0:
                newNode.x++;
                break;
            case 1:
                newNode.x--;
                break;
            case 2:
                newNode.y++;
                break;
            case 3:
                newNode.y--;
                break;
            case 4:
                newNode.z++;
                break;
            case 5:
                newNode.z--;
                break;
            default:
                return;
        }
        s.push_front(newNode);
        s.pop_back();
    }

    void eatFood(int nowDir) {
        node newNode = s.front();
        switch (nowDir) {
            case 0:
                newNode.x++;
                break;
            case 1:
                newNode.x--;
                break;
            case 2:
                newNode.y++;
                break;
            case 3:
                newNode.y--;
                break;
            case 4:
                newNode.z++;
                break;
            case 5:
                newNode.z--;
                break;
            default:
                return;
        }
        s.push_front(newNode);
    }
};

#endif //OPENGLGAME_SNAKE_H
