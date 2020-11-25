#ifndef RED_BLACK_NODE_H
#define RED_BLACK_NODE_H

enum Color { RED, BLACK };

template <typename T>
struct rbnode {
    rbnode(T data, long key) {
        this->data = data;
        this->key = key;
        left = right = parent = nullptr;
        this->color = RED;
    }

    T data;
    long key;
    bool color;
    rbnode *left, *right, *parent;
};

#endif
