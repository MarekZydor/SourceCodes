//
// Created by QwerTY on 31.12.2021.
//

#ifndef KRUSKAL_NODE_H
#define KRUSKAL_NODE_H

class Node {
private:
    float x,y;
public:
    Node(float x, float y);
    float getX() const;
    float getY() const;
};

Node::Node(float x, float y) : x(x), y(y) {}

float Node::getX() const {
    return x;
}

float Node::getY() const {
    return y;
}


#endif //KRUSKAL_NODE_H
