#ifndef RED_BLACK_NODE_H
#define RED_BLACK_NODE_H

template <typename T>
struct node {
    T data;
    int key;
    bool color;
    node *left, *right, *parent;

    // Constructor
    node(T data, int key) {
        this->data = data;
        this->key = key;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

#endif
