#ifndef RED_BLACK_NODE_H
#define RED_BLACK_NODE_H

enum Color { RED, BLACK };

template <typename T>
struct rbnode {
    rbnode(T value, long key) {
        this->value = value;
        this->key = key;
        left = right = parent = nullptr;
        this->color = RED;
    }

    T value;
    long key;
    bool color;
    rbnode *left, *right, *parent;
};

#endif
